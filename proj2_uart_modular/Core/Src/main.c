#include <stdio.h>
#include <stdint.h>
#include "stm32f051x8.h"
#include "uart.h"


int main(void){
	uart2_tx_init();
	while(1){

		// sending 'Y': 10010101 or 149
		// sending 'X': 10000101 or 133

		//uart2_write(2U): 00100000

		printf("Hello world!\n\r");
	}
}
