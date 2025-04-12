#include "stm32f10x.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "oled.h"
#include "hx711.h"
#include "Key.h"
#include "DP_Print_inc.h"
#include "bitmap.h"
/*****************������ʾ******************
oled��ʾ��
Ʒ�֣�
���ۣ�
���������
�ܼۣ�Ԫ����
**********************BEGIN***********************/
int value;
float weight;
int32_t reset;
u8 buff[30];//OLED��ʾ
u8 buf[100];//��ӡ��
uint8_t KeyNum;	
float final_price=0;
float Weights=238.0;  //238g
int32_t Weights_238=8363350;  //238g��ADCֵ
void select_lines(uint8_t times)
{
	uint8_t i;
	for(i=1;i<=times;i++)
	{
		print_And_Line();  /* ���λ��� */ 
	}
	
}

/*
** ������ void init_putstr(uint8_t *buf, unsigned char nsel)
** ������ *buf  �ַ�����  nsel  ���뷽ʽ 0���� 1���� 2����
   ������ print_show_str()�����еĲ��ִ����һ����װ��
	 ���ܣ�	��ʼ����ӡ���Ͷ��뷽ʽ��ѡ�� 
*/
void  init_putstr(uint8_t *buf, unsigned char nsel)
{
		/* ��ʼ����ӡ�� */
		InitializePrint();
	
		/* 0����1���С�2���� */
		Sel_Align_Way(nsel);
		Print_ASCII(buf);
}

/*
** ������ void print_show_str(void)
** ���ܣ�	���󡢾��С��Ӵֵ�Ч����ӡʾ��

*/
void print_show_str(void)
{
		uint8_t buf[100]={"��ӡ�ı�Ч��չʾ��"};
		InitializePrint(); /* ��ʼ����ӡ�� */
		Print_ASCII(buf);			 /* �������� */
		select_lines(2);   /* �������� */
		//0 �����
		sprintf((char *)buf,"���־���Ч����ʾabc123��");
		/* ��ʼ����ӡ����ѡ����뷽ʽ���������� */
		init_putstr(buf,0); 
		select_lines(2);
		/*  ���ж��� */
		sprintf((char *)buf,"���־���Ч����ʾabc123��");
		init_putstr(buf,1);
		select_lines(2);
		/* �Ҷ��� */
		sprintf((char *)buf,"���־���Ч����ʾabc123��");
		init_putstr(buf,2);		
		select_lines(2);
		/* ���ּӴ� */
		sprintf((char *)buf,"�����¼Ӵ�Ч����ʾabc123��");
	  InitializePrint();
		SetBold(1);
		Print_ASCII(buf);	
		select_lines(2);
		/* �»��� 1 */
		sprintf((char *)buf,"�����»���Ч����ʾabc123��");
		InitializePrint();
		Set_UnderLine(1);
		Print_ASCII(buf);
		select_lines(2);
}	

/*******************����״̬��***********************/
typedef enum {
    STATE_KEY1,  // �����°���1
    STATE_KEY2,  // �����°���2��3
} KeyState;
KeyState currentState = STATE_KEY1;




int main(void)
{ 
	
  SystemInit();//����ϵͳʱ��Ϊ72M	
	delay_init(72);
	LED_Init();
	Key_Init();
	LED_On();
	HX711_Init();
	USART1_Config();//���ڳ�ʼ��
	USART2_Config();
	OLED_Init();
	//printf("Start \n");
	delay_ms(1000);
	OLED_homepage();
	
	reset = HX711_GetData();//������ȡ���ֵ
	uint8_t class_id = 0xFF;
	while (1)
		{
		//UART_SendByte('A');
		//print_show_str();
		LED_Toggle();
		//printf("��������");
		KeyNum = Key_GetNum();
		switch (currentState){
			case STATE_KEY1:
				if (KeyNum == 1)			//����1���¿�ʼ����
				{
					
					OLED_Clear();
					OLED_ShowChinese(16,20,0,24,1);
					OLED_ShowChinese(40,20,1,24,1);
					OLED_ShowChinese(64,20,2,24,1);
					OLED_ShowChinese(88,20,3,24,1);//��ʾ����ʼ����
					delay_ms(1000);
					OLED_Clear();
					OLED_set();
					
					/****************************��ӡ����****************/
					InitializePrint(); /* ��ʼ����ӡ�� */
					sprintf((char *)buf,"��ӭ������֮����ˮ��С�꣡");
					init_putstr(buf,1);
					select_lines(4);   /* �����Ĵ� */
					sprintf((char *)buf,"����  ����  ����  ���");
					init_putstr(buf,1);		 /* �������� */
					select_lines(4);   /* �����Ĵ� */
					//printf("����  ����  ����  �ܼ�\r\n");
					final_price=0;
					currentState = STATE_KEY2;
					//print_show_str();
					//printf("key1������\r\n");		
					
				}
				break;
			case STATE_KEY2:
				value = HX711_GetData();
				weight=(float)(value-reset)*Weights*0.002/(float)(Weights_238-reset);
				weight=fabs(weight);//ȡ����ֵ������ָ���
				sprintf((char*)buff,"%.2f",weight);
			
				
				OLED_ShowString(96,32,buff,16,1);
				if (rx_flag) {
					rx_flag = 0;	
					class_id = rx_buf[0];// �������յ����ֽڣ����͵��ǵ����ֽ���0x02��					
				}else{
					class_id=0xFF;
				}
				OLED_DisplayFruitInfo(class_id);
				if(KeyNum == 2){
					OLED_Clear();
					OLED_ShowChinese(16,24,16,16,1);
					OLED_ShowChinese(32,24,17,16,1);
					OLED_ShowChinese(48,24,18,16,1);
					OLED_ShowChinese(64,24,19,16,1);
					OLED_ShowChinese(80,24,20,16,1);
					OLED_ShowChinese(96,24,21,16,1);//��ʾ���Ѽ��빺�ﳵ
					delay_ms(1000);
					OLED_Clear();
					OLED_set();
					
					if(class_id==0){
						//printf("��    7.0   %.2f   %.2f\r\n",weight,current_total);
						sprintf((char *)buf,"��    7.00  %.2f  %.2f",weight,current_total);
						init_putstr(buf,1);			 /* �������� */
						select_lines(2);   /* �������� */
					}else if(class_id==1){
						//printf("ƻ��  4.5   %.2f   %.2f\r\n",weight,current_total);
						sprintf((char *)buf,"ƻ��  4.50  %.2f  %.2f",weight,current_total);
						init_putstr(buf,1);			 /* �������� */
						select_lines(2);   /* �������� */
					}else if(class_id==2){
						//printf("����  2.5   %.2f   %.2f\r\n",weight,current_total);
						sprintf((char *)buf,"����  2.50  %.2f  %.2f",weight,current_total);
						init_putstr(buf,1);			 /* �������� */
						select_lines(2);   /* �������� */
					}else if(class_id==3){
						//printf("�㽶  3.3   %.2f    %.2f\r\n",weight,current_total);
						sprintf((char *)buf,"�㽶  3.30  %.2f  %.2f",weight,current_total);
						init_putstr(buf,1);			 /* �������� */
						select_lines(2);   /* �������� */
					}
					final_price=final_price+current_total;
					//printf("key2������\r\n");
				}else if(KeyNum == 3){
					OLED_Clear();
					OLED_ShowChinese(32,16,22,16,1);
					OLED_ShowChinese(48,16,23,16,1);
					OLED_ShowChinese(64,16,24,16,1);
					OLED_ShowChinese(80,16,25,16,1);//��ʾ���˿�Ӧ�� //�ڶ���
					//printf("key3������\r\n");
					
					/****************************��ӡ����****************/
					select_lines(2);
					sprintf((char *)buf,"�˿�Ӧ��%.2fԪ",final_price);
					init_putstr(buf,1);			 /* �������� */
					select_lines(6);   /* �������� */
					
					
					
					sprintf((char*)buff,"%.2f",final_price);
					OLED_ShowString(48,32,buff,16,1);
					currentState = STATE_KEY1;
					//printf("�˿�Ӧ��%.2fԪ\r\n",final_price);
				}
				break;
		}
		
		
		
		
		delay_ms(500);
  }
}




