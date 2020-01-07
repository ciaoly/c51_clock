#include <reg52.h>
#include "lcd1602.h"
#include "ds1302.h"

#define de_bcd(x) ((x / 16 * 10) + x % 16)
#define abc(x) (x)
#define display_shi(s, p)                                        \
	if (timing_flat == 24)                                       \
	{                                                            \
		display_lcd1602_byte(1, p, ' ');                         \
		display_lcd1602_byte(1, p + 2, s / 10 + '0');            \
		display_lcd1602_byte(1, p + 3, s % 10 + '0');            \
	}                                                            \
	else                                                         \
	{                                                            \
		if (s > 12)                                              \
		{                                                        \
			display_lcd1602_byte(1, p, 'P');                     \
			display_lcd1602_byte(1, p + 2, (s - 12) / 10 + '0'); \
			display_lcd1602_byte(1, p + 3, (s - 12) % 10 + '0'); \
		}                                                        \
		else                                                     \
		{                                                        \
			display_lcd1602_byte(1, p, 'A');                     \
			display_lcd1602_byte(1, p + 2, s / 10 + '0');        \
			display_lcd1602_byte(1, p + 3, s % 10 + '0');        \
		}                                                        \
	}

#define display_laba(p)         \
	uchar i;                    \
	uchar *z;                   \
	write_lcd1602(0x40, 0);     \
	if (stop_buzz)              \
		z = w2;                 \
	else                        \
		z = w1;                 \
	for (i = 0; i < 8; i++)     \
		write_lcd1602(z[i], 1); \
	write_lcd1602(0xc0 + p, 0); \
	write_lcd1602(0x00, 1);

sbit buzz = P2 ^ 2;
sbit led = P2 ^ 3;
sbit s1 = P3 ^ 0;
sbit s2 = P3 ^ 1;
sbit s3 = P3 ^ 2;
sbit s4 = P3 ^ 3;

char shi = 0, fen = 0, miao = 0, nian = 20, yue = 1, ri = 1, xingqi = 3, ashi = 0, afen = 0;
uchar s1num = 0, s4num;										//s1num 记录按键 1 按下的次数, s4num 记录按键 4 按下的次数
uchar flag = 1, flag1 = 0, stop_buzz = 0, timing_flat = 24; // flag 是设置暂停标志, flag1 是设闹钟标志,

uchar code w1[] = {0x01, 0x03, 0x1D, 0x11, 0x1D, 0x03, 0x01, 0x00};
uchar code w2[] = {0x05, 0x0E, 0x74, 0x4C, 0x54, 0x74, 0x4C, 0x84};
uchar code tab0[] = "20  -  -   day: ";
uchar code tab1[] = "--:--     :  :  ";
uchar code tab2[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
uchar code tab5[] = "OFF";
uchar code tab4[] = "reset clock";

void display();
void keyscan();
void buzzer();
void naozhong();
void zhengdian();
void liangdeng();

void main()
{
	init_lcd1602();
	init_ds1302(nian, yue, ri, shi, fen, miao, xingqi);
	display_lcd1602_text(0, 0, tab0);
	display_lcd1602_text(1, 0, tab1);
	while (1)
	{
		keyscan();
		if (flag)
		{
			naozhong(); // 检查闹钟时间是否已到

			display_lcd1602_byte(0, 2, read_ds1302(0x8d, 0) / 10 + '0');
			display_lcd1602_byte(0, 3, read_ds1302(0x8d, 0) % 10 + '0');

			display_lcd1602_byte(0, 5, read_ds1302(0x89, 0) / 10 + '0');
			display_lcd1602_byte(0, 6, read_ds1302(0x89, 0) % 10 + '0');

			display_lcd1602_byte(0, 8, read_ds1302(0x87, 0) / 10 + '0');
			display_lcd1602_byte(0, 9, read_ds1302(0x87, 0) % 10 + '0');

			display_lcd1602_byte(0, 15, read_ds1302(0x8b, 0) % 10 + '0');

			// 实现12/ 24进制转换
			shi = read_ds1302(0x85, 0);
			display_shi(shi, 6);

			display_lcd1602_byte(1, 11, read_ds1302(0x83, 0) / 10 + '0');
			display_lcd1602_byte(1, 12, read_ds1302(0x83, 0) % 10 + '0');

			display_lcd1602_byte(1, 14, read_ds1302(0x81, 0) / 10 + '0');
			display_lcd1602_byte(1, 15, read_ds1302(0x81, 0) % 10 + '0');
		}
	}
}

void keyscan()
{
	uchar i;
	uchar *z;
	if ((s1 == 0) && (s4num == 0)) // 按下按键 1 且没有激活闹钟设置功能
	{
		delay(5);
		if (s1 == 0)
		{
			if (flag)
			{ // 未进入设置模式
				nian = read_ds1302(0x8d, 0);
				delay(5);
				yue = read_ds1302(0x89, 0);
				delay(5);
				ri = read_ds1302(0x87, 0);
				delay(5);
				xingqi = read_ds1302(0x8b, 0);
				delay(5);
				shi = read_ds1302(0x85, 0);
				delay(5);
				fen = read_ds1302(0x83, 0);
				delay(5);
				miao = read_ds1302(0x81, 0);
			}
			while (!s1)
				; // 等待按钮释放, 进入时间设置模式
			buzzer();
			write_ds1302(0x8e, 0);  // 设写保护
			write_ds1302(0x80, 80); // 停表
			flag = 0;				// 进入设置模式, 设置主循环暂停, 同时暂停刷新
			s1num++;
			switch (s1num)
			{ // 设置光标操作
			case 1:
				write_lcd1602(0x0f, 0);
				write_lcd1602(0x80 + 3, 0);
				break;
			case 2:
				write_lcd1602(0x80 + 6, 0);
				break;
			case 3:
				write_lcd1602(0x80 + 9, 0);
				break;
			case 4:
				write_lcd1602(0x80 + 15, 0);
				break;
			case 5:
				write_lcd1602(0x80 + 0x40 + 9, 0);
				break;
			case 6:
				write_lcd1602(0x80 + 0x40 + 12, 0);
				break;
			case 7:
				write_lcd1602(0x80 + 0x40 + 15, 0);
				break;
			case 8: // 保存状态
				write_lcd1602(0x0c, 0);
				write_ds1302(0x8e, 80);
				init_ds1302(nian, yue, ri, shi, fen, miao, xingqi);
				flag = 1;
				break;
			}
			if (s1num == 8)
				s1num = 0;
		}
	}
	if (s1num != 0) // 当前已经按下按键 1, 说明进入了设置模式
	{
		if (s2 == 0)
			delay(5);
		if (s2 == 0) // 确认已经按下按键 2
		{
			while (!s2)
				;
			buzzer();
			switch (s1num) // 判断当前是修改哪一位
			{
			case 1:
				nian++;
				if (nian == 100)
					nian = 0;
				display_lcd1602_byte(0, 2, tab2[nian / 10]);
				display_lcd1602_byte(0, 3, tab2[nian % 10]);
				write_lcd1602(0x80 + 3, 0);
				break;
			case 2:
				yue++;
				if (yue == 13)
					yue = 1;
				display_lcd1602_byte(0, 5, tab2[yue / 10]);
				display_lcd1602_byte(0, 6, tab2[yue % 10]);
				write_lcd1602(0x80 + 6, 0);
				break;
			case 3:
				ri++;
				if (ri == 32)
					ri = 1;
				display_lcd1602_byte(0, 8, tab2[ri / 10]);
				display_lcd1602_byte(0, 9, tab2[ri % 10]);
				write_lcd1602(0x80 + 9, 0);
				break;
			case 4:
				xingqi++;
				if (xingqi == 8)
					xingqi = 1;
				display_lcd1602_byte(0, 15, tab2[xingqi % 10]);
				write_lcd1602(0x80 + 15, 0);
				break;
			case 5:
				shi++;
				if (shi == 24)
					shi = 0;
				display_shi(shi, 6);
				write_lcd1602(0x80 + 0x40 + 9, 0);
				break;
			case 6:
				fen++;
				if (fen == 60)
					fen = 0;
				display_lcd1602_byte(1, 11, tab2[fen / 10]);
				display_lcd1602_byte(1, 12, tab2[fen % 10]);
				write_lcd1602(0x80 + 0x40 + 12, 0);
				break;
			case 7:
				miao++;
				if (miao == 60)
					miao = 0;
				display_lcd1602_byte(1, 14, tab2[miao / 10]);
				display_lcd1602_byte(1, 15, tab2[miao % 10]);
				write_lcd1602(0x80 + 0x40 + 15, 0);
				break;
			}
		}
	}

	if (s1num != 0) // 设置模式-> 按键3
	{
		if (s3 == 0)
			delay(5);
		if (s3 == 0)
		{
			while (!s3)
				;
			buzzer();
			switch (s1num)
			{
			case 1:
				nian--;
				if (nian == -1)
					nian = 99;
				display_lcd1602_byte(0, 2, tab2[nian / 10]);
				display_lcd1602_byte(0, 3, tab2[nian % 10]);
				write_lcd1602(0x80 + 3, 0);
				break;
			case 2:
				yue--;
				if (yue == 0)
					yue = 12;
				display_lcd1602_byte(0, 5, tab2[yue / 10]);
				display_lcd1602_byte(0, 6, tab2[yue % 10]);
				write_lcd1602(0x80 + 6, 0);
				break;
			case 3:
				ri--;
				if (ri == 0)
					ri = 31;
				display_lcd1602_byte(0, 8, tab2[ri / 10]);
				display_lcd1602_byte(0, 9, tab2[ri % 10]);
				write_lcd1602(0x80 + 9, 0);
				break;
			case 4:
				xingqi--;
				if (xingqi == 0)
					xingqi = 7;
				display_lcd1602_byte(0, 15, tab2[xingqi % 10]);
				write_lcd1602(0x80 + 15, 0);
				break;
			case 5:
				shi--;
				if (shi == -1)
					shi = 23;
				display_shi(shi, 6);
				write_lcd1602(0x80 + 0x40 + 9, 0);
				break;
			case 6:
				fen--;
				if (fen == -1)
					fen = 59;
				display_lcd1602_byte(1, 11, tab2[fen / 10]);
				display_lcd1602_byte(1, 12, tab2[fen % 10]);
				write_lcd1602(0x80 + 0x40 + 12, 0);
				break;
			case 7:
				miao--;
				if (miao == -1)
					miao = 59;
				display_lcd1602_byte(1, 14, tab2[miao / 10]);
				display_lcd1602_byte(1, 15, tab2[miao % 10]);
				write_lcd1602(0x80 + 0x40 + 15, 0);
				break;
			}
		}
	}
	// 闹钟设置
	if ((s4 == 0) && (s1num == 0)) // 按下闹钟设置键, 且未进入时间设置模式
	{
		delay(5);
		if (s4 == 0) // 确认按键 4 已按下
			delay(500);
		if (s4 != 0) // 按键 4 已经松手
		{
			while (!s4)
				;
			buzzer();
			flag = 0;
			s4num++;
			if (s4num == 4)
				s4num = 1;
			switch (s4num)
			{
			case 1:
				write_lcd1602(0x01, 0);
				display_lcd1602_text(0, 2, tab4);
				display_lcd1602_text(1, 7, tab5);
				write_lcd1602(0x0c, 0);
				flag1 = 0;
				break;
			case 2:
				write_lcd1602(0x0f, 0);
				display_lcd1602_byte(1, 5, tab2[ashi / 10]);
				display_lcd1602_byte(1, 6, tab2[ashi % 10]);
				display_lcd1602_byte(1, 7, ':');
				display_lcd1602_byte(1, 8, tab2[afen / 10]);
				display_lcd1602_byte(1, 9, tab2[afen % 10]);
				write_lcd1602(0x80 + 0x40 + 6, 0);
				flag1 = 1;
				break;
			case 3:
				display_lcd1602_byte(1, 5, tab2[ashi / 10]);
				display_lcd1602_byte(1, 6, tab2[ashi % 10]);
				display_lcd1602_byte(1, 7, ':');
				display_lcd1602_byte(1, 8, tab2[afen / 10]);
				display_lcd1602_byte(1, 9, tab2[afen % 10]);
				write_lcd1602(0x80 + 0x40 + 9, 0);
				flag1 = 1;
				break;
			}
		}
		if (s4 == 0) // 长按 (500ms) 按键 4 触发, 退出闹钟设置模式
		{
			while (!s4)
				;
			s4num = 0;
			write_lcd1602(0x01, 0);
			write_lcd1602(0x0c, 0);
			display_lcd1602_text(0, 0, tab0);
			display_lcd1602_text(1, 0, tab1);
			flag = 1;
			if (flag1)
				display();
		}
	}
	if (s4num != 0) // 进入闹钟设置模式
	{
		if (s2 == 0)
			delay(5);
		if (s2 == 0)
		{
			while (!s2)
				;
			buzzer();
			switch (s4num)
			{
			case 2:
				ashi++;
				if (ashi == 24)
					ashi = 0;
				display_shi(ashi, 3);
				write_lcd1602(0x80 + 0x40 + 6, 0);
				break;
			case 3:
				afen++;
				if (afen == 60)
					afen = 0;
				display_lcd1602_byte(1, 8, tab2[afen / 10]);
				display_lcd1602_byte(1, 9, tab2[afen % 10]);
				write_lcd1602(0x80 + 0x40 + 9, 0);
				break;
			}
		}
		if (s3 == 0)
			delay(5);
		if (s3 == 0)
		{
			while (!s3)
				;
			buzzer();
			switch (s4num)
			{
			case 2:
				ashi--;
				if (ashi == -1)
					ashi = 23;
				display_shi(ashi, 3);
				write_lcd1602(0x80 + 0x40 + 6, 0);
				break;
			case 3:
				afen--;
				if (afen == -1)
					afen = 59;
				display_lcd1602_byte(1, 8, tab2[afen / 10]);
				display_lcd1602_byte(1, 9, tab2[afen % 10]);
				write_lcd1602(0x80 + 0x40 + 9, 0);
				break;
			}
		}
	}
	else
	{
		if (s1num != 0)
			return;
		if (s2 == 0)
			delay(5);
		if (s2 == 0)
		{
			while (!s2)
				;
			buzzer();
			if (timing_flat == 12)
				timing_flat = 24;
			else
				timing_flat = 12;
		}
		if (s3 == 0)
			delay(5);
		if (s3 == 0)
		{
			while (!s3)
				;
			stop_buzz = !stop_buzz;
			write_lcd1602(0x40, 0);
			if (stop_buzz)
				z = w2;
			else
				z = w1;
			for (i = 0; i < 8; i++)
				write_lcd1602(z[i], 1);
			write_lcd1602(0xc0 + 5, 0);
			write_lcd1602(0x00, 1);
		}
	}
}

void buzzer()
{
	buzz = 0;
	delay(50);
	buzz = 1;
}

void liangdeng()
{
	led = 0;
	delay(50);
	led = 1;
}

void naozhong()
{
	// char diff_fen = 0; // 一个入栈指令
	delay(5);
	shi = read_ds1302(0x85, 0);
	delay(5);
	fen = read_ds1302(0x83, 0);
	delay(5);
	miao = read_ds1302(0x81, 0);
	zhengdian();
	if (flag1)
	{
		// display();
		if (ashi == shi && afen == fen)
		{
			// diff_fen = read_ds1302(0x83, 0) - 1 - afen;
			// if (abc(diff_fen) <= 1) // [afen, afen + 2]
			if (!stop_buzz)
				buzzer();
			// else
			// {
			// 	stop_buzz = 0;
			// }
		}
	}
}

void zhengdian()
{
	if (shi + fen + miao == 0)
	{
		buzzer();
		buzzer();
		buzzer();
	}
}

void display() // 显示闹钟开启标志
{
	display_laba(5);
	display_lcd1602_byte(1, 0, ashi / 10 + '0');
	display_lcd1602_byte(1, 1, ashi % 10 + '0');
	display_lcd1602_byte(1, 3, afen / 10 + '0');
	display_lcd1602_byte(1, 4, afen % 10 + '0');
}