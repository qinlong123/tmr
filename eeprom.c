#include "eeprom.H"
#include "stc12c56.h"
#include "INTRINS.H"

#define uint unsigned int
#define uchar unsigned char
#define ulong unsigned long
//警报界限

/*EEPROM处理函数(不可修改)*/
//eeprom------------------------------------------
#define RdCommand 0x01 //定义ISP的操作命令
#define PrgCommand 0x02
#define EraseCommand 0x03 
#define Error 1
#define Ok 0
#define WaitTime 0x03 //定义CPU的等待时间

void mdelay(uint n)
{
    uint x;
    while (n--)
    {
        x = 5000;
        while (x--);
    }
}
//eeprom-------------------------------------------
/*打开 ISP,IAP 功能*/
void ISP_IAP_enable(void)
{
	 EA = 0;       //关中断   
	 ISP_CONTR = ISP_CONTR & 0x18;		//0001,1000 
	 ISP_CONTR = ISP_CONTR | WaitTime;	//写入硬件延时 
	 ISP_CONTR = ISP_CONTR | 0x80;      //ISPEN=1  
}

/*关闭 ISP,IAP 功能*/
void ISP_IAP_disable(void)
{
	 ISP_CONTR = ISP_CONTR & 0x7f; //ISPEN = 0 
	 ISP_TRIG = 0x00;
	 EA   =   1;   //开中断 
}

/*触发ISP*/
void ISPgoon(void)
{
	 ISP_IAP_enable();	//打开 ISP,IAP 功能 
	 ISP_TRIG = 0x46;	//触发ISP_IAP命令字节1 
	 ISP_TRIG = 0xb9;	//触发ISP_IAP命令字节2 
 	 _nop_();
}

/*字节读*/
unsigned char byte_read(uchar high_add,uchar low_add)
{
	 ISP_ADDRH = high_add;	//地址赋值 
	 ISP_ADDRL = low_add;
	 ISP_CMD   = ISP_CMD & 0xf8;		//清除低3位  
	 ISP_CMD   = ISP_CMD | RdCommand;	//写入读命令 
	 ISPgoon();       		//触发执行  
	 ISP_IAP_disable();    	//关闭ISP,IAP功能 
	 return (ISP_DATA);    	//返回读到的数据
}

/*扇区擦除*/
void SectorErase(uchar high_add,uchar low_add)
{
	 ISP_ADDRH = high_add;	//地址赋值
	 ISP_ADDRL = low_add;
	 ISP_CMD = ISP_CMD & 0xf8;   		//清空低3位
	 ISP_CMD = ISP_CMD | EraseCommand; 	//擦除命令3
	 ISPgoon();       					//触发执行
	 _nop_();
	 ISP_IAP_disable();    				//关闭ISP,IAP功能
}

/*字节写*/
void byte_write(uchar str, uchar high_add,uchar low_add)
{
	 ISP_ADDRH = high_add;	//地址赋值
	 ISP_ADDRL = low_add;
	 ISP_CMD  = ISP_CMD & 0xf8;    		//清低3位
	 ISP_CMD  = ISP_CMD | PrgCommand;  	//写命令2
	 ISP_DATA = str;   					//写入数据准备
	 ISPgoon();       					//触发执行
	 _nop_();
	 ISP_IAP_disable();     			//关闭IAP功能
}

/*EEPROM处理函数end(不可修改)*/
/**************************************************/
/***************************************************************/

/****EEPROM相关值处理函数***/


void writein(uchar a[],uchar b1,uchar b)	 //a wei
{ 	 			
	byte_write(a[0],b1,b);
	byte_write(a[1],b1,b+1);
	byte_write(a[2],b1,b+2);
	byte_write(a[3],b1,b+3);
	byte_write(a[4],b1,b+4);
}

void Refresh_all_k(char b,char b1)	 //a wei
{ 	 			
	byte_write(0x00,b1,b);
	byte_write(0x00,b1,b+1);
	byte_write(0x00,b1,b+2);
	byte_write(0x00,b1,b+3);
	byte_write(0x00,b1,b+4);
}
//SectorErase(XQ4,0x00);//写入前要清扇区
/*-----------------------------------------------28------------------------
read_uSvhr_max: 读最大值	<06.1.3>			
输入参数：无
返回值	：无 
最后修改日期：2012-02-17 
修改人	：skyman
作用	：读最大值	
-----------------------------------------------------------------------*/
unsigned char read_uSvhr_max(uchar b1 ,uchar k, uchar count)//读出数组count 下标	 b1星期 地址 k 当天时间段
{
	//uchar u1,u2,u3,u4;
	uchar dk;

	if(k == 1)
	{		
		dk = byte_read(b1,count);
	}
	if(k == 2)
	{
		dk = byte_read(b1,count+5);
	}
	if(k == 3)
	{
		dk = byte_read(b1,count+10);
	}
	if(k == 4)
	{
		dk = byte_read(b1,count+15);
	}
	
	//rekey = u1+(u2<<8)+(u3<<16)+(u4<<24);
	return (dk);				
}

