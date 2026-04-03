#include<REGX52.H>
#include <intrins.h>
#include<I2C.h>

//AT24C02的通信地址码，256字节
#define Address 0xA0

//数据地址在0~255，每一地址存储一字节
//读取字节后，若需立即读取，应延时5ms，留取时间于芯片内部操作

void AT_WByte(unsigned char WordAdd,Data);//写字节，数据地址，数据
unsigned char AT_RByte(unsigned char WordAdd);//读字节
void AT_WPage(unsigned char WAddress,unsigned char* Datas);//页写数据,需自备数组
void AT_RPage(unsigned char* Datas,unsigned char Waddress,num);//多字节读取，需自备数组,可指定读取数目，到达末尾后从开头读取
void Delay5ms();
	


//写字节，数据地址，数据
void AT_WByte(unsigned char WordAdd,Data)
{
	I2C_Start();
	I2C_SendByte(Address);//发送机器地址
	I2C_ReceiveAck();//应答
	I2C_SendByte(WordAdd);//发送数据地址
	I2C_ReceiveAck();
	I2C_SendByte(Data);//发送数据
	I2C_ReceiveAck();
	I2C_Stop();

} 
//读字节
unsigned char AT_RByte(unsigned char WordAdd)
{
	unsigned char Byte = 0;
	I2C_Start();
	I2C_SendByte(Address);//发送机器地址
	I2C_ReceiveAck();
	I2C_SendByte(WordAdd);//发送数据地址
	I2C_ReceiveAck();
	I2C_Start();
	I2C_SendByte(Address|0x01);//发送机器地址，此次末尾改为读->1
	I2C_ReceiveAck();
	Byte = I2C_GainByte();//接收数据
	I2C_SendAck(1);//发送应答，1为停止
	I2C_Stop();
	return Byte;
}




//页写数据,需自备数组,可以跨页写数据，大小为8字节
unsigned char AT_num,i = 0;
void AT_WPage(unsigned char WAddress,unsigned char* Datas)
{
	AT_num = WAddress % 8;
	I2C_Start();
	I2C_SendByte(Address);//发送机器地址
	I2C_ReceiveAck();//应答
	I2C_SendByte(WAddress);//发送数据地址
	I2C_ReceiveAck();
	for (i=0;i<(8-AT_num);i++)
		{
			I2C_SendByte(Datas[i]);//发送数据
			I2C_ReceiveAck();
		}
	I2C_Stop();
	if (AT_num!=0)//判断是否需要进入下一页
		{
			Delay5ms();
			I2C_Start();
			I2C_SendByte(Address);//发送机器地址
			I2C_ReceiveAck();//应答
			I2C_SendByte(WAddress+8-AT_num);//发送数据地址
			I2C_ReceiveAck();
			for (i=8-AT_num;i<8;i++)
				{
					I2C_SendByte(Datas[i]);//发送数据
					I2C_ReceiveAck();
				}
			I2C_Stop();
		}
}


//多字节读取，需自备数组,可指定读取数目，到达末尾后从开头读取
unsigned char k =0;
void AT_RPage(unsigned char* Datas,unsigned char Waddress,num)
{

	I2C_Start();
	I2C_SendByte(Address);//发送机器地址
	I2C_ReceiveAck();
	I2C_SendByte(Waddress);//发送数据地址
	I2C_ReceiveAck();
	I2C_Start();
	I2C_SendByte(Address|0x01);//发送机器地址，此次末尾改为读->1
	I2C_ReceiveAck();
	for (;k<num;k++)
		{
			Datas[k] = I2C_GainByte();//接收数据
			I2C_SendAck(0);//发送应答,0为继续
		}
	I2C_SendAck(1);//发送应答，1为停止
	I2C_Stop();
}


void Delay5ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 10;
	j = 183;
	do
	{
		while (--j);
	} while (--i);
}