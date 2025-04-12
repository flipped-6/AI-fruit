#ifndef __HX711_H
#define	__HX711_H
#include "stm32f10x.h"
#include "delay.h"
#include "math.h"

/*****************辰哥单片机设计******************
											STM32
 * 文件			:	HX711电子秤模块h文件                   
 * 版本			: V1.0
 * 日期			: 2024.9.11
 * MCU			:	STM32F103C8T6
 * 接口			:	见代码							
 * BILIBILI	:	辰哥单片机设计
 * CSDN			:	辰哥单片机设计
 * 作者			:	辰哥

**********************BEGIN***********************/

/***************根据自己需求更改****************/
// HX711 GPIO宏定义
#define		HX711_GPIO_CLK									RCC_APB2Periph_GPIOB
#define 	HX711_SCK_GPIO_PORT							GPIOB
#define 	HX711_SCK_GPIO_PIN							GPIO_Pin_11
#define 	HX711_DT_GPIO_PORT							GPIOB
#define 	HX711_DT_GPIO_PIN								GPIO_Pin_10
/*********************END**********************/

#define HX711_SCK_H				GPIO_SetBits(HX711_SCK_GPIO_PORT,HX711_SCK_GPIO_PIN);
#define HX711_SCK_L				GPIO_ResetBits(HX711_SCK_GPIO_PORT,HX711_SCK_GPIO_PIN);

#define HX711_DT					GPIO_ReadInputDataBit(HX711_DT_GPIO_PORT, HX711_DT_GPIO_PIN)



void HX711_Init(void);
unsigned long HX711_GetData(void);

#endif /* __ADC_H */

