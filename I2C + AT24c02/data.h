#include<REGX52.H>
#include <intrins.h>


//数码管是共阴

//51上接上一个译码器，P22，23，24为输入，当为000时，输出的八个引脚中，有一个
//会变为低电平，
//000 -> LED1 最右边
//001 -> LED2 左边一个
//以此类推
//
//消影
//显示多段时，一个数字后要延时1ms，再将P0清零
//



void show_num(char loca,char number);//精简版本
void location(char number);//确定数字位置
void show(char number);//显示数字
void Delay100us(void);


int num[11]=
{
	0x3f,//0
	0x30,//1
	0x5b,//2
	0x4f,//3
	0x66,//4
	0x6d,//5
	0x7d,//6
  0x07,//7
  0x7f,//8
  0x6f,//9
  0x80,//.
};

void show_num(char loca,char number)
{
	location(loca);
	show(number);
	Delay100us();
	P0 = 0x00;
}


void Delay100us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	i = 47;
	while (--i);
}



void location(char number)
{
	if (number <=8 && number >=1)
		{
			switch (number)
			{
				case 1:P2_2=1;P2_3=1;P2_4=1;break;//左边第一个
				case 2:P2_2=0;P2_3=1;P2_4=1;break;
				case 3:P2_2=1;P2_3=0;P2_4=1;break;
				case 4:P2_2=0;P2_3=0;P2_4=1;break;
				case 5:P2_2=1;P2_3=1;P2_4=0;break;
				case 6:P2_2=0;P2_3=1;P2_4=0;break;
				case 7:P2_2=1;P2_3=0;P2_4=0;break;
				case 8:P2_2=0;P2_3=0;P2_4=0;break;
			}		
		} 
}


void show(char number)
{
	if (number <=10 && number >=0)
		{
			P0 = num[number];
		} 

}