#include "hx711.h"
#include "delay.h"



/**
  * @brief  PM25初始化程序
  * @param  无
  * @retval 无
  */


void HX711_Init(void)
{
		/*定义一个HX711_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启相关的GPIO外设时钟*/
		RCC_APB2PeriphClockCmd(HX711_GPIO_CLK , ENABLE);
		/*选择要控制的GPIO引脚*/

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_InitStructure.GPIO_Pin = HX711_SCK_GPIO_PIN;
		GPIO_Init(HX711_SCK_GPIO_PORT, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = HX711_DT_GPIO_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   

		GPIO_Init(HX711_DT_GPIO_PORT, &GPIO_InitStructure);	

	
}
	
unsigned long HX711_GetData(void)
{
		unsigned long Count;
		unsigned char i;
		HX711_SCK_L;
		delay_us(1);
		Count=0;
		while(HX711_DT);
		for (i=0;i<24;i++)//循环读取24位数据
		{
				HX711_SCK_H;//拉高SCK
				delay_us(1);
				Count=Count<<1;//数据左移接收新位
				HX711_SCK_L;//拉低SCK
				delay_us(1);
				if(HX711_DT) Count++;//数据线为高的话数据位置1
		}
		HX711_SCK_H;
		delay_us(1);
		Count=Count^0x800000;//最高位取反，其他位不变
		HX711_SCK_L;
		delay_us(1);
		return(Count);
}

