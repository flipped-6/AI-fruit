#ifndef		_DP_PRINT_INC_H
#define		_DP_PRINT_INC_H

/*
** 串口发送1字节的函数 
** 需由客户在根据自己具体的单片机编写
** 函数名及参数需由这里一样
*/
extern void UART_SendByte(unsigned char Send_Dat);

/*
** 串口接收1字节的函数
** 需由客户在根据自己具体的单片机编写
*/
//extern unsigned char UART_RecByte(void);


/*
** 函数：void jump_lattice(void)
** 参数：无
** 功能：每调用一次，打印位置向后移动一定字的符单位
*/
void jump_lattice(void);



/*
** 函数：void print_And_Line(void)
** 参数：无
** 功能：打印缓冲区数据并向前推进一行
*/
void print_And_Line(void);


/*
** 函数：void Print_ASCII(unsigned char *buf)
** 参数：*buf 指定要打印的字符串
** 功能：把要打印的字符串放入缓冲区
** 示例：打印ASCII abcdefg Print_ASCII("abcdefg");
*/
void Print_ASCII(unsigned char *buf);

/*
** 函数：void Set_Right_Interval(unsigned char interval)
** 参数：interval 右侧字符间距 每单位0.125mm
** 功能：设置字符右侧的间距为interval X 0.125mm
*/
void Set_Right_Interval(unsigned char interval);


/*
** 函数：void Set_Print_Mode(unsigned char optbit)
** 参数：optbit  根据下面的宏定义给输入参数赋值，以选择不同的功能模式
** 功能：选择打印模式
** 示例：如要选择字型B 
					unsigned char optbit;
					EnableASCII9X17(optbit);
					Set_Print_Mode(optbit);
*/
#define		EnableASCII9X17(optbit)			optbit=(optbit|0x01)			/* 选择字型B 9*17 */
#define		EnableASCII12X24(optbit)		optbit=(optbit&0xfe)			/* 选择字形A 12*24 */
#define		EnableInverse(optbit)				optbit=(optbit|0x02)			/* 使能打印反白模式 */
#define		DisableInverse(optbit)			optbit=(optbit&0xfd)			/* 取消打印反白模式 */	
#define		EnableInversion(optbit)			optbit=(optbit|0x04)			/* 设置上下倒置模式 */
#define		DisableInversion(optbit)		optbit=(optbit&0xfb)			/* 取消上下倒置模式 */
#define		EnableBold(optbit)					optbit=(optbit|0x08)			/* 设置粗体模式 */
#define		DisableBold(optbit)					optbit=(optbit&0x07)			/* 取消粗体模式 */
#define		EnableDoubleHight(optbit)		optbit=(optbit|0x10)			/* 设置倍高模式 */
#define		DisableDoubleHight(optbit)	optbit=(optbit&0xef)			/* 取消倍高模式 */
#define		EnableDoubleWide(optbit)		optbit=(optbit|0x20)			/* 设置倍宽模式 */
#define		DisableDoubleWide(optbit)		optbit=(optbit&0xdf)			/* 取消倍宽模式 */
#define		EnableUnderLine(optbit)			optbit=(optbit|0x40)			/* 使能下划线 */
#define		DisableUnderLine(optbit)		optbit=(optbit&0xbf)			/* 取消下划线 */
void Set_Print_Mode(unsigned char optbit);


/*
** 函数：void Set_Print_Position(unsigned char Lpos,unsigned char Hpos)
** 参数：Lpos打印位置数值的低8位；Hpos打印位置数值的高8位
** 功能：设定从一行的开始到将要打印字符的位置距离为(Lpos+Hpos*256)*0.125mm
*/
void Set_Print_Position(unsigned char Lpos,unsigned char Hpos);


/*
** 函数：void Set_Left_Interval(unsigned char nL, unsigned char nH)
** 参数：用 nL 和 nH 设置左边距，左边距设置为[( nL + nH×256)×横向移动单位]英
寸.
** 功能：设置左边间距
*/
void Set_Left_Interval(unsigned char nL, unsigned char nH);



/*
** 函数：void Sel_Custom_Character(unsigned char SelOpt)
** 参数：SelOpt  0x00 表示取消用户自定义字符集
								 0x01 表示选择用户自定义字符集
** 功能：选择/取消用户自定义字符集
** 示例：选择自定义字符集 
					Sel_Custom_Character(EnableCustomCharacter);
*/
#define		EnableCustomCharacter		(0x01)
#define		DisableCustomCharacter	(0x00)
void Sel_Custom_Character(unsigned char SelOpt);



/*
** 函数：void UserDefineCharacter(unsigned char yByte,unsigned char xDot, unsigned char DefChar,unsigned char *pData)
** 参数：yByte 垂直方向字节数
				 xDot	 水平方向点数
				 DefChar 自定义字符
				 *pData 自定义字符点阵数据
*/
void UserDefineCharacter(unsigned char yByte,unsigned char xDot, unsigned char DefChar,unsigned char *pData);


/*
** 函数：void Sel_Bitmap_Mode(unsigned char mode,unsigned char DatLenLowByte,unsigned char DatLenHightByte,unsigned char *pDotData)
** 参数：mode 0 8-点 单密度
							1 8-点 双密度
							32 24-点 单密度
							33 24-点 双密度
				 DatLenLowByte 点阵数据水平长度的低8位
				 DatLenHightByte 点阵数据水平长度的高8位
				 pDotData 指向点阵数据的指针
*/
void Sel_Bitmap_Mode(unsigned char mode,unsigned char DatLenLowByte,unsigned char DatLenHightByte,unsigned char *pDotData);



/*
** 函数：void Set_UnderLine(unsigned char Opt)
** 参数：Opt 0 解除下划线模式
						 1 设定1点粗下划线
						 2 设定2点粗下划线
*/
void Set_UnderLine(unsigned char Opt);



/*
** 函数：void SetDefaultLineInterval(void)
** 功能：设置缺省行间距3.75mm
*/
void SetDefaultLineInterval(void);


/*
** 函数：void Del_UserDefineCharacter(unsigned char SelCharacter)
** 参数：SelCharacter  32=<SelCharacter<=126
** 功能：取消自定义字符
*/
void Del_UserDefineCharacter(unsigned char SelCharacter);


/*
** 函数：void InitializePrint(void)
** 功能：初始化打印机，清除打印缓冲
*/
void InitializePrint(void);

/*
** 函数：void SetHorizPosition(unsigned char Position)
** 参数：Position 水平定位位置
*/
void SetHorizPosition(unsigned char Position);



/*
** 函数：void SetBold(unsigned char opt)
** 参数：opt  ==0 解除粗体打印模式
				      >=1 设定粗体打印模式
*/
void SetBold(unsigned char opt);


/*
** 函数：void PrintGoPage(unsigned char nstep)
** 参数：nstep 进纸参数 nstep*0.125mm
** 功能：打印并进纸nstep*0.125mm
*/
void PrintGoPage(unsigned char nstep);


/*
** 函数：void SelCountryCharacter(unsigned char nsel)
** 参数：nsel  0<=nsel<=15
					0:美国	1:法国	2:德国	3:英国	4:丹麦I		5:瑞典	6:意大利
					7:西班牙I		8:日本	9:挪威	10:丹麦II		11:西班牙II		12:拉丁美洲
					13:韩国		14:斯洛文尼亚		15:中国
*/
void SelCountryCharacter(unsigned char nsel);


/*
** 函数：void Set_Rotate(unsigned char nsel)
** 参数：nsel  0 取消顺时钟90度旋转模式
							 1 设置顺时钟90度旋转模式
*/
void Set_Rotate(unsigned char nsel);





/*
** 函数：void Sel_Align_Way(unsigned char nsel)
** 参数：nsel  0<=nsel<=2   0 左对齐		1 居中		2 右对齐
*/
void Sel_Align_Way(unsigned char nsel);



/*
** 函数：void SetReversal(unsigned char nsel)
** 参数：nsel 0 关闭颠倒打印模式		1 打开颠倒打印模式
*/
void SetReversal(unsigned char nsel);


/*
** 函数：void SetCharacterSize(unsigned char width,unsigned char hight)
** 参数： 0<=width<=7 宽度放大1~8倍  0<=hight<=7 高度放大1~8倍
** 功能：设置字符大小
*/
void SetCharacterSize(unsigned char width,unsigned char hight);


/*
** 函数：void DownLoadBitmap(unsigned char xDot,unsigned char yDot,unsigned char *pDat)
** 参数：xDot 水平方向点数 yDot 垂直方向点数 *pDat 位图点阵数据指针 xDot yDot最好为8的倍数
*/
void DownLoadBitmap(unsigned char xDot,unsigned char yDot,unsigned char *pDat);


/*
** 函数：void PrintDownLoadBitmap(unsigned char mode)
** 参数：mode 打印模式 0 普通  1 倍宽  2 倍高  3 4倍大小
** 功能：打印由DownLoadBitmap函数定义的位图数据
*/
void PrintDownLoadBitmap(unsigned char mode);

/*
** 函数：void Set_Inverse(unsigned char opt)
** 参数：opt 0 反白模式关闭
						 1 反白模式打开
*/
void Set_Inverse(unsigned char opt);


/*
** 函数：void Set_LeftSpaceNum(unsigned char nL,unsigned char nH)
** 参数：设置左边空白量为(nL+nH*256)*0.125mm
** 功能：
*/
void Set_LeftSpaceNum(unsigned char nL,unsigned char nH);

/*
** 函数：void Set_HRIPosition(unsigned char opt)
** 参数：opt 0 不打印HRI		1	在条码上方		2	在条码下方		3 在条码的上方及下方
** 功能：打印条形码时选择HRI字符的打印位置
*/

void Set_HRIPosition(unsigned char opt);

/*
** 函数：void Set_BarCodeHight(unsigned char verticalDotNum)
** 参数：verticalDotNum 条码的高度，垂直方向的点数 缺省值162
** 功能：设置条形码高度
*/
void Set_BarCodeHight(unsigned char verticalDotNum);

/*
** 函数：void Set_BarCodeLeftSpace(unsigned char nL, unsigned char nH)
** 参数：用 nL 和 nH 设置左边距，左边距设置为[( nL + nH×256)×横向移动单位]英
寸.
** 功能：设置条形码左边间距
*/
void Set_BarCodeLeftSpace(unsigned char nL, unsigned char nH);

/*
** 函数：void Set_BarCodeWidth(unsigned char widthsel)
** 参数：widthsel (2<=widthsel<=6)  2->0.25mm	3->0.375mm 4->0.56mm 5->0.625mm 6->0.75mm
** 功能：设置条码宽度
*/
void Set_BarCodeWidth(unsigned char widthsel);


/*
** 函数：void PrintBarCode(unsigned char CodeType,unsigned char *pData,unsigned char pDataLen)
** 参数：CodeType								pDataLen					数据类型
				 65 UPC-A								11<=k<=12				  48<=d<=57
				 66 UPC-E								11<=k<=12					48<=d<=57
				 67 JAN13(EAN13)				12<=k<=13					48<=d<=57
				 68 JAN8(EAN8)					7<=k<=8						48<=d<=57
				 69 CODE39							1<=k							48<=d<=57,65<=d<=90,32,36,37,43,45,46,47
				 70 ITF									1<=k							48<=d<=57
				 71 CODABAR							1<=k							48<=d<=57,65<=d<=90,36,43,45,46,47,58
				 72 CODE93							1<=k<=255					0<=d<=127
				 73 CODE128							2<=k<=255					0<=d<=127
** 功能：打印条形码
*/
void PrintBarCode(unsigned char CodeType,unsigned char *pData,unsigned char pDataLen);

/*
** 函数：void SetChinesemode(unsigned char opt)
** 参数：opt 按下面的宏定义操作
** 功能：设置汉字字符打印模式
** 示例：使能倍宽、使能倍高
					unsigned char optbit;
					EnableChinaDoubleWidth(optbit);
					EnableChinaDoubleHight(optbit);
					SetChinesemode(optbit);
*/
#define	EnableChinaDoubleWidth(opt)		opt=opt|0x04			/* 使能倍宽 */
#define DisableChinaDoubleWidth(opt)	opt=opt&0xfb			/* 取消倍宽 */
#define	EnableChinaDoubleHight(opt)		opt=opt|0x08			/* 使能倍高 */
#define	DisableChinaDoubleHight(opt)	opt=opt&0xf7			/* 取消倍高 */
#define	EnableChinaUnderLine(opt)			opt=opt|0x80			/* 使能下划线*/
#define	DisableChinaUnderLine(opt)		opt=opt&0x7f			/* 取消下划线 */
void SetChinesemode(unsigned char opt);

/*
** 函数：void SelChineseChar(void)
** 功能：选择打印机为汉字打印模式
*/
void SelChineseChar(void);


/*
** 函数：void DisChineseChar(void)
** 功能：取消打印机汉字打印模式
*/
void DisChineseChar(void);

/*
** 函数：void Set_ChineseCode(unsigned char selopt)
** 参数：selopt  1 UNICODE编码  2 UTF-8编码  3 CODEPAGE 编码
** 功能：选择中文编码格式
*/
void Set_ChineseCode(unsigned char selopt);

/*
** 函数：void TestPrintPage(void)
** 功能：打印自测页
*/
void TestPrintPage(void);


/*
** 函数：void PrintGratinmap(unsigned char mode,unsigned int xDot,unsigned int yDot,unsigned char *pData)
** 参数：mode 位图模式  0-->普通  1-->倍宽  2-->倍高   3-->四倍大小
				 xDot 水平方向点数
				 yDot 垂直方向点数
				 *pData 位图数据
** 功能：打印光栅位图
*/

void PrintGratinmap(unsigned char mode,unsigned int xDot,unsigned int yDot,unsigned char *pData);


/*
** 函数：void Set_QRcodeMode(unsigned char mode)
** 参数：mode 设置QR码的模块类型 [mode*mode]点
*/

void Set_QRcodeMode(unsigned char mode);



/*
** 函数：void Set_QRCodeAdjuLevel(unsigned char level)
** 参数：level  level>=0x30&&level<=0x33
** 功能：设置QR码的错误校正水平误差 
*/
void Set_QRCodeAdjuLevel(unsigned char level);


/*
**	函数：void Set_QRCodeBuffer(unsigned int Len,unsigned char *pData)
**	函数：Len 添加QR码缓冲长度  *pData 添加QR码缓冲数据
**	功能：存储QR码的数据到QR码缓冲区
*/
void Set_QRCodeBuffer(unsigned int Len,unsigned char *pData);


/*
**	函数：void PrintQRCode(void)
**	功能：打印QRCode码
*/
void PrintQRCode(void);


/*
** 函数：void Barcode_sw(unsigned char mode)
** 参数：mode 0禁止，1启用
	 功能：条形码打印使能
*/
void Barcode_sw(unsigned char mode);



#endif

