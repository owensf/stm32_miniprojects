#include "stm32f051x8.h"
#include <stdint.h>

#define GPIOCEN (1U << 19)
#define GPIOAEN (1U << 17)
#define UART2EN (1U << 17)

#define SYS_FREQ 8000000 // 8MHz
#define APB1_CLK SYS_FREQ

#define UART_BAUDRATE 57600

#define CR1_TE (1U << 3)
#define CR1_UE (1U << 0)
#define ISR_TXE (1U << 7)
/*
 * USART2 TX: PA2 AF1
 * USART2 RX: PA3: AF1
 * USART2 CTS PA0 AF1
 * USART2 RTS PA1 AF1
 * USART2 CK PA3 AF1
 */

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t Baudrate);
static uint16_t compute_uart_div(uint32_t PeriphClk, uint32_t Baudrate);
void uart2_write(uint8_t ch);
void uart2_tx_init(void);


int main(void){
	uart2_tx_init();
	while(1){

		// sending 'Y': 10010101 or 149
		// sending 'X': 10000101 or 133

		//uart2_write(2U): 00100000

		uart2_write(2U);
	}
}

void uart2_tx_init(void){
	/* Configure UART GPIO Pin
	 * Enable clk access to gpioa
	 * Set PA2 mode to alternate function mode
	 * Set PA2 alternate function type to UART_TX (AF1)
	 */
	RCC -> AHBENR |= GPIOAEN;
	GPIOA -> MODER &= ~(1U << 4);
	GPIOA -> MODER |= (1U << 5);
	GPIOA -> AFR[0] &= ~(14U << 8);
	GPIOA -> AFR[0] |= (1U << 8);

	//select pull up
	//GPIOA -> PUPDR |= (2U << 4);

	/* configure USART Module
	 * enable clock access to uart2
	 * Configure baudrate
	 * Configure the transfer direction
	 * Enable UART module
	 */
	RCC -> APB1ENR |= UART2EN;
	uart_set_baudrate(USART2,APB1_CLK, UART_BAUDRATE);
	USART2 -> CR1 = CR1_TE;
	USART2 -> CR1 |= CR1_UE;

}

void uart2_write(uint8_t ch){
	/* Make sure the transmit date register is empty
	 * Write to transmit data register
	 */
	while (!(USART2 -> ISR & ISR_TXE)){}
	USART2 -> TDR = ch;
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t Baudrate){
	USARTx -> BRR = compute_uart_div(PeriphClk, Baudrate);
}

static uint16_t compute_uart_div(uint32_t PeriphClk, uint32_t Baudrate){
	return ((PeriphClk + (Baudrate/2U))/Baudrate);
}
