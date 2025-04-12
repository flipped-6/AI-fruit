#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Key.h"
/**
  * 函    数：按键初始化
  * 参    数：无
  * 返 回 值：无
  */
void Key_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//开启GPIOA的时钟
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//将PA5,6,7引脚初始化为上拉输入
}

/**
  * 函    数：按键获取键码
  * 参    数：无
  * 返 回 值：按下按键的键码值，范围：0~2，返回0代表没有按键按下
  * 注意事项：此函数是阻塞式操作，当按键按住不放时，函数会卡住，直到按键松手
  */
uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;		//定义变量，默认键码值为0
	
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0)			//读PA5输入寄存器的状态，如果为0，则代表按键1按下
	{
		delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0);	//等待按键松手
		delay_ms(20);											//延时消抖
		KeyNum = 1;												//置键码为1
	}
	
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0)			//读PB11输入寄存器的状态，如果为0，则代表按键2按下
	{
		delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0);	//等待按键松手
		delay_ms(20);											//延时消抖
		KeyNum = 2;												//置键码为2
	}
	
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 0)			//读PB11输入寄存器的状态，如果为0，则代表按键2按下
	{
		delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 0);	//等待按键松手
		delay_ms(20);											//延时消抖
		KeyNum = 3;												//置键码为2
	}
	return KeyNum;			//返回键码值，如果没有按键按下，所有if都不成立，则键码为默认值0
}
