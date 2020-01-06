#ifndef __DS1302_H__
#define __DS1302_H__

#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int

sbit ds_rst=P3^7;
sbit ds_clk=P3^5;
sbit ds_io=P3^6;

void write_ds1302(uchar com,uchar val);
uchar read_ds1302(uchar com);
void init_ds1302(uchar nian,uchar yue,uchar ri,uchar shi,uchar fen,uchar miao,uchar xingqi);




#endif

