/***************STM32F103C8T6**********************
 * 文件名  ：usart1.c
 * 描述    ：将printf函数重定向到USART1。
 * 硬件连接：------------------------
 *          | PA9  - USART1(Tx)      |
 *          | PA10 - USART1(Rx)      |
 *           ------------------------
 * 库版本  ：ST3.0.0  *

********************LIGEN*************************/

#include "usart.h"
#include <stdarg.h>
volatile uint8_t rx_buf[RX_BUF_SIZE];
volatile uint16_t rx_index = 0;
volatile uint8_t rx_flag = 0; // 接收完成标志
volatile uint8_t rx2_buf[RX2_BUF_SIZE];
volatile uint16_t rx2_index = 0;
volatile uint8_t rx2_flag = 0;
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 使能 USART1 时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); 
	

	/* USART1 使用IO端口配置 */    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);    
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOA
	  
	/* USART1 工作模式配置 */
	USART_InitStructure.USART_BaudRate = 115200;	//波特率设置：115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;  //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收与发送都使能
	USART_Init(USART1, &USART_InitStructure);  //初始化USART1
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);


	USART_Cmd(USART1, ENABLE);// USART1使能
}


 /* 描述  ：重定向c库函数printf到USART1*/ 
int fputc(int ch, FILE *f)
{
/* 将Printf内容发往串口 */
  USART_SendData(USART1, (unsigned char) ch);
  while (!(USART1->SR & USART_FLAG_TXE));
 
  return (ch);
}

void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		
        uint8_t data = USART_ReceiveData(USART1);

        // 检测帧结束符 "\r\n"
        if (data == '\r') {
            // 忽略'\r'，等待'\n'
        } else if (data == '\n') {
            rx_buf[rx_index] = '\0'; // 添加字符串结束符
            rx_flag = 1;             // 设置接收完成标志
            rx_index = 0;
        } else {
            // 存储数据到缓冲区
            if (rx_index < RX_BUF_SIZE - 1) {
                rx_buf[rx_index++] = data;
            } else {
                // 缓冲区溢出处理
                rx_index = 0;
            }
        }
    }
}



// UART2初始化函数
void USART2_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /* 启用时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    /* 配置GPIO */
    // TX (PA2) 和 RTS (PA1) 配置为复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // RX (PA3) 配置为浮空输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 配置UART2参数 */
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 启用RTS
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);

    /* 配置中断 */
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // 启用接收中断

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 优先级低于USART1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART2, ENABLE);
}

// UART2中断服务函数
void USART2_IRQHandler(void) {
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        uint8_t data = USART_ReceiveData(USART2);

        // 检测帧结束符 "\r\n"
        if (data == '\r') {
            // 忽略'\r'
        } else if (data == '\n') {
            rx2_buf[rx2_index] = '\0'; // 添加字符串结束符
            rx2_flag = 1;              // 设置接收完成标志
            rx2_index = 0;
        } else {
            if (rx2_index < RX2_BUF_SIZE - 1) {
                rx2_buf[rx2_index++] = data;
            } else {
                rx2_index = 0; // 缓冲区溢出处理
            }
        }
    }
}
void UART_SendByte(unsigned char Send_Dat)
{
    // 发送数据
    USART_SendData(USART2, Send_Dat);
    // 等待发送完成（TXE标志置位表示数据已转移到移位寄存器）
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}
