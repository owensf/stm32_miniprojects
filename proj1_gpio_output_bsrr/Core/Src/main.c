#include "stm32f051x8.h"

#define GPIOCEN (1U << 19)
// set pin 8 = bit 8
#define PIN8 (1U <<8)
#define LED_PIN PIN8

int main(void){
	RCC -> AHBENR |= GPIOCEN;

	GPIOC -> MODER |= (1U <<16);
	GPIOC -> MODER &= ~(1U<<17);
	while(1){
		GPIOC->BSRR |= PIN8;
		for(int i = 0; i < 100000; i++){}
		GPIOC->BSRR |= (1U << 24);
		for(int i = 0; i < 100000; i++){}
	}
}
