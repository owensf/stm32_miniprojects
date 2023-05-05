#include "stm32f051x8.h"

#define GPIOCEN (1U << 19)
#define GPIOBEN (1U << 18)
// set pin 9 = bit 9
#define PIN9 (1U <<9)


// GPIO ROW starts on GPIOB4
#define GPIO_ROW_OFFSET (1U << 4)



/* GPIOB
 * 4:Row 1
 * 5: Row 2
 * 6: Row 3
 * 7: Row 4
 *
 * 13: col 1
 * 14: col2
 * 15: col3
 */

/* Init GPIOC LED + GPIOBs for keyboard
 */
void init_gpios(void){
	/* Enable clock access to GPIOB/GPIOC */
	RCC -> AHBENR |= GPIOBEN | GPIOCEN;
	/* Set GPIOB 4-7 as output pins */
	GPIOB -> MODER |= (1U << 8) | (1U << 10) | (1U << 12) | (1U << 14);
	GPIOB -> MODER &= ~((1U << 9) | (1U << 11) | (1U << 13) | (1U << 15));
	/* Set GPIOB 13-15 as input pins */
	GPIOB -> MODER &= ~((3U << 26) | (3U << 28) | (3U << 30));
	/* activate GPIOB 13-15 pull-downs */
	GPIOB -> PUPDR |= (2U << 26) | (2U << 28) | (2U << 30);
	GPIOB -> PUPDR &= ~((1U << 26) | (1U << 28) | (1U < 30));
	/* Set GPIOC9 as output pin */
	GPIOC -> MODER |= (1U << 18);
	GPIOC -> MODER &= ~(1U << 19);
}

/* sleep for n clock cycles */
void custom_sleep(int clockcycles){
	for (int i = 0; i < clockcycles; i++)
		;
}

/* scan through keypad rows and find which button is being pressed
 * 10 = *
 * 11 = 0
 * 12 = #
 * */

int read_keypad(void){
	int key_pressed = -1;
	for (int i = 0; i < 4; i++){
		/* cycle through setting each GPIO row */
		GPIOB -> BSRR = GPIO_ROW_OFFSET << i;
		custom_sleep(1000);
		/* see if a column is pressed */
		uint32_t col_pressed = (GPIOB -> IDR & (7U << 13)) >> 13;
		// if col_pressed = 0b100, column 3 is pressed. but we want it as an int
		if (col_pressed == 4U){
			col_pressed = 3U;
		}
		if (col_pressed > 0){
			key_pressed = col_pressed + 3 * i;
		}
		// reset GPIOB
		GPIOB -> BSRR = (GPIO_ROW_OFFSET << i) << 16;
	}
	return key_pressed;
}

int blink_light(int num_times){
	for (int i = 0; i < num_times; i++){
		// turn on
		GPIOC -> BSRR = PIN9;
		custom_sleep(100000);
		// turn off
		GPIOC -> BSRR = (1U << 25);
		custom_sleep(100000);
	}
}

int main(void){

	init_gpios();
	while(1){
		int num_blinks = read_keypad();
		blink_light(num_blinks);
		custom_sleep(100000);
	}
}
