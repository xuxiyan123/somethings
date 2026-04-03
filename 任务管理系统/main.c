#include <REGX52.H>
#include <INTRINS.H>


#define MAX_TASKS 2 //函数数量
#define MAX_TASK_DEP 32//储存数组的大小


unsigned int time_num = 0;//时间计数器
unsigned char idata task_sp[MAX_TASKS];//函数地址数组的首字节的地址
unsigned char idata task_stack[MAX_TASKS][MAX_TASK_DEP];//储存函数的地址
unsigned char task_id;//表示目前进行的任务

void Delay1000ms(void);		
void task_switch(void);
void task0(void);
void task1(void);
void task_load(unsigned int fn,unsigned char tid);
void start_0();//初始化定时器0

void main()
{
	int w,j,k = 0;
	task_load(task0,0);//加载任务
	task_load(task1,1);
	P0_1=0;//初始化led
	P0_0=0;
	start_0();//初始化定时器
	task_id = 0;
	w = &task0;
	j = task_stack[0][0];
	k = task_stack[0][1];
	SP=task_sp[0];
	
}
//配置定时器，10ms
void start_0()
{
	TMOD = 0x01;//设置定时器模式
	TL0 = 55536 % 256;//高位
	TH0 = 55536 / 256;//低位
	TF0 = 0;//清除中断标志位
	EA = 1;//中断总开关
	ET0 = 1;//定时器0中断开关
	TR0 = 1;//打开定时器

}
// 中断函数
void inter(void) interrupt 1
{
	EA = 0;//关闭中断总开关
	TL0 = 55536 % 256;//高位
	TH0 = 55536 / 256;//低位
	time_num++;
	if (time_num == 101)//清零
		{
			time_num = 1;
		}
	EA = 1;//中断总开关
}
/*
void Delay1000ms()		
{
	unsigned char i, j, k;

	_nop_();
	i = 8;
	j = 154;
	k = 122;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
*/
void task_switch()
{
  task_sp[task_id]=SP;//不知有何作用,删去也并无大碍
	
	task_id=task_id+1;//转换下一函数
	if(task_id==MAX_TASKS)//若为2，则转换为0 也可以task %=2;
	{
	  task_id=0;
	}
	SP=task_sp[task_id];//将栈指针转换至下一函数头地址
}

void task0()
{
  while(1)
	{
		if (time_num == 100)//100个中断为1s
			{
				P0_0=~P0_0;//调转电平，实现亮灭
			}
		
		task_switch();//转换至下一函数
	}
}

void task1()
{
	
  while(1)
	{
		if (time_num % 50 == 0 && time_num != 0)//0.5s转换一次
			{
				P0_1=~P0_1;
			}
		task_switch();
	}
}
	
void task_load(unsigned int fn,unsigned char tid)
{
	task_stack[tid][1] = (unsigned char)(fn >> 8); //高八位的地址
  task_stack[tid][0] = (unsigned char)fn;  //低八位的地址
	task_sp[tid] = (unsigned char)&task_stack[tid][1];//储存函数地址的首地址  sp压栈时要自增1
}



