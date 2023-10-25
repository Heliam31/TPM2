/* Embedded Systems - Exercise 3 */

#include <tinyprintf.h>
#include <stm32f4/rcc.h>
#include <stm32f4/gpio.h>


// GPIOD
#define GREEN_LED	12
#define ORANGE_LED	13
#define RED_LED		14
#define BLUE_LED	15
#define LED 		7
// GPIODA
#define BUT	2
void init(){
	GPIOA_MODER = REP_BITS(GPIOA_MODER, BUT*2 , 2, GPIO_MODER_IN);
	GPIOA_PUPDR = REP_BITS(GPIOA_PUPDR, BUT*2 , 2, GPIO_PUPDR_PD);

	GPIOB_MODER = REP_BITS(GPIOB_MODER, LED*2, 2, GPIO_MODER_OUT);
	GPIOB_OTYPER = GPIOB_OTYPER && ~(1<<LED);
}

int main() {
	printf("Starting...\n");

	// RCC init
	RCC_AHB1ENR |= RCC_GPIOAEN;
	RCC_AHB1ENR |= RCC_GPIODEN;

	// GPIO init
	init();
	int B1_state = 0;

	printf("Endless loop!\n");
	while(1) {
		if(B1_state == 0){
			if (GPIOA_IDR & (1<<0) != 0){
				GPIOD_BSRR = (1 << LED);	
				GPIOD_BSRR = (1 << RED_LED+16);
				B1_state = 1;
			}
		}
		else {
			if (GPIOA_IDR & (1<<0) != 0){
				GPIOD_BSRR = (1 << LED+16);	
				GPIOD_BSRR = (1 << RED_LED);
				B1_state = 0;
			}
		}
		for (int i = 0; i< 30000000/3; i++){
			NOP;
		}

	}
}
