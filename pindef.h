#include	<stc12c56.h>

//定时器0初始值
#define T0_H	0xcf
#define T0_L	0x2c
//EEPROM地址
#define PERIOD  		20	//每秒间隔，单位：1/定时器单次时间 秒
/*************************/
/*12864引脚定义*/

sbit di		= P2^4;			//串行模式 时钟端				   7
sbit scl 	= P2^2;			//SI  串行模式数据端			   8
sbit cs 	= P2^1;			//芯片选通断，低电平有效		   12
sbit rst 	= P2^0;			//复位信号，低电平有效		   13
sbit a0 	= P2^3;			//数据命令选择  H 数据  L 命令   14
sbit pin_bg = P2^6;
sbit PS_DATA = P1^0;
sbit PS_CLK = P1^2;

//按键管脚定义
sbit PRINT_KEY = P1^5;
sbit ADD_KEY = P1^4;
sbit ARR_KEY = P1^3;
sbit ZERO_KEY = P1^6;
sbit TOTAL_KEY = P1^7;
sbit SW_LED = P3^4;
sbit SW_KEY = P2^5;
sbit SW_LEDP = P3^5; //显示屏电源开关

//CLOCK管脚定义
sbit CHK_CLK = P2^7;
sbit DOT_LED = P3^7;

/*12864引脚定义end*/
/*************************/