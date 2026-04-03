#ifndef __I2C_h
#define __I2C_h

#include<REGX52.H>
#include <intrins.h>

//具体说明见I2C.md文件

//通信线定义
#define I2C_SCL P2_1 
#define I2C_SDA P2_0

void I2C_Start(void);//开启通信
void I2C_Stop(void);//关闭通信
void I2C_SendByte(unsigned char Data);//发送一个字节
char I2C_GainByte(void);//获取一个字节
void I2C_SendAck(unsigned char Ack);//发送应答
char I2C_ReceiveAck(void);//接收应答
//由于51速度较慢，因此无需延时，但指令间隔需5us左右

//开启通信
void I2C_Start()
{
	I2C_SDA = 1;
	I2C_SCL = 1;
	I2C_SDA = 0;
	I2C_SCL = 0;
}
//关闭通信
void I2C_Stop()
{
	I2C_SDA = 0;
	I2C_SCL = 1;
	I2C_SDA = 1;
}
//发送一个字节
void I2C_SendByte(unsigned char Data)
{
	unsigned char i = 0;
	I2C_SCL = 0;
	for(;i<8;i++)
		{
			I2C_SDA = Data & (0x80 >> i);
			I2C_SCL = 1;
			I2C_SCL = 0;
		}

}

//获取一个字节
char I2C_GainByte(void)
{
	unsigned char i,Data = 0;
	I2C_SDA = 1;
	for (i=0;i<8;i++)
		{
			I2C_SCL = 1;
			if (I2C_SDA == 1){Data |= (0x80 >> i);}
			I2C_SCL = 0;
		}
	return Data;
}

//发送应答
void I2C_SendAck(char Ack)
{
	I2C_SDA = Ack;
	I2C_SCL = 1;
	I2C_SCL = 0;
}

//接收应答
char I2C_ReceiveAck()
{
	char rece = 0;
	I2C_SDA = 1;
	I2C_SCL = 1;
	rece = I2C_SDA;
	I2C_SCL = 0;
	return rece;
}





#endif