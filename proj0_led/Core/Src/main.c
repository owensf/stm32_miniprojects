// where is the LED connected?
// what is the port: C
// what is the pin: 8/9


#define PERIPH_BASE (0x40000000UL)
// GPIOC is in AHB2
// Starting point of AHB2: 0x48000000
#define AHB2PERIPH_OFFSET (0x8000000UL)
#define AHB2PERIPH_BASE PERIPH_BASE + AHB2PERIPH_OFFSET
#define GPIOC_OFFSET (0x800U)
#define GPIOC_BASE AHB2PERIPH_BASE + GPIOC_OFFSET

// RCC is in AHB1
#define AHB1PERIPH_OFFSET (0X0020000UL)
#define AHB1PERIPH_BASE PERIPH_BASE + AHB1PERIPH_OFFSET
#define RCC_OFFSET (0X1000UL)
#define RCC_BASE AHB1PERIPH_BASE + RCC_OFFSET
// seems like there's only 1 AHB EN??
#define AHBEN_OFFSET (0x14UL)
#define RCC_AHBENR (*(volatile unsigned int *) (RCC_BASE + AHBEN_OFFSET))
// GPIOC EN: bit 19
#define GPIOCEN (1U << 19)
#define MODER_OFFSET (0x0UL)
#define GPIOC_MODER (*(volatile unsigned int *) (GPIOC_BASE + MODER_OFFSET))
/*
// for GPIOC_8, pin 8 = bit 16
(1U << 16) // set bit 16 to 1
&= ~(1U<<17) set bit 16 to 0*/

#define ODR_OFFSET 0x14UL
#define GPIOC_ORDR (*(volatile unsigned int *) (GPIOC_BASE + ODR_OFFSET))
// set pin 8 = bit 8
#define PIN8 (1U <<8)
#define LED_PIN PIN8

int main(void){
	/* 1. Enable clock access to GPIOC */
	RCC_AHBENR |= GPIOCEN;
	 /* 2. Set PC8 as output pin */
	GPIOC_MODER |= (1U << 16);
	GPIOC_MODER &= ~(1U << 17);
	while(1){
		/* 3. Set PC8 high*/
		//GPIOC_ORDR |= LED_PIN;
		/*4. Experiment: toggle PC8*/
		 GPIOC_ORDR ^= LED_PIN;
		 for(int i = 0; i < 100000; i++){
			 ;;
		 }
	}
}
