#include "ds1302.h"

/**
* DS1302 与微处理器进行数据交换时，首先由微处理器向电路发送命令字节，
* 命令字节最高位Write Protect(D7)必须为逻辑1，如果D7=0，则禁止写DS1302，
* 即写保护；D6=0，指定时钟数据，D6=1，指定RAM数据；D5-D1指定输入或输出的特定寄存器；
* 最低位LSB(D0)为逻辑0，指定写操作(输入)， D0=1，指定读操作(输出)。
*/

uchar read_ds1302(uchar com) // com 命令
{
	uchar i, val;
	ds_clk = 0;
	ds_rst = 1;
	ds_clk = 0;
	for (i = 0; i < 8; i++)
	{
		ds_io = com & 0x01;
		ds_clk = 0;
		ds_clk = 1;
		com >>= 1;
	}
	for (i = 0; i < 8; i++)
	{
		ds_clk = 1;
		ds_clk = 0;
		if (ds_io)
			val |= 0x01 << i;
	}
	ds_rst = 0;
	val = (val / 16 * 10) + val % 16;
	return val;
}

void write_ds1302(uchar com, uchar val) // com 命令, val 值
{
	uchar i;
	ds_clk = 0;
	ds_rst = 1;
	ds_clk = 0;
	val = val / 10 * 16 + val % 10; // 转换为 val 同形的十六进制, 其实是 BCD
	for (i = 0; i < 8; i++)
	{
		ds_io = com & 0x01;
		ds_clk = 0;
		ds_clk = 1;
		com >>= 1;
	}
	for (i = 0; i < 8; i++)
	{
		ds_io = val & 0x01;
		ds_clk = 0;
		ds_clk = 1;
		val >>= 1;
	}
	ds_rst = 0;
	ds_clk = 0;
}

/*void init_ds1302()
{
	write_ds1302(0x8e,0);
	write_ds1302(0x84,0);

	write_ds1302(0x8c,0x11);//��
	write_ds1302(0x8a,0x07);//����
	write_ds1302(0x88,0x05);//��
	write_ds1302(0x86,0x08);//��
	write_ds1302(0x84,0x22);//ʱ
	write_ds1302(0x82,0x50);//��
	write_ds1302(0x80,0x50);//��
//	write_ds1302(0x8e,0x80);//�ؼĴ���
}*/

void init_ds1302(uchar nian, uchar yue, uchar ri, uchar shi, uchar fen, uchar miao, uchar xingqi)
{
	write_ds1302(0x8e, 0);
	write_ds1302(0x84, 0);

	write_ds1302(0x8c, nian);   //��
	write_ds1302(0x8a, xingqi); //����
	write_ds1302(0x88, yue);	//��
	write_ds1302(0x86, ri);		//��
	write_ds1302(0x84, shi);	//ʱ
	write_ds1302(0x82, fen);	//��
	write_ds1302(0x80, miao);   //��
	write_ds1302(0x8e, 80);		//�ؼĴ���
}
