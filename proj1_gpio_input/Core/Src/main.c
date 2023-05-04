#include "stm32f051x8.h"

#define GPIOCEN (1U << 19)
#define GPIOAEN (1U << 17)
// set pin 8 = bit 8
#define PIN9 (1U <<9)
#define PIN0 (1U <<0)
#define LED_PIN PIN9
#define BTN_PIN PIN0

// push button = A0

int main(void){
	/* Enable clock access to GPIOC/GPIOA */
	RCC -> AHBENR |= GPIOCEN | GPIOAEN;

	/* Set PC9 as output pin */
	GPIOC -> MODER |= (1U <<18);
	GPIOC -> MODER &= ~(1U<<19);
	/* Set PB1 as input pin */
	GPIOA -> MODER &= ~(3U << 0);
	while(1){
		/* Check if button is pressed */
		if (GPIOA->IDR & BTN_PIN){
			GPIOC->BSRR = PIN9;
		}
		else{
			GPIOC->BSRR = (1U << 25);
		}
	}
}
