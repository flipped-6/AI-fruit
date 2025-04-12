#include "stm32f10x.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "oled.h"
#include "hx711.h"
#include "Key.h"
#include "DP_Print_inc.h"
#include "bitmap.h"
/*****************称重显示******************
oled显示：
品种：
单价：
重量（斤）：
总价（元）：
**********************BEGIN***********************/
int value;
float weight;
int32_t reset;
u8 buff[30];//OLED显示
u8 buf[100];//打印机
uint8_t KeyNum;	
float final_price=0;
float Weights=238.0;  //238g
int32_t Weights_238=8363350;  //238g的ADC值
void select_lines(uint8_t times)
{
	uint8_t i;
	for(i=1;i<=times;i++)
	{
		print_And_Line();  /* 单次换行 */ 
	}
	
}

/*
** 函数： void init_putstr(uint8_t *buf, unsigned char nsel)
** 参数： *buf  字符数据  nsel  对齐方式 0居左 1居中 2居右
   描述： print_show_str()函数中的部分代码进一步封装，
	 功能：	初始化打印机和对齐方式的选择 
*/
void  init_putstr(uint8_t *buf, unsigned char nsel)
{
		/* 初始化打印机 */
		InitializePrint();
	
		/* 0居左、1居中、2居右 */
		Sel_Align_Way(nsel);
		Print_ASCII(buf);
}

/*
** 函数： void print_show_str(void)
** 功能：	居左、居中、加粗等效果打印示例

*/
void print_show_str(void)
{
		uint8_t buf[100]={"打印文本效果展示："};
		InitializePrint(); /* 初始化打印机 */
		Print_ASCII(buf);			 /* 发送数据 */
		select_lines(2);   /* 换行两次 */
		//0 左对齐
		sprintf((char *)buf,"文字居左效果演示abc123：");
		/* 初始化打印机、选择对齐方式、发送数据 */
		init_putstr(buf,0); 
		select_lines(2);
		/*  居中对齐 */
		sprintf((char *)buf,"文字居中效果演示abc123：");
		init_putstr(buf,1);
		select_lines(2);
		/* 右对齐 */
		sprintf((char *)buf,"文字居右效果演示abc123：");
		init_putstr(buf,2);		
		select_lines(2);
		/* 文字加粗 */
		sprintf((char *)buf,"文字下加粗效果演示abc123：");
	  InitializePrint();
		SetBold(1);
		Print_ASCII(buf);	
		select_lines(2);
		/* 下划线 1 */
		sprintf((char *)buf,"文字下划线效果演示abc123：");
		InitializePrint();
		Set_UnderLine(1);
		Print_ASCII(buf);
		select_lines(2);
}	

/*******************定义状态机***********************/
typedef enum {
    STATE_KEY1,  // 允许按下按键1
    STATE_KEY2,  // 允许按下按键2和3
} KeyState;
KeyState currentState = STATE_KEY1;




int main(void)
{ 
	
  SystemInit();//配置系统时钟为72M	
	delay_init(72);
	LED_Init();
	Key_Init();
	LED_On();
	HX711_Init();
	USART1_Config();//串口初始化
	USART2_Config();
	OLED_Init();
	//printf("Start \n");
	delay_ms(1000);
	OLED_homepage();
	
	reset = HX711_GetData();//称重是取相对值
	uint8_t class_id = 0xFF;
	while (1)
		{
		//UART_SendByte('A');
		//print_show_str();
		LED_Toggle();
		//printf("串口正常");
		KeyNum = Key_GetNum();
		switch (currentState){
			case STATE_KEY1:
				if (KeyNum == 1)			//按键1按下开始称重
				{
					
					OLED_Clear();
					OLED_ShowChinese(16,20,0,24,1);
					OLED_ShowChinese(40,20,1,24,1);
					OLED_ShowChinese(64,20,2,24,1);
					OLED_ShowChinese(88,20,3,24,1);//显示：开始结账
					delay_ms(1000);
					OLED_Clear();
					OLED_set();
					
					/****************************打印内容****************/
					InitializePrint(); /* 初始化打印机 */
					sprintf((char *)buf,"欢迎光临任之初的水果小店！");
					init_putstr(buf,1);
					select_lines(4);   /* 换行四次 */
					sprintf((char *)buf,"种类  单价  重量  金额");
					init_putstr(buf,1);		 /* 发送数据 */
					select_lines(4);   /* 换行四次 */
					//printf("种类  单价  重量  总价\r\n");
					final_price=0;
					currentState = STATE_KEY2;
					//print_show_str();
					//printf("key1被按下\r\n");		
					
				}
				break;
			case STATE_KEY2:
				value = HX711_GetData();
				weight=(float)(value-reset)*Weights*0.002/(float)(Weights_238-reset);
				weight=fabs(weight);//取绝对值避免出现负数
				sprintf((char*)buff,"%.2f",weight);
			
				
				OLED_ShowString(96,32,buff,16,1);
				if (rx_flag) {
					rx_flag = 0;	
					class_id = rx_buf[0];// 解析接收到的字节（发送的是单个字节如0x02）					
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
					OLED_ShowChinese(96,24,21,16,1);//显示：已加入购物车
					delay_ms(1000);
					OLED_Clear();
					OLED_set();
					
					if(class_id==0){
						//printf("梨    7.0   %.2f   %.2f\r\n",weight,current_total);
						sprintf((char *)buf,"梨    7.00  %.2f  %.2f",weight,current_total);
						init_putstr(buf,1);			 /* 发送数据 */
						select_lines(2);   /* 换行两次 */
					}else if(class_id==1){
						//printf("苹果  4.5   %.2f   %.2f\r\n",weight,current_total);
						sprintf((char *)buf,"苹果  4.50  %.2f  %.2f",weight,current_total);
						init_putstr(buf,1);			 /* 发送数据 */
						select_lines(2);   /* 换行两次 */
					}else if(class_id==2){
						//printf("西瓜  2.5   %.2f   %.2f\r\n",weight,current_total);
						sprintf((char *)buf,"西瓜  2.50  %.2f  %.2f",weight,current_total);
						init_putstr(buf,1);			 /* 发送数据 */
						select_lines(2);   /* 换行两次 */
					}else if(class_id==3){
						//printf("香蕉  3.3   %.2f    %.2f\r\n",weight,current_total);
						sprintf((char *)buf,"香蕉  3.30  %.2f  %.2f",weight,current_total);
						init_putstr(buf,1);			 /* 发送数据 */
						select_lines(2);   /* 换行两次 */
					}
					final_price=final_price+current_total;
					//printf("key2被按下\r\n");
				}else if(KeyNum == 3){
					OLED_Clear();
					OLED_ShowChinese(32,16,22,16,1);
					OLED_ShowChinese(48,16,23,16,1);
					OLED_ShowChinese(64,16,24,16,1);
					OLED_ShowChinese(80,16,25,16,1);//显示：顾客应付 //第二行
					//printf("key3被按下\r\n");
					
					/****************************打印内容****************/
					select_lines(2);
					sprintf((char *)buf,"顾客应付%.2f元",final_price);
					init_putstr(buf,1);			 /* 发送数据 */
					select_lines(6);   /* 换行两次 */
					
					
					
					sprintf((char*)buff,"%.2f",final_price);
					OLED_ShowString(48,32,buff,16,1);
					currentState = STATE_KEY1;
					//printf("顾客应付%.2f元\r\n",final_price);
				}
				break;
		}
		
		
		
		
		delay_ms(500);
  }
}




