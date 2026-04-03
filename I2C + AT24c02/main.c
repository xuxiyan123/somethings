#include<REGX52.H>
#include <intrins.h>
#include<I2C.h>
#include<AT24c02.h>
#include<data.h>
void Delay5ms(void);
int main()
{
	unsigned char a,b,c = 0;
	unsigned char qqnums[7] = {1,2,3,4,5,6,7};
	//unsigned char qq[8] = {0};
	AT_WPage(32,&qqnums);
	Delay5ms();
	//AT_RPage(&qq,3,8);
	a = AT_RByte(33);
	b = AT_RByte(40);
	c = AT_RByte(44);
	while(1)
		{
			show_num(1,a);
			show_num(2,b);
			show_num(3,c);
		}


}
