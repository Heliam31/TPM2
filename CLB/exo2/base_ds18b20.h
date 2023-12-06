#ifndef BASE_DS18B20_H__
# define BASE_DS18B20_H__

# define NUM_RETRIES 50

/* Constantes DS18B20 */
/* Temps de conversion, cf. datasheet tableau 2 page 9. */
static const u16 ds18b20_conv_time[4] = {94, 188, 375, 750};

/* Résolutions minimales et maximales, cf. datasheet tableau 2 page 9. */
# define DS18B20_MIN_RESOLUTION 9
# define DS18B20_MAX_RESOLUTION 12

/* Nombre de registres dans le scratchpad, cf. datasheet figure 9 page 8. */
# define DS18B20_NUM_REGS 9

/* Numéro des registres dans le scratchpad, cf. datasheet figure 9 page 8. */
# define DS18B20_REG_TEMP_LSB 0
# define DS18B20_REG_TEMP_MSB 1
# define DS18B20_REG_TH 2
# define DS18B20_REG_TL 3
# define DS18B20_REG_CONF 4
# define DS18B20_REG_CRC 8

/* Résolutions possibles, cf datasheet tableau 2 page 9. */
# define DS18B20_RES_9B 0b00
# define DS18B20_RES_10B 0b01
# define DS18B20_RES_11B 0b10
# define DS18B20_RES_12B 0b11

/**
 * Envoie un reset sur le bus, permettant d'adresser un nouveau périphérique et
 * d'envoyer une nouvelle commande.  Ré-essaye NUM_RETRIES fois.  En cas
 * d'erreur, écrit un message dans le log noyau et renvoie -ECANCELED.
 *
 * @returns 0 en cas de succès, -ECANCELED en cas d'erreur.
 */
int onewire_reset(void);

/**
 * Lit un octet sur le bus et le retourne.
 */
u8 onewire_read_byte(void);

/**
 * Écrit un octet sur le bus.
 *
 * @parameter val La valeur à écrire sur le bus.
 */
void onewire_write_u8(u8 val);

/**
 * Écrit 64 bits sur le bus.
 *
 * @parameter val La valeur à écrire sur le bus.
 */
void onewire_write_u64(u64 val);

/**
 * Calcule le CRC d'une liste d'octets.
 *
 * @parameter data Liste d'octets dont il faut calculer la somme de contrôle.
 * @parameter len Le nombre d'octets.
 * @returns Le CRC de la liste.
 */
u8 onewire_crc8(const u8 *data, size_t len);

/**
 * Sélectionne le périphérique d'adresse `rom`.
 *
 * @parameter rom L'adresse du périphérique à sélectionner.
 * @returns 0 en cas de succès, -ECANCELED en cas d'erreur.
 */
int ds18b20_select(u64 rom);

/**
 * Énumère la liste des capteurs de température présents sur le bus.
 *
 * Exemple d'utilisation :
 *
 *     u64 *roms = NULL;
 *     int res, retries = 0;
 *
 *     do {
 *         res = ds18b20_search(&roms);
 *     } while (retries++ < NUM_RETRIES && res == -EAGAIN);
 *
 *     // Gestion des erreurs
 *
 * @parameter roms Un pointeur vers la liste de capteurs à créer.  Sa valeur
 * sera modifiée.  Si des capteurs ont été trouvés, un nouveau pointeur sera
 * alloué.  Si aucun capteur n'a été trouvé, ou en cas d'erreur, sa valeur
 * deviendra NULL.
 * @returns Le nombre de capteurs détectés, ou -EAGAIN ou -ECANCELED en cas d'erreur.
 */
int ds18b20_search(u64 **roms);

/**
 * Alloue le pin `bus_pin`.
 *
 * @parameter bus_pin Le pin à allouer.
 * @parameter dev Device à associer au pin.
 * @returns 0 en cas de succès, une valeur négative en cas d'erreur.
 */
int init_base_ds18b20(int bus_pin, struct device *dev);

/**
 * Libère le pin alloué.
 */
void deinit_base_ds18b20(void);

#endif  /* BASE_DS18B20_H__ */
