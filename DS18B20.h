/*********** (C) COPYRIGHT 2010 http://www.wang1jin.com ************

* ������;��wang1jin��������ѧ��Ƭ����Ƶ�̳����׳������

* ��������: wang1jin(QQ:56873678 ,Email:wang1jin@163.com)

* ����汾: V1.0

* ��������: 2010-01-01

* ��������Keil C uVision4

* оƬ��STC89C52,11.0592M����

* ����֧�֣�http://www.wang1jin.com

* ���򿪷��壺WJ-V4.0 51�汾

* ��Ƶ�̳����׿����幺���ַ��http://shop35240330.taobao.com/  

* �ṩԭ��51��Ƭ����PROTEL99SE��Ƶ�̳����أ���ӭ���ǰ����̳���ء�

* ��̳��ַ�� http://www.wang1jin.com/bbs/ 

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
