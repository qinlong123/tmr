#include <string.h>

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long

/*************************************************

*****          函数声明区          ***************

*************************************************/

/************************************************************/
//#define PF1     0x5A	//配方1	扇区
//#define PF2     0x5C	//配方2	扇区
//#define PF3     0x5E	//配方3	扇区
//#define PF4     0x60	//配方4	扇区
//#define PF5     0x62	//配方5	扇区
//#define PF6     0x64	//配方6	扇区
//#define PF7     0x66	//配方7	扇区
//#define PF8     0x68	//配方8	扇区
//#define PF9     0x6A	//配方9	扇区
//#define PF10    0x6C	//配方10扇区
//#define PF11    0x6E	//配方11扇区
//子索引定义
//0x01,0x02 物料1
//0x03,0x04 物料1
//0x05,0x06 物料1
//0x07,0x08 物料1
//0x09,0x0a 物料1
//0x0b,0x0c 物料1
//0x0d,0x0e 物料1
//0x0f,0x10 物料1
//0x11,0x12 物料1
//0x13,0x14 物料1

/*初始化函数*/
void init();		//初始化函数	<00>
void init_CPU();	//CPU初始化		<00.1>
void init_TIMER();	//定时器初始化	<00.2>
void init_AD();		//AD转换初始化	<00.3>
void delayL(uint n);	//延时函数	<00.3.1>
/*初始化函数end*/

/*每秒动作函数*/





uint sys_kind_temp = 1;
uchar  rev_1[7] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uchar  weightL[2] = {0x00,0x00};
uchar  PFID = 0;
uchar  WLID = 0;
uchar  RWSW = 0;
//uchar  BitCounter = 8;
uchar  ok_id = 0;//负号开关
uchar  ok_id2 = 0;//0开关

uint time0_tick = 0; //计数容器,此处可以作为延时使用
uchar tick = 0;	//间隔开关
uchar count[4] = {0,0,0,0};
//bit flag = 1;  //计算开始开关
uint sw_clkchk = 0;
uint sw_one = 0; //执行一次规避开关

uchar int0sw = 0; //寻址与读取数据开关  0为寻址 1为读取数据
//bit tim8 = 0; //寻址标识
//uint counter1 = 32;
//uint counter2 = 0;
uint cvover = 0;
uchar convertsw[] = {252,96,218,242,102,182,190,224,254,246,2};
uchar diswich = 0; //数据容错开关
uchar fu_hao = -0x05;
uchar ZERO_SW = 0;
uchar sys_status = 0;//系统状态：0 初始化状态 1 等待数据接收 2数据监控状态 3按键等待状态（选择配方种类）	4按键等待状态（PRINT）
uchar sys_subjects = 0;//配方种类；从0~19
uchar sys_kinds = 1;//物料种类:从0~30
uint sys_press = 0;//对应物料种类的物料重量
uchar sys_res_status = 0;//数据接收状态 0 未接收到 1 已接收到
uint sys_press_temp = 0;//对应上次入料的设定重量，为了下次快速计算
uchar addorarr = 0;//下料增加设定的符号，0为加 1为减
uint addorarr_count = 0;//增加或者减少的数值
uint recv_buf[5]= {0,0,0,0,0};
uchar dis[4]={0x00,0x00,0x00,0x00};
uchar standby = 0;
//uint led2 = 0;

code unsigned char dis_data[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
//0->0x3F,
//1->0x06,
//2->0x5B,
//3->0x4F
//4->0x66
//5->0x6D
//6->0x7D
//7->0x07
//8->0x7F
//9->0x6F
//A->0x77
//L->0x38
//E->0x79
//R->0x50
//P->0x73
//G->等同于9
//- ->0x40

uint intko = 0;


/*************************************************

*****          变量声明区END          ************

**************************************************/