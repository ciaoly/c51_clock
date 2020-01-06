#ifndef __LCD1602_H__
#define __LCD1602_H__

#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int


sbit rs=P2^4;
sbit rw=P2^5;
sbit en=P2^6;

void init_lcd1602();
void write_lcd1602(uchar com,uchar i);
void display_lcd1602_byte(uchar h,uchar l,uchar val);
void display_lcd1602_text(uchar h,uchar l,uchar *val);
void delay(uint z);








#endif
