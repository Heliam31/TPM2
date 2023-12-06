#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/gpio/consumer.h>
#include <linux/gpio/machine.h>

#include "base_ds18b20.h"

GPIO_LOOKUP_SINGLE(gpios_table, NULL, "pinctrl-bcm2835", 17, "onewire-bus", GPIO_ACTIVE_HIGH);

static struct gpio_desc *busdesc;

/* Fonctions onewire */
static inline void onewire_high(void)
{
	/* Bus à l'état haut réalisé en plaçant le pin en mode entrée. */
	gpiod_direction_input(busdesc);
}

static inline void onewire_low(void)
{
	/* Bus à l'état bas réalisé en plaçant le pin en mode sortie, valeur
	   0. */
	gpiod_direction_output(busdesc, 0);
}

int onewire_reset(void)
{
	int val, was_pulled_down = 0, retries = 0;

	onewire_low();
	usleep_range(480, 500);
	onewire_high();

	usleep_range(15, 20);

	do {
		udelay(5);
		val = gpiod_get_value(busdesc);

		if (!was_pulled_down && !val)
			was_pulled_down = 1;
		++retries;
	} while ((!was_pulled_down || (was_pulled_down && !val)) && retries < NUM_RETRIES);

	if (retries == NUM_RETRIES) {
		printk(KERN_ALERT "failed to reset onewire bus (%d retries)\n", NUM_RETRIES);
		return -ECANCELED;
	}

	udelay(5);
	return 0;
}

static inline int onewire_read(void)
{
	int res;

	/* Ouverture d'une fenêtre de lecture, cf. datasheet page 16. */
	onewire_low();
	udelay(5);
	onewire_high();

	/* Moins de 15µs après le front descendant, si le bus est à l'état bas,
	   un 0 a été écrit, sinon un 1 a été écrit. */
	udelay(7);
	res = gpiod_get_value(busdesc);
	usleep_range(50, 55);

	return res;
}

u8 onewire_read_byte(void)
{
	int i;
	u8 val = 0;

	/* Lecture d'un octet. */
	for (i = 0; i < 8; ++i)
		val |= onewire_read() << i;

	return val;
}

static void onewire_write_zero(void)
{
	/* Écriture d'un 0 : on met le bus à l'état bas pour 60µs, cf. datasheet
	   page 15. */
	onewire_low();
	usleep_range(60, 65);
	onewire_high();
	udelay(5);
}

static void onewire_write_one(void)
{
	/* Écriture d'un 1 : on met le bus à l'état bas pour 5µs, et on attend
	   60µs, cf. datasheet page 15. */
	onewire_low();
	udelay(5);
	onewire_high();
	usleep_range(60, 65);
}

#define ONEWIRE_DEFINE_WRITE(type)			\
	void onewire_write_##type(type val)		\
	{						\
		type mask;				\
		for (mask = 1; mask; mask <<= 1) {	\
			if (val & mask)			\
				onewire_write_one();	\
			else				\
				onewire_write_zero();	\
		}					\
	}

ONEWIRE_DEFINE_WRITE(u8);
ONEWIRE_DEFINE_WRITE(u64);

#define ONEWIRE_CRC8_INIT 0  /* Shift register initialement à 0. */

static u8 onewire_crc8_step(u8 shift_register, u8 byte)
{
	u8 mask;

	for (mask = 1; mask; mask <<= 1) {
		/* input = (bit courant) xor shifted_register[0] */
		u8 input = (!!(byte & mask)) ^ (shift_register & 0x01);

		/* Shift à gauche du registre, masquage des bits 3 et 4 pour les
		   remplacer. */
		u8 shifted_register = (shift_register >> 1) & ~0x0C;

		/* shifted_register[2] = input xor shift_register[3] */
		shifted_register |= ((!!(shift_register & 0x08)) ^ input) << 2;

		/* shifted_register[3] = input xor shift_register[4] */
		shifted_register |= ((!!(shift_register & 0x10)) ^ input) << 3;

		/* shifted_register[7] = input */
		shifted_register |= input << 7;
		shift_register = shifted_register;
	}

	return shift_register;
}

u8 onewire_crc8(const u8 *data, size_t len)
{
	size_t i;
	u8 shift_register = ONEWIRE_CRC8_INIT;

	/* Calcul d'un CRC, selon la description sur la datasheet page 9. */
	for (i = 0; i < len; ++i)
		shift_register = onewire_crc8_step(shift_register, data[i]);
	return shift_register;
}

static u8 onewire_crc8_rom(u64 data)
{
	unsigned int shift;
	u8 shift_register = ONEWIRE_CRC8_INIT;

	/* Le dernier octet correspond au CRC, on ne s'en sert pas pour le
	   calculer. */
	for (shift = 0; shift < 56; shift += 8) {
		u8 byte = (u8) ((data & (((u64) 0xFF) << shift)) >> shift);
		shift_register = onewire_crc8_step(shift_register, byte);
	}

	return shift_register;
}

/* Appel des fonctions du DS18B20 */
int ds18b20_select(u64 rom)
{
	int res;

	/* Sélection du périphérique à partir de son numéro de série, cf.
	   datasheet page 11. */
	if ((res = onewire_reset()) < 0)
		return res;

	onewire_write_u8(0x55);
	onewire_write_u64(rom);

	return 0;
}

static int ds18b20_next(u64 mask, u64 *rom, u64 *discrepancies, s8 *last_disc, u8 orig_pos)
{
	u8 bit, comp, pos;

	for (pos = orig_pos; pos < 64; ++pos, mask <<= 1) {
		bit = onewire_read();
		comp = onewire_read();

		if (!bit) {
			if (!comp) {
				*discrepancies |= mask;
				*last_disc = pos;
			}

			*rom &= ~mask;
			onewire_write_zero();
		} else if (bit && !comp) {
			*rom |= mask;
			onewire_write_one();
		} else if (pos > orig_pos) {
			/* Les capteurs de température ont arrêté de répondre au
			   milieu de nulle part, on relance la recherche. */
			return -EAGAIN;
		} else {
			return -ECANCELED;
		}
	}

	return 0;
}

static int ds18b20_backtrack(u64 *rom, u64 *discrepancies, s8 *last_disc)
{
	u64 mask = 0x1, discmask;
	int res;
	u8 bit, comp, pos = 0;

	if ((res = onewire_reset()) < 0)
		return res;

	/* Mode recherche, cf. datasheet page 10 */
	onewire_write_u8(0xF0);

	if (*last_disc >= 0) {
		for (; pos <= *last_disc && pos < 64; ++pos, mask <<= 1) {
			bit = onewire_read();
			comp = onewire_read();

			if (pos < *last_disc && (*rom & mask))
				onewire_write_one();
			else if (pos < *last_disc)
				onewire_write_zero();
			else {
				onewire_write_one();
				*rom |= mask;
				*discrepancies &= ~mask;
			}
		}

		for (discmask = mask >> 1;
		     discmask && !(*discrepancies & discmask);
		     discmask >>= 1, --(*last_disc));

		if (bit && comp)
			return 0;
	}

	return ds18b20_next(mask, rom, discrepancies, last_disc, pos);
}

int ds18b20_search(u64 **roms)
{
	u64 *tmp_roms = NULL, rom = 0, discrepancies = 0;
	int res = 0, nr = 0;
	s8 last_disc = -1;

	do {
		if (!(res = ds18b20_backtrack(&rom, &discrepancies, &last_disc))) {
			if (rom >> 56 != onewire_crc8_rom(rom)) {
				printk(KERN_ALERT "CRC check failed\n");
				return -1;
			}

			if ((tmp_roms = krealloc(*roms, sizeof(u64) * (++nr), GFP_KERNEL)) == NULL)
				return -ENOMEM;

			tmp_roms[nr - 1] = rom;
			*roms = tmp_roms;
		}
	} while (last_disc != -1 && res == 0);

	if (res < 0) {
		if (*roms)
			kfree(*roms);
		*roms = NULL;
		return res;
	}

	*roms = tmp_roms;
	return nr;
}

int init_base_ds18b20(int bus_pin, struct device *dev)
{
	/* Copy to avoid use-after-free bugs due to incorrect ordering of
	 * deinit_base_ds18b20()/device_destroy(). */
	if ((gpios_table.dev_id = kstrdup_const(dev_name(dev), GFP_KERNEL)) == NULL)
		return -ENOMEM;

	gpios_table.table[0].chip_hwnum = bus_pin;
	gpiod_add_lookup_table(&gpios_table);

	busdesc = gpiod_get(dev, "onewire-bus", GPIOD_OUT_LOW);
	if (IS_ERR(busdesc)) {
		gpiod_remove_lookup_table(&gpios_table);
		kfree(gpios_table.dev_id);
		return PTR_ERR(busdesc);
	}

	return 0;
}

void deinit_base_ds18b20(void)
{
	gpiod_put(busdesc);
	gpiod_remove_lookup_table(&gpios_table);
	kfree(gpios_table.dev_id);
}
