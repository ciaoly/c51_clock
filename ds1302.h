#ifndef __DS1302_H__
#define __DS1302_H__

#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int

sbit ds_rst = P1 ^ 2;
sbit ds_clk = P1 ^ 3;
sbit ds_io = P1 ^ 4;

void write_ds1302(uchar com, uchar val);
uchar read_ds1302(uchar com, char bcd_flag);
void init_ds1302(uchar nian, uchar yue, uchar ri, uchar shi, uchar fen, uchar miao, uchar xingqi);

#endif
