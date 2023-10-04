/* Embedded Systems - Exercise 6 */


#include <tinyprintf.h>
#include <stm32f4/rcc.h>
#include <stm32f4/gpio.h>
#include <stm32f4/tim.h>
#include <stm32f4/nvic.h>

// GPIOD
#define GREEN_LED	12
#define ORANGE_LED	13
#define RED_LED		14
#define BLUE_LED	15

// GPIODA
#define USER_BUT	0



void handle_TIM4()  {
	static char *s1 = "!", *s2 = "?";
	TIM4_SR = ~TIM_UIF;
	while((TIM4_SR & TIM_UIF) == 1);
	//for(int i = 0; i < 9; i++) NOP;
	/*NOP; NOP; NOP; NOP; 
	NOP; NOP; NOP; NOP; 
	NOP; NOP;*/
	//NOP; NOP; NOP; NOP; 
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


