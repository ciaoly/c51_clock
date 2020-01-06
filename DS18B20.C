/*********** (C) COPYRIGHT 2010 http://www.wang1jin.com ************

* 程序用途：wang1jin带您从零学单片机视频教程配套程序代码

* 程序作者: wang1jin(QQ:56873678 ,Email:wang1jin@163.com)

* 程序版本: V1.0

* 编制日期: 2010-01-01

* 编译器：Keil C uVision4

* 芯片：STC89C52,11.0592M晶振

* 技术支持：http://www.wang1jin.com

* 程序开发板：WJ-V4.0 51版本

* 视频教程配套开发板购买地址：http://shop35240330.taobao.com/  

* 提供原创51单片机、PROTEL99SE视频教程下载，欢迎大家前往论坛下载。

* 论坛地址： http://www.wang1jin.com/bbs/ 

******************** http://www.wang1jin.com **********************/ 
#include"DS18B20.h"

uchar DS18B20_ID[8];

bit	RESET_DS18B20(void)
{
	DS18B20_DQ = 1;
	DS18B20_DQ = 0;
	Delay_Us(247);
	DS18B20_DQ = 1;
	Delay_Us(27);
	if(DS18B20_DQ == 0)
		{
			while(DS18B20_DQ == 0);
			return 0;
		}
	else
		{
			return 1;
		}
}


void	Write_DS18B20_DATA(uchar Value)
{
	uchar i = 0;
	for(i = 0; i < 8; i ++)
		{
			DS18B20_DQ = 1;
			DS18B20_DQ = 0;
			Delay_Us(5);			
			DS18B20_DQ = Value & 0x01;
			Delay_Us(20);
			DS18B20_DQ = 1;
			Value >>= 1;
		}
}

uchar Read_DS18B20_DATA(void)
{
	uchar i = 0;
	uchar Value = 0;
	for(i = 0; i < 8; i ++)
		{
			DS18B20_DQ = 1;
			DS18B20_DQ = 0;
			Delay_Us(1);
			DS18B20_DQ = 1;
			Delay_Us(1);
			if(DS18B20_DQ)
				{
					Value |= 0x01 << i;
				}
			Delay_Us(17);
			DS18B20_DQ = 1;
			nop();
		}
	return Value;
}

void Read_DS18B20_ID(void)
{
	uchar i = 0;
	RESET_DS18B20();					//复位DS18B20
	Write_DS18B20_DATA(0x33);		//读ROM命令。读ID号
	for(i = 0; i < 8; i++)
		{
			DS18B20_ID[i] = Read_DS18B20_DATA();
		}
}

uchar Read_DS18B20_TEMP(void)
{
	uchar Temp_L = 0;
	uchar Temp_H = 0;
	uchar Temp = 0;
	bit ZF = 0;
	RESET_DS18B20();
	Write_DS18B20_DATA(0xCC);
	Write_DS18B20_DATA(0x44);
	RESET_DS18B20();
	Write_DS18B20_DATA(0xCC);
	Write_DS18B20_DATA(0xBE);
	Temp_L = Read_DS18B20_DATA();
	Temp_H = Read_DS18B20_DATA();
	Temp_L >>= 4;
	Temp_H <<= 4;
	Temp = Temp_H | Temp_L;
	if(Temp & 0x80)
		{
			ZF = 0;
		}
	else
		{
			ZF = 1;
		}
	return Temp;
}


void Delay_Us(uchar i)
{
	while(-- i);
}



/**************************

MOV		R7,#0X01;		1
LCALL	DELAY;			2
DELAY:	DJNZ R7,DELAY;	2
RET						2

X = 5+I*2;

**************************/
