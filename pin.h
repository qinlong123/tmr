#include <stc12c56.h>

/************************************/
/*读写区*/
//12864复位信号端
#define RST_HIGH	rst=1
#define RST_LOW		rst=0

//12864数据命令选择端
#define A0_HIGH		a0=1
#define A0_LOW		a0=0

//12864串行模式数据端
#define SCL_HIGH	scl=1
#define SCL_LOW		scl=0

//12864时钟端
#define DI_HIGH		di=1
#define DI_LOW		di=0

//12864芯片选通断
#define CS_HIGH		cs=1
#define CS_LOW		cs=0
/*读写区end*/
/************************************/