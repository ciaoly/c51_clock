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
#ifndef	_DS18B20_H_
#define	_DS18B20_H_

#include<reg52.H>
#include<intrins.h>

#define	uchar 	unsigned char
#define	uint		unsigned int

#define	nop()	_nop_()

extern uchar DS18B20_ID[8];

sbit	DS18B20_DQ = P1^5;

void Delay_Us(uchar i);
bit	RESET_DS18B20(void);
void Read_DS18B20_ID(void);
uchar Read_DS18B20_DATA(void);
void	Write_DS18B20_DATA(uchar Value);
uchar Read_DS18B20_TEMP(void);

#endif
