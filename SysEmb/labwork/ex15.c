/* Embedded Systems - Exercise 15 */

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

// GPIODA
#define USER_BUT	0


void init_servo(){
	GPIOA_MODER = REP_BITS(GPIOA_MODER, 6*2,2,GPIO_MODER_ALT);
	GPIOA_AFRL = REP_BITS(GPIOA_AFRL, 6*4, 4, 2);

	TIM3_CR1 = 0;
	TIM3_CCMR1 = TIM_CCS1S_OUT|TIM_OC1M_PWM1;
	TIM3_CCER = TIM_CC1E;
	TIM3_CCR1 = 60000/20;
	TIM3_ARR = 60000;
	TIM3_PSC = 14 - 1;
	TIM3_EGR = TIM_UG;
	TIM3_CR1 = TIM_CEN|TIM_ARPE;
}

void set_servo(int n){
	TIM3_CCR1 = (60000/20) + (n*60000/20/180);
	}

int main() {
	printf("\nStarting...\n");

	// RCC init
	RCC_AHB1ENR |= RCC_GPIOAEN;
	RCC_AHB1ENR |= RCC_GPIODEN;
	RCC_APB1ENR |= RCC_TIM3EN;
	RCC_APB2ENR |= RCC_ADC1EN;

	// initialization
	init_servo();

	// main loop
	printf("Endless loop!\n");
	while(1) {
		set_servo(0);
		printf("0!\n");
		for (int i = 0; i< 100000000; i++){
			NOP;
		}
		set_servo(180);
		printf("180!\n");
		for (int i = 0; i< 100000000; i++){
			NOP;
		}
	}

}


