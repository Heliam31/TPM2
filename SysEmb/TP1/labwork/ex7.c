/* Embedded Systems - Exercise 7 */

#include <tinyprintf.h>
#include <stm32f4/rcc.h>
#include <stm32f4/gpio.h>
#include <stm32f4/nvic.h>
#include <stm32f4/exti.h>
#include <stm32f4/syscfg.h>
#include <stm32f4/tim.h>


// GPIOD
#define GREEN_LED	12
#define ORANGE_LED	13
#define RED_LED		14
#define BLUE_LED	15

// GPIODA
#define USER_BUT	0


void handle_button() {
}

void init_TIM4(){
	DISABLE_IRQS;
	//Butt mapped to PA0/EXTI0
	//config EXTI for GPIOA(0)
	SYSCFG_EXTICR1 = REP_BITS(SYSCFG_EXTICR1, 0, 4, 0);
	EXTI_RTSR |= 1 << 0; //rising edge
	EXTI_FTSR |= 1 << 0; //falling edge
	EXTI_IMR |= 1 << 0; //enable
	EXTI_PR |= 1 << 0; //clear pending
}

int main() {
	printf("\nStarting...\n");

	// RCC init
	RCC_AHB1ENR |= RCC_GPIOAEN;
	RCC_AHB1ENR |= RCC_GPIODEN;
	RCC_APB1ENR |= RCC_TIM4EN;


	// main loop
	printf("Endless loop!\n");
	while(1) {
	}

}


