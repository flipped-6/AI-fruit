
#include "DP_Print_inc.h"
#include "led.h"
#include "usart.h"
static unsigned char gucCheckPassWord=0;

/* 初始化打印机，清除打印缓冲 a*/
void InitializePrint(void)
{
	/* 初始化打印机，清除打印缓冲 */
	UART_SendByte(0x1B);
	UART_SendByte(0x40);
	gucCheckPassWord = 1;
}

/* 每调用一次，打印位置向后移动一定字的符单位  a*/
void jump_lattice(void)
{
	if(!gucCheckPassWord)
		return;
	UART_SendByte(0x09);
}
 
/* 打印缓冲区数据并向前推进一行  a*/
void print_And_Line(void)
{

	/* 发送一个换行符 */
	UART_SendByte(0x0A);
}


/*
** 函数： Print_ASCII(unsigned char *buf)
** 参数： buf  要发送的字符串数据
   功能： 把要打印的字符串放入缓冲区 
*/
void Print_ASCII(unsigned char *buf)
{
	while(*buf != '\0')
    {
    	UART_SendByte(*buf++);
    }   
}


/* 设置字符右侧的间距为interval X 0.125mm a*/
void Set_Right_Interval(unsigned char interval)
{
	if(!gucCheckPassWord)
		return;
	UART_SendByte(0x1B);
	UART_SendByte(0x20);
	UART_SendByte(interval);
}


/* 选择打印模式 a*/
void Set_Print_Mode(unsigned char optbit)
{
	if(!gucCheckPassWord)
		return;
	
	UART_SendByte(0x1B);
	UART_SendByte(0x21);
	UART_SendByte(optbit);
}

/* 设定从一行的开始到将要打印字符的位置距离为(Lpos+Hpos*256)*0.125mm  a*/
void Set_Print_Position(unsigned char Lpos,unsigned char Hpos)
{
	if(!gucCheckPassWord)
		return;
	UART_SendByte(0x1B);
	UART_SendByte(0x24);
	UART_SendByte(Lpos);
	UART_SendByte(Hpos);
}


/*
设置左边间距 a
*/
void Set_Left_Interval(unsigned char nL, unsigned char nH)
{
	if(!gucCheckPassWord)
		return;


	UART_SendByte(0x1D);
	UART_SendByte(0x4C);
	UART_SendByte(nL);
	UART_SendByte(nH);
}

/* 选择/取消用户自定义字符集  a*/
void Sel_Custom_Character(unsigned char SelOpt)
{
	if(!gucCheckPassWord)
		return;
	UART_SendByte(0x1B);
	UART_SendByte(0x25);
	UART_SendByte(SelOpt);
}
/* 用户自定义字符集 a*/
void UserDefineCharacter(unsigned char yByte,unsigned char xDot, unsigned char DefChar,unsigned char *pData)
{
	unsigned short int len;
	if(!gucCheckPassWord)
		return;
	
	UART_SendByte(0x1B);
	UART_SendByte(0x26);
	UART_SendByte(yByte);
	UART_SendByte(DefChar);
	UART_SendByte(DefChar);
	UART_SendByte(xDot);
	len = yByte*xDot;
	while(len)
	{
		UART_SendByte(*pData++);
		len--;
	}
}

/* 选择位图模式 a*/
void Sel_Bitmap_Mode(unsigned char mode,unsigned char DatLenLowByte,unsigned char DatLenHightByte,unsigned char *pDotData)
{
	unsigned short int i;
	
	if(!gucCheckPassWord)
		return;
	
	if((mode!=0) && (mode!=1) && (mode!=32) && (mode!=33))
		return;
	
	if(DatLenHightByte > 3)
		return;
	
	UART_SendByte(0x1B);
	UART_SendByte(0x2A);
	UART_SendByte(mode);
	UART_SendByte(DatLenLowByte);
	UART_SendByte(DatLenHightByte);
	
	i=DatLenLowByte + DatLenHightByte*256;
	
	while(i)
	{
		UART_SendByte(*pDotData++);
		i--;
	}
}

/* 设置下划线有无及规格  a*/
void Set_UnderLine(unsigned char Opt)
{
	if(!gucCheckPassWord)
		return;
	
	if(Opt > 2)
	{
		Opt=2;
	}
	UART_SendByte(0x1B);
	UART_SendByte(0x2D);
	UART_SendByte(Opt);
}

/* 设置缺省行间距3.75mm  a*/
void SetDefaultLineInterval(void)
{
	if(!gucCheckPassWord)
		return;
	UART_SendByte(0x1B);
	UART_SendByte(0x32);
}

/* 取消自定义字符  a*/
void Del_UserDefineCharacter(unsigned char SelCharacter)
{
	if(!gucCheckPassWord)
		return;
	if((SelCharacter < 32) || (SelCharacter > 126))
		return;
	UART_SendByte(0x1B);
	UART_SendByte(0x3F);
	UART_SendByte(SelCharacter);
}


/* Position 水平定位位置（设置横向跳格位置） a */
void SetHorizPosition(unsigned char Position)
{
	if(!gucCheckPassWord)
		return;
	UART_SendByte(0x1B);
	UART_SendByte(0x44);
	UART_SendByte(Position);
	UART_SendByte(Position);
	UART_SendByte(0x00);
}

/* 选择/取消加粗模式 a*/
void SetBold(unsigned char opt)
{
	if(!gucCheckPassWord)
		return;
	if(opt>0x01)
	{
		opt=0x01;
	}
	UART_SendByte(0x1B);
	UART_SendByte(0x45);
	UART_SendByte(opt);
}
/* 打印并进纸nstep*0.125mm  a*/
void PrintGoPage(unsigned char nstep)
{
	if(!gucCheckPassWord)
		return;
	
	UART_SendByte(0x1B);
	UART_SendByte(0x4A);
	UART_SendByte(nstep);
}
/* 选择语言字符 a*/  
void SelCountryCharacter(unsigned char nsel)
{
	if(!gucCheckPassWord)
		return;
		
	UART_SendByte(0x1B);
	UART_SendByte(0x74);
	UART_SendByte(nsel);
}

/* 选择/取消顺时针旋转 90°  a*/
void Set_Rotate(unsigned char nsel)
{
	if(!gucCheckPassWord)
		return;
	
	if(nsel>1)
	{
		nsel=1;
	}
	
	UART_SendByte(0x1B);
	UART_SendByte(0x56);
	UART_SendByte(nsel);
}


/* 选择对齐方式 a*/
void Sel_Align_Way(unsigned char nsel)
{
	if(!gucCheckPassWord)
		return;
	
	if(nsel > 2)
	{
		nsel = 2;
	}
	
	UART_SendByte(0x1B);
	UART_SendByte(0x61);
	UART_SendByte(nsel);
}

/* 选择/取消倒置打印模式  a*/
void SetReversal(unsigned char nsel)
{
	if(!gucCheckPassWord)
		return;
	
	if(nsel > 1)
	{
		nsel = 1;
	}
	
	UART_SendByte(0x1B);
	UART_SendByte(0x7B);
	UART_SendByte(nsel);
	
}

/* 设置/选择字符大小 a*/
void SetCharacterSize(unsigned char width,unsigned char hight)
{
	if(!gucCheckPassWord)
		return;
	
	if(width > 7)
	{
		width = 7;
	}
	if(hight > 7)
	{
		hight = 7;
	}
	
	hight = hight&0x0f;
	width = (width&0x0f)<<4;
	width = width | hight;
	
	UART_SendByte(0x1D);
	UART_SendByte(0x21);
	UART_SendByte(width);
	
}

/* 定义下载位图 */
void DownLoadBitmap(unsigned char xDot,unsigned char yDot,unsigned char *pDat)
{
	unsigned short int len;
	
	if(!gucCheckPassWord)
		return;
	
	if(yDot > 48)
		return;
	
	UART_SendByte(0x1D);
	UART_SendByte(0x2A);
	UART_SendByte(xDot*8);
	UART_SendByte(yDot*8);
	len = xDot*yDot/8;
	
	while(len)
	{
		UART_SendByte(*pDat++);
		len--;
	}
	
}
/* 打印下载位图 */
void PrintDownLoadBitmap(unsigned char mode)
{
	if(!gucCheckPassWord)
		return;
	
	if(mode > 3)
	{
		mode = 3;
	}	
	UART_SendByte(0x1D);
	UART_SendByte(0x2F);
	UART_SendByte(mode);
}

/* 黑白反显打印  a*/
void Set_Inverse(unsigned char opt)
{
	if(!gucCheckPassWord)
		return;
	
	if(opt>1)
	{
		opt=1;
	}
	UART_SendByte(0x1D);
	UART_SendByte(0x42);
	UART_SendByte(opt);
}


/* 设置左边距 a*/
void Set_LeftSpaceNum(unsigned char nL,unsigned char nH)
{
	if(!gucCheckPassWord)
		return;
	
	UART_SendByte(0x1D);
	UART_SendByte(0x4C);
	UART_SendByte(nL);
	UART_SendByte(nH);
}


/* 选择 HRI 字符的打印位置 a*/
void Set_HRIPosition(unsigned char opt)
{
	if(!gucCheckPassWord)
		return;
	
	if(opt>3)
	{
		opt=3;
	}
	
	UART_SendByte(0x1D);
	UART_SendByte(0x48);
	UART_SendByte(opt);
}

/* 选择条码高度 a*/
void Set_BarCodeHight(unsigned char verticalDotNum)
{
	if(!gucCheckPassWord)
		return;
	
	UART_SendByte(0x1D);
	UART_SendByte(0x68);
	
	UART_SendByte(verticalDotNum);
}


/*  打印条码 a*/
void PrintBarCode(unsigned char CodeType,unsigned char *pData,unsigned char pDataLen)
{
	if(!gucCheckPassWord)
		return;
	
	if((CodeType == 65) || (CodeType == 66))
	{
		if((pDataLen < 11) || (pDataLen > 12))
			return;
	}
	else if(CodeType == 67)
	{
		if((pDataLen < 12) || (pDataLen > 13))
			return;
	}
	else if(CodeType == 68)
	{
		if((pDataLen < 7) || (pDataLen > 8))
			return;
	}
	
	UART_SendByte(0x1D);
	UART_SendByte(0x6B);
	UART_SendByte(CodeType);
	
	if(CodeType == 0x47)
	{	
		UART_SendByte(pDataLen+2);
		UART_SendByte(0x41);
	}
	else
	
	{
		UART_SendByte(pDataLen);
	}

	while(pDataLen)
	{
		UART_SendByte(*pData++);
		pDataLen--;
	}
	
	if(CodeType == 0x47)
	{
		UART_SendByte(0x41);
	}
	
	UART_SendByte(0x00);
}
/* 设置条形码左边间距 a*/
void Set_BarCodeLeftSpace(unsigned char nL, unsigned char nH)
{
	if(!gucCheckPassWord)
		return;


	UART_SendByte(0x1D);
	UART_SendByte(0x4C);
	UART_SendByte(nL);
	UART_SendByte(nH);
}

/* 条形码的宽度  a*/
void Set_BarCodeWidth(unsigned char widthsel)
{
	if(!gucCheckPassWord)
		return;
	
	if(widthsel < 2)
	{
		widthsel = 2;
	}
	else if(widthsel > 6)
	{
		widthsel = 6;
	}
	
	UART_SendByte(0x1D);
	UART_SendByte(0x77);
	UART_SendByte(widthsel);
}
/* 设置汉字字符打印模式     a*/
void SetChinesemode(unsigned char opt)
{
	
	if(!gucCheckPassWord)
		return;
	
	UART_SendByte(0x1C);
	UART_SendByte(0x21);
	UART_SendByte(opt);
}

/* 选择汉字模式 a*/
void SelChineseChar(void)
{
	if(!gucCheckPassWord)
		return;
	
	UART_SendByte(0x1C);
	UART_SendByte(0x26);
}
/*取消打印机汉字打印模式 a*/
void DisChineseChar(void)
{
	if(!gucCheckPassWord)
		return;
	
	UART_SendByte(0x1C);
	UART_SendByte(0x2E);
}
/* 选择中文编码格式 a */
void Set_ChineseCode(unsigned char selopt)
{
	if(!gucCheckPassWord)
		return;
	
	if(selopt > 3)
		selopt = 3;
	
	UART_SendByte(0x1B);
	UART_SendByte(0x23);
	UART_SendByte(0x23);
	UART_SendByte(0x43);
	UART_SendByte(0x44);
	UART_SendByte(0x54);
	UART_SendByte(0x59);
	UART_SendByte(selopt);
}



 /*功能：打印测试页结束时进行切纸 a*/
void TestPrintPage(void)
{
		
	UART_SendByte(0x1B);
	UART_SendByte(0x23);
	UART_SendByte(0x23);
	UART_SendByte(0x53);
	UART_SendByte(0x45);
	UART_SendByte(0x4C);
	UART_SendByte(0x46);

	UART_SendByte(0x1D);
	UART_SendByte(0x53);

}

/*		打印光栅位图 a */
void PrintGratinmap(unsigned char mode,unsigned int xDot,unsigned int yDot,unsigned char *pData)
{
	unsigned char tmp;
	unsigned int len;
	
	if(!gucCheckPassWord)
		return;
	
	if(mode > 3)
	{
		mode = 3;
	}
	UART_SendByte(0x1D);
	UART_SendByte(0x76);
	UART_SendByte(0x30);
	UART_SendByte(mode);
	xDot = xDot / 8;
	tmp = (unsigned char)(xDot & 0x00ff);
	UART_SendByte(tmp);
	tmp = (unsigned char)((xDot>>8) & 0x00ff);
	UART_SendByte(tmp);
	tmp = (unsigned char)(yDot & 0x00ff);
	UART_SendByte(tmp);
	tmp = (unsigned char)((yDot>>8) & 0x00ff);
	UART_SendByte(tmp);
	len = xDot * yDot;
	while(len)
	{
		UART_SendByte(*pData++);
		
		len--;
	}
}

/* 设置QR Code（二维码）单元大小 a*/
void Set_QRcodeMode(unsigned char mode)
{
	if(!gucCheckPassWord)
		return;
	
	if(mode > 16)
		mode = 16;
	
	UART_SendByte(0x1D);
	UART_SendByte(0x28);
	UART_SendByte(0x6B);
	UART_SendByte(0x03);
	UART_SendByte(0x00);
	UART_SendByte(0x31);
	UART_SendByte(0x43);
	UART_SendByte(mode);
}


/* 设置错误纠错等级  a*/
void Set_QRCodeAdjuLevel(unsigned char level)
{
	if(!gucCheckPassWord)
		return;
	
	if(level < 0x30)
	{
		level = 0x30;
	}
	else if(level > 0x33)
	{
		level = 0x33;
	}
	
	UART_SendByte(0x1D);
	UART_SendByte(0x28);
	UART_SendByte(0x6B);
	UART_SendByte(0x03);
	UART_SendByte(0x00);
	UART_SendByte(0x31);
	UART_SendByte(0x45);
	UART_SendByte(level);
}


/* QRCode传输数据至编码缓存 a*/
void Set_QRCodeBuffer(unsigned int Len,unsigned char *pData)
{
	unsigned char tmp;
	if(!gucCheckPassWord)
		return;
	
	if(Len < 3)
	{
		return;
	}
	
	Len += 3;
	
	UART_SendByte(0x1D);
	UART_SendByte(0x28);
	UART_SendByte(0x6B);
	tmp = (unsigned char)(Len & 0x00ff);
	UART_SendByte(tmp);
	tmp = (unsigned char)((Len >> 8)&0x00ff);
	UART_SendByte(tmp);
	UART_SendByte(0x31);
	UART_SendByte(0x50);
	UART_SendByte(0x30);
	Len -= 3;
	while(Len)
	{
		UART_SendByte(*pData++);
		Len--;
	}
	UART_SendByte(0x00);
}
/* QR Code 打印编码缓存的二维条码   a*/
void PrintQRCode(void)
{
	if(!gucCheckPassWord)
		return;
	UART_SendByte(0x1D);
	UART_SendByte(0x28);
	UART_SendByte(0x6B);
	UART_SendByte(0x03);
	UART_SendByte(0x00);
	UART_SendByte(0x31);
	UART_SendByte(0x51);
	UART_SendByte(0x30);
	UART_SendByte(0x00);
}


/*
** 函数：void Barcode_sw(unsigned char mode)    
** 参数：mode 0禁止，1启用
   功能：条形码打印使能 
*/
void Barcode_sw(unsigned char mode)
{
	UART_SendByte(0x1D);
	UART_SendByte(0x45);
	UART_SendByte(0x43);
	UART_SendByte(mode);
}


