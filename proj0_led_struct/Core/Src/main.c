// where is the LED connected?
// what is the port: C
// what is the pin: 8/9
#include <stdint.h>

#define PERIPH_BASE (0x40000000UL)
// GPIOC is in AHB2
// Starting point of AHB2: 0x48000000
#define AHB2PERIPH_OFFSET (0x8000000UL)
#define AHB2PERIPH_BASE (PERIPH_BASE + AHB2PERIPH_OFFSET)
#define GPIOC_OFFSET (0x800U)
#define GPIOC_BASE (AHB2PERIPH_BASE + GPIOC_OFFSET)

// RCC is in AHB1
#define AHB1PERIPH_OFFSET (0X0020000UL)
#define AHB1PERIPH_BASE (PERIPH_BASE + AHB1PERIPH_OFFSET)
#define RCC_OFFSET (0X1000UL)
#define RCC_BASE (AHB1PERIPH_BASE + RCC_OFFSET)
// seems like there's only 1 AHB EN??
#define AHBEN_OFFSET (0x14UL)
// GPIOC EN: bit 19
#define GPIOCEN (1U << 19)

// set pin 8 = bit 8
#define PIN8 (1U <<8)
#define LED_PIN PIN8

typedef struct{
	volatile uint32_t MODER;
	volatile uint32_t DUMMY[4];
	volatile uint32_t ODR;
} GPIO_TypeDef;


typedef struct
{
  volatile uint32_t DUMMY[5];
  volatile uint32_t AHBENR;     /*!< RCC AHB peripheral clock register,                           Address offset: 0x14 */
} RCC_TypeDef;

#define RCC ((RCC_TypeDef *) RCC_BASE)
#define GPIOC ((GPIO_TypeDef *) GPIOC_BASE)

int main(void){
	/* 1. Enable clock access to GPIOC */
	RCC -> AHBENR |= GPIOCEN;
	/* 2. Set PC8 as output pin */
	GPIOC->MODER |= (1U << 16);
	GPIOC->MODER &= ~(1U << 17);
	while(1){
		GPIOC->ODR ^= LED_PIN;
		for(int i = 0; i < 100000; i++){
			 ;;
		}
	}
}
