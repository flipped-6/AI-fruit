/***************STM32F103C8T6**********************
 * �ļ���  ��usart1.c
 * ����    ����printf�����ض���USART1��
 * Ӳ�����ӣ�------------------------
 *          | PA9  - USART1(Tx)      |
 *          | PA10 - USART1(Rx)      |
 *           ------------------------
 * ��汾  ��ST3.0.0  *

********************LIGEN*************************/

#include "usart.h"
#include <stdarg.h>
volatile uint8_t rx_buf[RX_BUF_SIZE];
volatile uint16_t rx_index = 0;
volatile uint8_t rx_flag = 0; // ������ɱ�־
volatile uint8_t rx2_buf[RX2_BUF_SIZE];
volatile uint16_t rx2_index = 0;
volatile uint8_t rx2_flag = 0;
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* ʹ�� USART1 ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); 
	

	/* USART1 ʹ��IO�˿����� */    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);    
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);   //��ʼ��GPIOA
	  
	/* USART1 ����ģʽ���� */
	USART_InitStructure.USART_BaudRate = 115200;	//���������ã�115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;  //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�����뷢�Ͷ�ʹ��
	USART_Init(USART1, &USART_InitStructure);  //��ʼ��USART1
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);


	USART_Cmd(USART1, ENABLE);// USART1ʹ��
}


 /* ����  ���ض���c�⺯��printf��USART1*/ 
int fputc(int ch, FILE *f)
{
/* ��Printf���ݷ������� */
  USART_SendData(USART1, (unsigned char) ch);
  while (!(USART1->SR & USART_FLAG_TXE));
 
  return (ch);
}

void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		
        uint8_t data = USART_ReceiveData(USART1);

        // ���֡������ "\r\n"
        if (data == '\r') {
            // ����'\r'���ȴ�'\n'
        } else if (data == '\n') {
            rx_buf[rx_index] = '\0'; // ����ַ���������
            rx_flag = 1;             // ���ý�����ɱ�־
            rx_index = 0;
        } else {
            // �洢���ݵ�������
            if (rx_index < RX_BUF_SIZE - 1) {
                rx_buf[rx_index++] = data;
            } else {
                // �������������
                rx_index = 0;
            }
        }
    }
}



// UART2��ʼ������
void USART2_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /* ����ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    /* ����GPIO */
    // TX (PA2) �� RTS (PA1) ����Ϊ�����������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // RX (PA3) ����Ϊ��������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* ����UART2���� */
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ����RTS
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);

    /* �����ж� */
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // ���ý����ж�

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // ���ȼ�����USART1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART2, ENABLE);
}

// UART2�жϷ�����
void USART2_IRQHandler(void) {
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        uint8_t data = USART_ReceiveData(USART2);

        // ���֡������ "\r\n"
        if (data == '\r') {
            // ����'\r'
        } else if (data == '\n') {
            rx2_buf[rx2_index] = '\0'; // ����ַ���������
            rx2_flag = 1;              // ���ý�����ɱ�־
            rx2_index = 0;
        } else {
            if (rx2_index < RX2_BUF_SIZE - 1) {
                rx2_buf[rx2_index++] = data;
            } else {
                rx2_index = 0; // �������������
            }
        }
    }
}
void UART_SendByte(unsigned char Send_Dat)
{
    // ��������
    USART_SendData(USART2, Send_Dat);
    // �ȴ�������ɣ�TXE��־��λ��ʾ������ת�Ƶ���λ�Ĵ�����
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}
