#ifndef __USART1_H
#define	__USART1_H
#define RX_BUF_SIZE 32
// 添加UART2相关的定义和声明
#define RX2_BUF_SIZE 128  // UART2接收缓冲区大小
#include "stm32f10x.h"
#include <stdio.h>


void USART2_Config(void);
void USART2_IRQHandler(void);

extern volatile uint8_t rx_buf[RX_BUF_SIZE];
extern volatile uint8_t rx_flag;
extern volatile uint8_t rx2_buf[RX2_BUF_SIZE];
extern volatile uint16_t rx2_index;
extern volatile uint8_t rx2_flag;  // UART2接收完成标志
void USART1_Config(void);
int fputc(int ch, FILE *f);
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);

#endif /* __USART1_H */
