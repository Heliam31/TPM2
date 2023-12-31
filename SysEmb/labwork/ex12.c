/* Embedded Systems - Exercise 12 */

#include <tinyprintf.h>
#include <stm32f4/rcc.h>
#include <stm32f4/gpio.h>
#include <stm32f4/nvic.h>
#include <stm32f4/exti.h>
#include <stm32f4/syscfg.h>
#include <stm32f4/tim.h>
#include <stm32f4/adc.h>


// GPIOD
#define GREEN_LED	12
#define ORANGE_LED	13
#define RED_LED		14
#define BLUE_LED	15

#define RES		 	3
//BOUTTON PD1
#define LED 		1  
// GPIODA
#define USER_BUT	0

void init_ADC(){
	GPIOA_MODER = REP_BITS(GPIOA_MODER, RES*2,2,GPIO_MODER_ANA);
	ADC1_SQR3=3;
	ADC1_CR1 = 0;
	ADC1_CR2 = ADC_ADON ;

	//Init led
	GPIOD_MODER = REP_BITS(GPIOD_MODER, LED*2, 2, GPIO_MODER_OUT);
	GPIOD_OTYPER = GPIOD_OTYPER && ~(1<<LED);
}
int main() {
	printf("\nStarting...\n");
	
	// RCC init
	RCC_AHB1ENR |= RCC_GPIOAEN;
	RCC_AHB1ENR |= RCC_GPIODEN;
	RCC_APB1ENR |= RCC_TIM4EN;
	RCC_APB2ENR |= RCC_ADC1EN;

	// initialization
	init_ADC();
	// main loop
	printf("Endless loop!\n");
	while(1) {
		ADC1_CR2|= ADC_SWSTART;
		while((ADC1_SR & ADC_EOC) == 0){
		}
		long mesure = ADC1_DR;
		printf("mesure %ld\n",mesure);
		if (mesure < 2000){
			GPIOD_BSRR = 1<<LED;
		}
		else{
			GPIOD_BSRR = 1 <<(16+LED);
		}
	}

}


