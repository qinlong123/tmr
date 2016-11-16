#include "lcd12864.h"
#include "pin.h"
#include "pindef.h"
#include "write_chinese.h"

//**************delay************************************
void delay_nms(unsigned int n)
{
	unsigned int i,j;
	for(i=n;i>0;i--)
	{
		for(j=125;j>0;j--)
		{
		}
	}
}

//***************send_data********************************
void data_send(unsigned char dat)
{	
	unsigned char s,temp;
	int i;
	SCL_LOW;
	s=dat;
	for(i=8;i>0;i--)
   	{ 
     	nop();
     	nop();
     	temp=s&0x80;
     	if(temp)
        	{DI_HIGH;}
     	else
        	{DI_LOW;}
	 	SCL_LOW;
     	s=s<<1;
	 	SCL_HIGH;
   	}
}

//***************write command*****************************
void wr_com(unsigned char com)
{
	A0_LOW;
	CS_LOW;
	data_send(com);
	CS_HIGH;
}
//***************write data********************************
void wr_data(unsigned char dat)
{
A0_HIGH;
CS_LOW;
data_send(dat);
CS_HIGH;
}

//清屏函数
void LCD_clear()
{

   unsigned char seg;
   unsigned char page;
   for(page=0xb0;page<0xb8;page++)
     {
         wr_com(page);
         wr_com(0x10);
         wr_com(0x00);
         for(seg=0;seg<128;seg++)
            { wr_data(0x00);
              }
       }
}

//**********************************************************
//设置坐标函数
void LCD_set_XY(unsigned char X, unsigned char Y)
  {
    unsigned char k;
	k=X;
    wr_com(0xb0+Y);		// colum
	   k=k>>4;
       wr_com(0x10+k);          	// row
	   wr_com(X&0x0f);
  }



//初始化LCD
void LCD_init(void)
  {
	//初始化动作
	delay_nms(100);
	RST_LOW;
	delay_nms(100);
	RST_HIGH;
	delay_nms(100);
	//初始化动作
	
	//**********lcd inintial************
	wr_com(0xa0); //ADC normal
	wr_com(0xc8); //com normal
	wr_com(0xa3); //set partial display duty 1/65
	wr_com(0x2c); //vc on
	delay_nms(100);
	wr_com(0x2e); //vr on
	delay_nms(100);
	wr_com(0x2f); //internal booster,follower,divided on
	wr_com(0x24); //set rb/ra=5.29
	wr_com(0x81); //set reference voltage select
	wr_com(0x10);
	delay_nms(100);
	wr_com(0xaf);
	//wr_com(0x7f);
	//wr_com(0x7d);
	wr_com(0x40); //set start line 00
	//************************************
	
	//display_lattice(0xff,0xff);
	//delay_nms(1000);
	//clearled();
	LCD_clear();
  }
void LCD_write1(unsigned char X, unsigned char Y,unsigned char* c,unsigned char with)
{
	unsigned char n;
    
    LCD_set_XY(X,Y);                             //设置初始位置
    

  	for (n=0; n<with*8; n++)              //写一个汉字
  	{ 
  	    if (n==with)                      //写汉字的下半部分
  	    {
  	         LCD_set_XY(X,Y+1);
        }
		else if (n==with*2)
		{
			LCD_set_XY(X,Y+2);
		}
		else if (n==with*3)
		{
			LCD_set_XY(X,Y+3);
		}
		else if (n==with*4)
		{
			LCD_set_XY(X,Y+4);
		}
		else if (n==with*5)
		{
			LCD_set_XY(X,Y+5);
		}
		else if (n==with*6)
		{
			LCD_set_XY(X,Y+6);
		}
		else if (n==with*7)
		{
			LCD_set_XY(X,Y+7);
		}
  	    wr_data(c[n]);
  	}
} 

void LCD_write2(unsigned char X, unsigned char Y,unsigned char* c,unsigned char with)
{
	unsigned char n;
    
    LCD_set_XY(X,Y);                             //设置初始位置 
    

  	for (n=0; n<with*2; n++)              //写一个汉字
  	{ 
  	    if (n==with)                      //写汉字的下半部分
  	    {
  	         LCD_set_XY(X,Y+1);
        }
  	    wr_data(c[n]);
  	}
}
void LCD_writek(unsigned char X, unsigned char Y,unsigned char* c,unsigned char with)
{
	unsigned char n;
    
    LCD_set_XY(X,Y);                             //设置初始位置 
    

  	for (n=0; n<with; n++)              //写一个汉字	  with为行宽 列为 1列 8个点阵高
  	{ 
  	    if (n==with)                      //写汉字的下半部分
  	    {
  	         LCD_set_XY(X,Y+1);
        }
  	    wr_data(c[n]);
  	}
}     

void LCD_write3(unsigned char X, unsigned char Y,unsigned char* c,unsigned char with)
{
	unsigned char n;
	LCD_set_XY(X,Y);
	for(n=0;n<89;n++)
	{
		wr_data(c[n]);
	}
}

void LCD_digit_M(unsigned char X, unsigned char Y, unsigned char d)
  {
    unsigned char n;

	d-=48;
    
    LCD_set_XY(X,Y);                             //设置初始位置
	for(n=0;n<12;n++)
	{
		if(n==6)
		{
			LCD_set_XY(X,Y+1);
		}
    	wr_data(digit_M[d][n]);
	}
  }
