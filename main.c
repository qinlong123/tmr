#include "main.h"
#include "LCD12864.h"
#include "stc12c56.h"
#include "pindef.h"
#include "bmp.h"
#include "eeprom.h"

uchar key_zero = 0;
uchar sd_kind = 0;
uchar PF[2];
//内存定义
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

//save
//#define PF1     0x4A	//配方1	扇区
//#define PF2     0x4C	//配方2	扇区
//#define PF3     0x4E	//配方3	扇区
//#define PF4     0x40	//配方4	扇区
//#define PF5     0x42	//配方5	扇区
//#define PF6     0x44	//配方6	扇区
//#define PF7     0x46	//配方7	扇区
//#define PF8     0x48	//配方8	扇区
//#define PF9     0x4A	//配方9	扇区
//#define PF10    0x4C	//配方10扇区
//#define PF11    0x4E	//配方11扇区

//PFID-> WLSL(物料数量)
//HBIT 0x46

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

/************************************************************/
/****主函数****/
//644
//处理显示的两种方法，1、通过识别CLOCK的频率结束位进行切换 2、通过发送LED控制芯片的复位信号。
////////前4位
//8->254
//7->224
//6->190
//5->182
//4->102
//3->242
//2->218
//1->096
//0->252
//- -> 002
//n -> 246
//d -> 102
//p -> 238
//a -> 206

//////////上1位
//0 -> 000
//1 -> 182
void beep1()
{
 uchar i;
  SW_KEY=0;
  for(i=0;i<4;i++)
  {delayL(40000);}
  SW_KEY=1;
  delayL(30000);
}
void beep3()
{
uchar i;
  //P25=0;
  for(i=0;i<4;i++)
  {delayL(40000);}
  SW_KEY=1;
  delayL(30000);
  SW_KEY=0;
  for(i=0;i<4;i++)
  {delayL(40000);}
  SW_KEY=1;
  delayL(30000);
  SW_KEY=0;
  for(i=0;i<4;i++)
  {delayL(40000);}
  SW_KEY=1;
  delayL(30000);
}
//发送模块初始化//////////////////////////////////////
void init_uart(void)
{
 SCON=0x50;
 PCON=0x00;
 ES=1;
 TMOD=0x20;
 TL1=TH1 = 0xF8;
 //TH1=0xFB;
 TR1=1;
 EA=1;
 //IE=0; //屏蔽中断

 }
//发送方法////////////单字节发送方法有补偿（测试用）/////////////////////////
void send(unsigned char text)
 {
  ES=0;
  SBUF=text+0x30;
  while(!TI); 
  TI=0;
  ES=1;
 }
//发送方法///////////单字节/////发布后使用///////////////////////////////////
void send_usual(unsigned char text)//正常发送
 {
  ES=0;
  SBUF=text;
  while(!TI); 
  TI=0;
  ES=1;
 }
//解析收到的数据编程INT值
void recv_data()
{
sys_press = byte_read(0x58+(PFID*2),(WLID*2))*256+byte_read(0x58+(PFID*2),(WLID*2)+1);
// sys_kinds = (recv_buf[6]-0x30)+(recv_buf[5]-0x30)*10;

   if(sys_press > sys_press_temp)//如果下料重量大于上次设定重量则加料值为增加
    {
      addorarr = 0;
      addorarr_count = sys_press - sys_press_temp;
      sys_press_temp = sys_press;
    }
   if(sys_press < sys_press_temp)//如果下料重量大于上次设定重量则加料值为增加
    {
   addorarr = 1;
   addorarr_count = sys_press_temp - sys_press;
   sys_press_temp = sys_press;
    }
//if(sys_press == sys_press_temp)
//{addorarr = 2;}

//    sys_res_status = 1;//已经确认接收到数据
    delayL(20000);
    delayL(20000);
    delayL(20000);
    delayL(20000);

	}
//接收方法//////////采用中断////////////////////////////////////////////////

//整体发送模块（测试用）///////////////////////////////////////////////
void send_data(dis_V)
{
	uchar temp,i,sy,j;
	for(j=0;j<15;j++)
	{delayL(20000);}
//0xD1 日期时间头，并发送内容
	//send_usual(0x44);
	//send_usual(0x31);		
//日期发送 
 	//send_usual(0x32);
	//send_usual(0x30);
	//send_usual(0x31);
	//send_usual(0x33);
	//send_usual(0x30);
	//send_usual(0x32);
	//send_usual(0x32);
	//send_usual(0x31);
//0xD2 日期时间头，并发送内容
	//send_usual(0x44);
	//send_usual(0x32);
//时间，ID发送
	//send_usual(0x31);
	//send_usual(0x34);
	//send_usual(0x31);
	//send_usual(0x33);
//ID
    //send_usual(0x30);
	//send_usual(0x32);
	//send_usual(0x33);
//配方种类ID发送
send_usual(0x4b);

				  sy = sys_subjects;
				  
				  sy=sy/10;
		          send_usual((sy % 10)+'0');
		          
				  sy = sys_subjects;
		          send_usual((sy % 10)+'0');
//物料种类
send_usual(0x4d);

	 			  sy = sys_kind_temp;
				  
				  sy=sy/10;
		          send_usual((sy % 10)+'0');
		          
				  sy = sys_kind_temp;
		          send_usual((sy % 10)+'0');
		          



//限制重量

/*//按键标识报头
	send_usual(0x41);
	send_usual(0x31);
//按键标识内容
   	for(i = 0; i < 3; i++)
	{
		temp = KEY_BUF % 10;
		send(temp);
		KEY_BUF=KEY_BUF/10;
	}
*/
//重量标识报头
	send_usual(0x41);
	send_usual(0x32);
//以下为重量值 ///////////////////////////////////////////////////////////////-------------------------------write---------
    if(dis_V == 0)
	   {ok_id2=1;}
	   else{ok_id2=0;}
//    	 for(i = 0; i < 5; i++)
// 	   {
// 		   temp = dis_V % 10;
// 		   send(temp);
// 		   dis_V=dis_V/10;
// 	   }
//	send(fu_hao);

    //send(-0x26);//换行符
}
//将按键数据组合在一起形成16进制数////////////////////////////////////////////
void Code2char()
{
uchar i;
//第一组数据
for(i=0;i<10;i++)
{
  if(rev_1[0] == convertsw[i])
  {
    cvover = i;
	diswich++;  
  }
}
if((diswich == 0)&&(rev_1[0]==0))
{diswich++;}
if((diswich == 0)&&(rev_1[0]==2))
{diswich++;
 fu_hao = -0x03;
}
//第二组数据
for(i=0;i<10;i++)
{
  if(rev_1[1] == convertsw[i])
  {cvover = cvover+(i*10);
   diswich++;
  }
}
if((diswich == 1)&&(rev_1[1]==0))
{diswich++;}
if((diswich == 1)&&(rev_1[1]==2))
{diswich++;
fu_hao = -0x03;
}
if((diswich == 1)&&(rev_1[1]==238))
{
ZERO_SW = 1;
}

//第三组数据
for(i=0;i<10;i++)
{
  if(rev_1[2] == convertsw[i])
  {cvover = cvover+(i*100);
   diswich++;
  }
}
if((diswich == 2)&&(rev_1[2]==0))
{diswich++;}
if((diswich == 2)&&(rev_1[2]==2))
{diswich++;
fu_hao = -0x03;
}
//第四组数据
for(i=0;i<10;i++)
{
  if(rev_1[3] == convertsw[i])
  {cvover = cvover+(i*1000);
   diswich++;
  }
}
if((diswich == 3)&&(rev_1[3]==0))
{diswich++;}
if((diswich == 3)&&(rev_1[3]==2))
{diswich++;
fu_hao = -0x03;
}
//第五组数据
if(rev_1[4] == 182)
{//cvover = cvover+(1*10000); 更改原因，首位不要了，新版本有干扰
diswich++;
}
if((diswich == 4)&&(rev_1[4]==0))
{diswich++;}


if(diswich == 5)
 {
   //dis_data_led(cvover);
   send_data(cvover);
 }

//一个周期之后进行数据清理
diswich = 0;
cvover=0;
fu_hao = -0x05;
ZERO_SW = 0;

}
//抓取信号主函数/////////////////////////////////////////////////////
void ReadPS2()
{
   uchar i;
   while(PS_DATA == 1)
   {
     while(PS_CLK==0);
   	 while(PS_CLK==1);																					 
	 while(!PS_CLK); 	 
   }
   for(i=0;i<8;i++)	 //第一队列
   {
   while(PS_CLK);
   while(!PS_CLK);	
	if(PS_DATA == 1)
	 {
	   rev_1[0]++;
	 }
	 rev_1[0] = rev_1[0] << 1;	   
   }   
   for(i=0;i<8;i++)	 //第二队列
   {
    while(PS_CLK);
    while(!PS_CLK);	
	if(PS_DATA == 1)
	 {
	   rev_1[1]++;
	 }
	 rev_1[1]=rev_1[1]<<1;
    }
   for(i=0;i<8;i++)	 //第三队列
   {
    while(PS_CLK);
    while(!PS_CLK);	
	if(PS_DATA == 1)
	 {
	   rev_1[2]++;
	 }
	 rev_1[2]=rev_1[2]<<1;
    }
   for(i=0;i<8;i++)	 //第四队列
   {
    while(PS_CLK);
    while(!PS_CLK);	
	if(PS_DATA == 1)
	 {
	   rev_1[3]++;
	 }
	 rev_1[3]=rev_1[3]<<1;
    }	   
   for(i=0;i<3;i++)	 //第五队列
   {
    while(PS_CLK);
    while(!PS_CLK);	
	if(PS_DATA == 1)
	 {
	   rev_1[4]++;
	 }
	 rev_1[4]=rev_1[4]<<1;
    }
 //P26=0; 

}

void clk_test()
{
   ulong i=0;
   while(sw_clkchk == 0)
   {
     if(CHK_CLK == 0)
     {i++;}//在时钟引脚为低电平的时候i递增计算
	 else{i=0;}//在时钟引脚为高电平的时候i清零
	 if(i>300)
	 {sw_clkchk = 1;}//当i间隔数变大的时候证明时钟线发送中断，此时可以自动转换显示。开关由此打开。
   }
   
}
void Set_press_int()
{
uchar i;
ulong j;
   delayL(20000);
   delayL(20000);
   delayL(20000);
   delayL(20000);
   delayL(20000);
   delayL(20000);
   delayL(20000);
   delayL(20000);
   delayL(20000);
   delayL(20000);
   ARR_KEY=0;
   ADD_KEY=0;
   for(i=0;i<25;i++)
   {delayL(10000);}//此处之前为进入设定模式
   ARR_KEY=1;
   ADD_KEY=1;
   for(i=0;i<55;i++)
   {delayL(20000);}//此处之前为进入设定模式
   delayL(20000);
   delayL(20000);
   delayL(20000);
   delayL(20000);
   delayL(20000);
   delayL(20000);
   delayL(20000);

if(addorarr == 0)
 {
   for(j=0;j<addorarr_count;j++)
   {
   delayL(6000);
   ADD_KEY=0;//按住
   delayL(8500);
   ADD_KEY=1;//送开
   delayL(6000);     
   }
 }
if(addorarr == 1)
 {
   for(j=0;j<addorarr_count;j++)
   {
   delayL(6000);
   ARR_KEY=0;//按住
   delayL(8500);
   ARR_KEY=1;//送开
   delayL(6000);  
   }
 }
   //此处应返回设定结果。
   sys_res_status=20;  //此时系统进入无限等待状态，不做任何发送

}
int bit0o1(int it,int position) 
{     
 int oc=1;     
 oc<<=(position);     
 return (oc&it)!=0?1:0; 
} 
void Display_LED()
{

uchar i;
PS_DATA = 0;
PS_CLK = 0;
delayL(1000);
//初始化
//开机段发送
PS_CLK = 1;//刚过一个高电平
delayL(1000);//高电平凸起部分
PS_CLK = 0;//时钟高电平拉低
delayL(500);//持续半个周期
PS_DATA = 1;//提前半个中期给START信号
delayL(500);//经过半个周期（时钟信号的剩余半个周期）
PS_CLK = 1;//时钟第二个周期开始（拉高信号准备传送START）
delayL(1000);//经过一个周期持续高电平
PS_CLK = 0;//拉低电平时钟又经过一个周期
delayL(500);//经过半个周期（时钟信号的剩余半个周期）
PS_DATA = 0;//数据信号低电平，START发送完毕
delayL(500);

 for(i=0;i<8;i++)
 {
 if(bit0o1(dis[0],i)==1)
 {
 PS_DATA = 1;//数据接入点（第一组）
 }
 else
 {PS_DATA = 0;}
 PS_CLK = 1;//拉高时钟
 delayL(1000);//经过一个周期上沿 
 PS_CLK = 0;//拉低时钟
 delayL(500);//经过半个周期拉低时钟
 PS_DATA = 0;//拉低数据型号
 delayL(500);//等待半个周期静茹下8位数据
 }
  for(i=0;i<8;i++)
 {
 if(bit0o1(dis[1],i)==1)
 {
 PS_DATA = 1;//数据接入点（第一组）
 }
 else
 {PS_DATA = 0;}//数据接入点（第二组）
 PS_CLK = 1;
 delayL(1000);
 PS_CLK = 0;
 delayL(500);
 PS_DATA = 0;
 delayL(500);//等待半个周期静茹下8位数据
 }

  for(i=0;i<8;i++)
 {
 if(bit0o1(dis[2],i)==1)
 {
 PS_DATA = 1;//数据接入点（第一组）
 }
 else
 {PS_DATA = 0;}//数据接入点（第三组）
 PS_CLK = 1;
 delayL(1000);
 PS_CLK = 0;
 delayL(500);
 PS_DATA = 0;
 delayL(500);//等待半个周期静茹下8位数据
 }


  for(i=0;i<8;i++)
 {
 if(bit0o1(dis[3],i)==1)
 {
 PS_DATA = 1;//数据接入点（第一组）
 }
 else
 {PS_DATA = 0;}//数据接入点（第四组）
 PS_CLK = 1;
 delayL(1000);
 PS_CLK = 0;
 delayL(500);
 PS_DATA = 0;
  delayL(500);//等待半个周期静茹下8位数据
 }

//33
 PS_DATA = 0;//数据接入点33
 PS_CLK = 1;
 delayL(1000);
 PS_CLK = 0;
 delayL(500);
 PS_DATA = 0;
  delayL(500);//等待半个周期静茹下8位数据
//34
 PS_DATA = 0;//数据接入点34
 PS_CLK = 1;
 delayL(1000);
 PS_CLK = 0;
 delayL(500);
 PS_DATA = 0;
  delayL(500);//等待半个周期静茹下8位数据
//35
 PS_CLK = 1;
 PS_DATA = 1;
 delayL(1000);
 PS_CLK = 0;
 PS_DATA = 1;
 delayL(500);
 PS_DATA = 0;
//35
delayL(5000);
PS_CLK = 1;	//两句话一个晚上啊，害死我了。
PS_DATA = 1; 
//上面主要意思是要复位芯片高电平，这样管脚才会正常待机。
} 

void dis_datLED_PL (char recv)
{
  	uchar temp;
	dis[3] = 0x73;
	dis[2] = 0x40;		

		temp = recv % 10;
		dis[0] = dis_data[temp];
		recv=recv/10;
	   	temp = recv % 10;
		dis[1] = dis_data[temp];

  Display_LED();
 
}

void dis_datLED_SD ()
{
  	uchar temp,u1,u2;
		//sys_kinds 种类ID

		u1 = sys_kinds;
		temp = u1%10;
		dis[0] = dis_data[temp];
		u1=u1/10;
		temp = u1%10;
		dis[1] = dis_data[temp];


		u2 = sys_subjects;
		//sys_subjects 分类ID
		temp = u2 % 10;
		dis[2] = dis_data[temp]+0x80;


		u2=u2/10;
	   	temp = u2 % 10;
		dis[3] = dis_data[temp];

  Display_LED();
 
}
void clear_system()
{
    uchar i;
	/*for(i=0;i<100;i++)
	{
		delayL(10000);
		}
	ZERO_KEY = 0;
	ARR_KEY = 0;
	for(i=0;i<100;i++)
	{
		delayL(15000);
		}
	ZERO_KEY = 1;
	ARR_KEY  = 1;
	*/
	for(i=0;i<100;i++)
	{
	  delayL(10000);
	}	//此处应该追加的是进入设定模式，检测设定模式的数值，之后将设定数值清零。（清零动作需要按ZERO键）清3000
    //addorarr_count=3000;
	//addorarr = 1;
	Set_press_int();
	ARR_KEY = 0;
	for(i=0;i<100;i++)//此处延时已经追加
	{
	  delayL(10000);
	}
	for(i=0;i<100;i++)
	{
	  delayL(10000);
	}
	for(i=0;i<100;i++)
	{
	  delayL(10000);
	}
	for(i=0;i<100;i++)
	{
	  delayL(10000);
	}
	for(i=0;i<100;i++)
	{
	  delayL(10000);
	}
	for(i=0;i<100;i++)
	{
	  delayL(10000);
	} 

	ARR_KEY = 1;
	ZERO_KEY = 0;
	delayL(20000);
	delayL(20000);
	ZERO_KEY = 1;
	delayL(15000);

	beep3();//三声提示
}
void start_system()
{
  	clear_system(); //系统一次清零
//	send_usual(0x53);//系统准备完毕，系统在线。
//	send_usual(0x45);//发送截止符
}

 
//程序主入口///////////////////////////////////////////////////////////////---------------------------------------------------------MAIN
void main(void)
{
uint sy = 0;
  if(sys_status==0)//系统状态为0时，自动初始化系统
	{
	init();		//初始化设置 /uint i=0;		
	//初始化终端0
	sys_status=77;//等待启动按键	
	}
	while(1)
	{ 
	   if(sys_status == 77)
	    {
		   if(PRINT_KEY == 0)
			 {
			  delayL(20000);
			  delayL(20000);
			  delayL(20000);
			  sys_status=17;//进入开机轮询状态;//进入系统初始化状态，系统启动
			 }//判断启动按键是否按下
		  }
	   if(sys_status==17)
	   {	        
		    start_system();	
				delayL(20000);
				delayL(20000);
				sys_status=18;	
	   }
		 if(sys_status==18)
		 {
			 	if(ZERO_KEY == 0)
        { 					
				  sys_status=1;
          sys_res_status=0;	
				}
        if(TOTAL_KEY == 0)
        {
					sys_status=66;//进入系统接受项目
		      sys_res_status=0;//接收标识归位?
				}
			}
	   if(sys_status==66)/////////////////////////////////////////////////////屏幕切换
	   {
	   	 clk_test();
		   if(sw_clkchk == 1)
		   {//SW_KEY = 0;
		      SW_LEDP=0;
		      sys_status=4;//进入系统接受项目
		      sys_res_status=0;//接收标识归位。
		      sw_clkchk = 0;
          PS_DATA = 0;
          PS_CLK = 0;
		      delayL(20000);
		      PS_DATA = 1;
          PS_CLK = 1;
		   }
	   }
	   if(sys_status==4)//选择并设定配方ID
	   {
	     //EA = 0;//关闭总中断
		 //前期切断显示屏电源一次，正确供电避免乱码		 
		 //此处只执行一次，不加限制会发生频闪。
		 delayL(20000);
		 delayL(20000);
		 delayL(20000);
		 delayL(20000);
		 clk_test(); //////////////////////////////////////////////////////////////////显示切换
		 SW_LED=0;//此时切换显示器		 先切换再发送
		 sw_clkchk = 0;

		 if(sw_one == 0)
		 {
		 SW_LEDP=0;
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 SW_LEDP=1;
		 delayL(2000);		 
		 sw_one++;	
		 }	 
	     if(ARR_KEY==1)
		 {
		   sys_subjects++;
		   if(sys_subjects>=20){sys_subjects=20;}
		   delayL(60000);
		 }
		 if(ADD_KEY==1) 
	   	 {
		   sys_subjects--;
		   if(sys_subjects<1){sys_subjects=1;}
		   delayL(60000);
		 }
	   	 //dis_counter_uSvhr(sys_subjects);
		 dis_datLED_PL(sys_subjects);

		 if(PRINT_KEY==0) //按确认按键后进入配方发送模式
		 {	
		   delayL(60000);
		   //sys_status=99;
		   	dis[3] = 0x40;
			dis[0] = 0x40;
			dis[2] = 0x3F;
			dis[1] = 0x3F;
			Display_LED();
			sw_one = 0;
			delayL(20000);
			delayL(20000);
			delayL(20000);
			delayL(20000);
			delayL(20000);
			delayL(20000);
			 sys_status = 1;
			 sys_res_status=16;
			 sys_kinds = 1;
			 WLID = sys_kinds;
			 PFID = sys_subjects;
			 PF[0] = byte_read(0x46,PFID);
			//EA = 1;//开启总中断
		 }
	   }

	  if(sys_status==1)//与选择配方数据接收处理
	   {		 
		 //发送所选择的配方种类
		 if(sys_res_status==0)//如果未接收到下条数据则一直发送
		 {
		   
		   if(tick == 0)
			{	  
			      //intko=0;
			    send_usual(0x01);//发送通讯标识
		   //send(sys_subjects);//发送配方ID ASCII //此处变换成两位整数？？？？？
		   //轮询发送数据
				  send_usual(0x01);
				  send_usual(0x04);
				  send_usual(0x04);
				  send_usual(0x04);
				  send_usual(0x04);
				  send_usual(0x0d);				  
				  //发送截止符
		   	}
		   if(tick == 1)
		   {
		     intko=0;
			 if(sys_res_status==0)
		     {send_usual(0x4a);}//变换发J
			 {send_usual(0x45);}//变换发E
			 {send_usual(0x45);}//变换发E
			 {send_usual(0x45);}//变换发E
			 {send_usual(0x45);}//变换发E
			 {send_usual(0x45);}//变换发E
			 {send_usual(0x45);}//变换发E
			 {send_usual(0x45);}//变换发E
		   }
		   		  
		   //send_usual(0x0A);//换行符
		   delayL(20000);//延时调整，等待计算机回馈
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   		   delayL(20000);//延时调整，等待计算机回馈
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   		   delayL(20000);//延时调整，等待计算机回馈
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   		   delayL(20000);//延时调整，等待计算机回馈
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);
		   delayL(20000);

		 }


		 if(sys_res_status==1)//接收到数据之后进行整体数据接收待机
			{
			  SW_LED=0;
			  dis[3] = 0x40;
			  dis[0] = 0x40;
			  dis[2] = 0x3F;
			  dis[1] = 0x38;
			  Display_LED();
			  //清除接收缓冲区
			  recv_buf[0]=0x00;
			  recv_buf[1]=0x00;
			  recv_buf[2]=0x00;
			  recv_buf[3]=0x00;
			  recv_buf[4]=0x00;
			  recv_buf[5]=0x00;
			  recv_buf[6]=0x00;

			  sys_res_status = 44;
			  key_zero = 0;
      }
		 if(sys_res_status==44&&key_zero == 2)//进入轮训接收待机状态 //EEPRROM xieru
			{
			   	 //对应EEPROM存储
				WLID = recv_buf[2]; 
				PFID = recv_buf[1]; 
				
				
				if(WLID == 0x01)//如果物料ID为第一位的时候首先执行扇区清零动作
				 {
				 	SectorErase(0x58+(PFID*2),0x00);//写入前要清扇�
					 delayL(20000);
					 delayL(20000);
					byte_write(recv_buf[3],0x58+(PFID*2),(WLID*2));//更新EEPROM	高位
				    delayL(20000);		            
					byte_write(recv_buf[4],0x58+(PFID*2),(WLID*2)+1);//更新EEPROM	低位
					delayL(20000);
					 //写入对应配方物料数量

          //byte_read(0x58+(PFID*2),(WLID*2))					 
				 }
			   	 else
				 {
				 	byte_write(recv_buf[3],0x58+(PFID*2),(WLID*2));//更新EEPROM	高位
					delayL(20000);
					byte_write(recv_buf[4],0x58+(PFID*2),(WLID*2)+1);//更新EEPROM	低位
					delayL(20000);			 
				 }
				 delayL(20000);
				 delayL(20000);
				 delayL(20000);
				 delayL(20000);
				 //返送结果
				  send_usual(recv_buf[0]);
				  send_usual(recv_buf[1]);
				  send_usual(recv_buf[2]);
 				  send_usual(byte_read(0x58+(PFID*2),(WLID*2)));
 				  send_usual(byte_read(0x58+(PFID*2),(WLID*2)+1));
// 				 	send_usual(recv_buf[3]);
// 				  send_usual(recv_buf[4]);
				  send_usual(recv_buf[5]);
				  send_usual(recv_buf[6]);
				  if(recv_buf[5] == 0xFA)//如果物料已经发送完毕退出到待机状态
					{							
					  sys_status=66;
					}
					else
					{
            PF[0] = recv_buf[5];
						byte_write(0x46,recv_buf[5],PFID);//更新EEPROM	低位
          }


				  //清除接收缓冲区
				  recv_buf[0]=0x00;
				  recv_buf[1]=0x00;
				  recv_buf[2]=0x00;
				  recv_buf[3]=0x00;
				  recv_buf[4]=0x00;
				  recv_buf[5]=0x00;
				  recv_buf[6]=0x00;
				  WLID = 0x00;
				  PFID = 0x00;
				  key_zero = 0;

			   
			}
			




		 if(sys_res_status==16)
		  {
		    ES=0;//禁止串行中断///////////////////////////////////////////////////////////关中断 
			  delayL(15000);			  
			  delayL(400);//3.15
			  recv_data();
			  delayL(20000);
			  delayL(20000);
			  delayL(20000);
			  delayL(20000);
			  delayL(20000);
			  delayL(20000);
			  delayL(20000);			  
		  	  delayL(20000);
			  delayL(20000);
			  delayL(20000);
			  delayL(20000);
		 if(sw_one == 0)
		 {
		 SW_LEDP=0;
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 SW_LEDP=1;
		 delayL(2000);
		 sw_one++;
		 }
			  SW_LED=0;//此时切换显示器开
			  delayL(15000);
			  delayL(20000);
			  delayL(20000);
			  dis_datLED_SD ();//xianshi
			  delayL(20000);
			  delayL(20000);
			  delayL(20000);
			  delayL(20000);
			  dis_datLED_SD ();
			  			  delayL(20000);
			  delayL(20000);
			  			  delayL(20000);
			  delayL(20000);
			  DOT_LED = 0;//显示小数点儿
			  			 
			  //此处显示更新物料种类，物料ID供确认
			  sys_kind_temp = sys_kinds;
			if(PRINT_KEY==0)
			{
			  if(ARR_KEY==0)
			  {DOT_LED = 1;
			  dis[3] = 0x40;
			  dis[0] = 0x40;
			  dis[2] = 0x3F;
			  dis[1] = 0x38;
			  Display_LED();
			  delayL(20000);
			  delayL(20000);
			  delayL(20000);
			  Display_LED();
			  delayL(20000);
			  delayL(20000);
			  Display_LED();
			  delayL(20000);
			  sw_one = 0;
			  sys_res_status=3;
			  }
			 }
		  } 
		  if(sys_res_status == 3)//确认ID后	//////////////////////////////////////////////此处按键设定位置。
			 {			 
			 delayL(20000);
			 delayL(20000);
			 delayL(20000);
			 delayL(20000);
			 delayL(20000);
			 delayL(20000);
			 clk_test(); //////////////////////////////////////////////////////////////////显示切换
			 SW_LED=1;//此时切换显示器
			 sw_clkchk = 0;
         SW_LEDP=0;
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 //SW_LEDP=1;
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 SW_LEDP=0;
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 SW_LEDP=1;
		 delayL(2000);
			 delayL(20000);
			 delayL(20000);
			 delayL(20000);
			 delayL(20000);
		     Set_press_int();//设定重量
			 sys_status=9; //发送确认标识确定设定好重量并开始计量
			 delayL(20000);
			 delayL(20000);
			 delayL(20000);
			 delayL(20000);
			 delayL(20000);
			 }	   
		}



		 if(sys_status == 7)
		 {
		 	//dis_data_led(00003);//ALL
			dis[3] = 0x40;
			dis[0] = 0x40;
			dis[2] = 0x77;
			dis[1] = 0x38;
			Display_LED();
			   //TOTAL_KEY=0;
			   //delayL(20000);
			   //delayL(20000);
			   //TOTAL_KEY=1;
			//sys_res_status = 0;//启动配方等待动作
			//sys_status = 4;
			if(PRINT_KEY == 0)
			{SW_LEDP=0;}
			else{SW_LEDP=1;}
			delayL(20000);
			delayL(20000);
			delayL(20000);
			delayL(20000);
			delayL(20000);
			delayL(20000);
			delayL(20000);
			delayL(20000);
			delayL(20000);
			delayL(20000);
			delayL(20000);
			delayL(20000);
			delayL(20000);
			delayL(20000);
			delayL(20000);
			delayL(20000);
			delayL(20000);
		 if(sw_one == 0)
		 {
		 SW_LEDP=0;
		 delayL(20000);//关闭延时时间。
		 delayL(20000);
		 delayL(20000);
		 delayL(20000);
		 		 delayL(20000);
		 delayL(20000);
		 delayL(20000);
		 		 delayL(20000);
		 delayL(20000);
		 delayL(20000);
		  clk_test(); //////////////////////////////////////////////////////////////////显示切换
			 SW_LED=1;//此时切换显示器
			 sw_clkchk = 0;
		 			//SW_LED=1;//切换到显示器，无限循环
		 SW_LEDP=1;
		 delayL(2000);
		 sw_one++;
		 }
			ES=0;//禁止串行中断///////////////////////////////////////////////////////////关中断 
			delayL(20000);
		 }

		 if(sys_status == 9)
		 {
			   delayL(20000);
			   delayL(20000);
			   //send_usual(0x4f);//结束标示符
			   delayL(40000);//Zero按键开始剂量并进入监控模式
			   ZERO_KEY=0;
			   delayL(40000);
			   ZERO_KEY=1;
			   sys_status=2;
			   			   delayL(20000);
			   delayL(20000);
			   			   delayL(20000);
			   delayL(20000);
			   			   delayL(20000);
			   delayL(20000);
			   			   delayL(20000);
			   delayL(20000);
			   			   delayL(20000);
			   delayL(20000);
			   			   delayL(20000);
			   delayL(20000);
			   			   delayL(20000);
			   delayL(20000);
			   		 		 
		 }
	  	 if(sys_status == 13)
		 {//接收错误
			dis[3] = 0x50;
			dis[0] = 0x40;
			dis[2] = 0x79;
			dis[1] = 0x50;
			Display_LED();
			sys_status = 0;
			delayL(20000);
			delayL(20000);
			delayL(20000);
			delayL(20000);
			delayL(20000);
			delayL(20000);
		 }
	     if(sys_status==2)//数据监控处理
	      {
         //beep3();
	       ReadPS2();
	       Code2char();
		   	 delayL(20000);
			delayL(20000);
			delayL(20000);
			delayL(20000);
			delayL(20000);
			delayL(20000);
			//在此处追加自动发送J的结束处理状态，并清除技术系统的临时保存值。以便下次继续正确配料。
			//if(fu_hao == -0x05)//在拾取数字不为0和负数的时候，OKID立即清零。
			//{ok_id=0;}
			//if(cvover!=0)
			//{ok_id=0;}
			if(ok_id ==0 && ok_id2 == 0)//这个地方有问题
			{time0_tick = 0;}
      if(ok_id == 1||ok_id2 == 1)//当OK_ID大于0的时候证明系统已经一次进入0的状态，证明加料已经到达过底线，这样就需要进入处理机制。
			{
			  time0_tick++;//接收到标识自动增加数字以便延时越过系统进入TOTAL自动发送J
			  //delayL(20000);
			  //delayL(20000);
			  //delayL(20000);
			  //delayL(20000);
			  //delayL(20000);
			  //if(ok_id>2) //延时开关，延时多少次后可以执行最终操作
			  //{
			  if(time0_tick>4)
			   {
			     time0_tick=0;
			  	 ok_id=0;//如果延时此数达到此时需要复位OK_ID以便下次进入。
				   ok_id2=0;
				 //sys_press_temp=0;//系统TEMP警报计数清零	  修正原因，新版本不需要清零
	if(fu_hao==-0x03)
	{cvover = cvover+(byte_read(0x58+(PFID*2),(WLID*2)))*256+byte_read(0x58+(PFID*2),(WLID*2)+1);}
	else{cvover = (byte_read(0x58+(PFID*2),(WLID*2)))*256+byte_read(0x58+(PFID*2),(WLID*2)+1)-cvover;}
//	send_usual(0x45);//发送截止符

				if(WLID == 0x01)//如果物料ID为第一位的时候首先执行扇区清零动作
				 {
				 	SectorErase(0x48+(PFID*2),0x00);//写入前要清扇?
					 delayL(20000);
					 delayL(20000);
					byte_write((cvover/256),0x48+(PFID*2),(WLID*2));//更新EEPROM	高位
				   delayL(20000);		            
					byte_write((cvover%256),0x48+(PFID*2),(WLID*2)+1);//更新EEPROM	低位
					delayL(20000);

          //byte_read(0x58+(PFID*2),(WLID*2))					 
				 }
			   	 else
				 {
				 	byte_write((cvover/256),0x48+(PFID*2),(WLID*2));//更新EEPROM	高位
					delayL(20000);
					byte_write((cvover%256),0x48+(PFID*2),(WLID*2)+1);//更新EEPROM	低位
					delayL(20000);			 
				 }
				 delayL(20000);
				 delayL(20000);
				 delayL(20000);
				 delayL(20000);
				 //自动发送J进入发送状态的SYS_STATUS.
				 //此处为了省事儿，复制一下按键状态
				 sys_res_status=0;
				 sys_status=1;
				 delayL(20000);
			     delayL(20000);
			     delayL(20000);
			     delayL(20000);
			     delayL(20000);
				 		 SW_LEDP=0;
		  clk_test(); //////////////////////////////////////////////////////////////////显示切换
			 SW_LED=0;//此时切换显示器
			 sw_clkchk = 0;
		 //SW_LED=0;
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 SW_LEDP=1;
				 
				 
				 			     
		 /*SW_LEDP=0;		 
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		  clk_test(); //////////////////////////////////////////////////////////////////显示切换
			 SW_LED=0;//此时切换显示器
			 sw_clkchk = 0;
		 //SW_LED=0;
		 SW_LEDP=1;*/
		       delayL(2000);
				   delayL(20000);
			     delayL(20000);
			     delayL(20000);
			     delayL(20000);
			     delayL(20000);
			     delayL(20000);
			     delayL(20000);
				   delayL(20000);
			     delayL(20000);
				   delayL(20000);
			     //send_usual(0x4a);
				 tick=1;
				dis[3] = 0x40;
				dis[0] = 0x40;
				dis[2] = 0x3F;
				dis[1] = 0x3F;
				Display_LED();
				delayL(20000);
				delayL(20000);
				delayL(20000);
				delayL(20000);
				delayL(20000);
				delayL(20000);
				ES=1;//开启串行中断//////////////////////////////////////////////////////////////开中断
				 //此处应显示确认画面
				delayL(20000);
				//清理计数器
				delayL(20000);
				delayL(20000);
				delayL(20000);
				delayL(20000);
				delayL(20000);
				delayL(20000);
				delayL(20000);
				delayL(20000);			  
			  }
			
			}



	       //key_scan();
		   if(PRINT_KEY==0&&ADD_KEY==0) //按确认按键后进入配方发送模式
		    {
			 sys_res_status=0;
			 sys_status=1;
			 delayL(20000);
		     delayL(20000);
			 delayL(20000);
		     delayL(20000);
		     delayL(20000);
		     delayL(20000);		     
		 SW_LEDP=0;
		  clk_test(); //////////////////////////////////////////////////////////////////显示切换
			 SW_LED=0;//此时切换显示器
			 sw_clkchk = 0;
		 //SW_LED=0;
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 delayL(20000);//关闭延时时间。
		 SW_LEDP=1;
		 delayL(2000);
			   delayL(20000);
		     delayL(20000);
		     delayL(20000);
		     delayL(20000);
		     delayL(20000);
			 		     delayL(20000);
		     delayL(20000);
		     delayL(20000);
		     delayL(20000);
			 		     delayL(20000);
		     delayL(20000);
		     delayL(20000);
		     delayL(20000);
			 		     delayL(20000);
		     delayL(20000);
		     delayL(20000);
		     delayL(20000);
			 		     delayL(20000);
		     delayL(20000);
		     delayL(20000);
		     delayL(20000);
			 		     delayL(20000);
		     delayL(20000);
		     delayL(20000);
		     delayL(20000);
			 		     delayL(20000);
		     delayL(20000);
		     delayL(20000);
		     delayL(20000);
		     //send_usual(0x4a);
			 tick=1;
				dis[3] = 0x40;
				dis[0] = 0x40;
				dis[2] = 0x3F;
				dis[1] = 0x3F;
				Display_LED();
				delayL(20000);
				delayL(20000);
				delayL(20000);
				delayL(20000);
				delayL(20000);
				delayL(20000);				
				 //此处应显示确认画面
				delayL(20000);
				//清理计数器
				delayL(20000);
				delayL(20000);
				delayL(20000);
				delayL(20000);
				delayL(20000);
				delayL(20000);
				delayL(20000);
				//按TOTAL清理警报
				//TOTAL_KEY = ;	
			   //SW_KEY = 0;//关闭键盘
			   delayL(20000);//Zero按键开始剂量并进入监控模式
			   delayL(20000);
			   delayL(20000);//Zero按键开始剂量并进入监控模式
			   delayL(20000);
			   delayL(20000);//Zero按键开始剂量并进入监控模式
			   delayL(20000);
		       delayL(20000);//Zero按键开始剂量并进入监控模式
			   delayL(20000);
			   //while(PRINT_KEY==0){;}
			   //SW_KEY = 0;//打开键盘
			   TOTAL_KEY=0;
			   delayL(20000);
			   delayL(20000);
			   delayL(20000);
			   delayL(20000);
			   TOTAL_KEY=0;
			   delayL(20000);
			   delayL(20000);
			   TOTAL_KEY=1;
			   delayL(20000);
			   delayL(20000);
			   delayL(20000);
			   delayL(20000);
			   //TOTAL_KEY=1;
			   //
			   ES=1;//开启串行中断//////////////////////////////////////////////////////////////开中断		 
		   }
	      }

	}
}
//初始化总函数////////////////////////////////////////////////////////////
void init()
{
	init_CPU();		//初始化CPU设置
	LCD_init();		//LCD初始化设置
	init_uart();
	P26=0;
}

//CPU初始化函数///////////////////////////////////////////////////////////
void init_CPU()
{
	EA = 1;			//开CPU中断
	CLK_DIV = 0x00;	//晶振不分频 20M默认
	//P1M0=1;
	//P1M1=0;
	//init_TIMER();//定时器初始化。
}

//延时函数/////////延时时间自己计算///////////////////////////////////////
void delayL(uint n)
{
	uchar i;
	while(n>0)
	{
		for(i=10;i>2;i--);
		n--;
	}	
}

//定时器初始化函数（未使用）/////////////////////////////////////////////
void init_TIMER()
{
EA = 0;
TR0 = 0;
TMOD = 0x01;
	TH0 = (65536-10000)/256;				//定时器0寄存器初值高8位
	TL0 = (65536-10000)%256;				//定时器0寄存器初值低8位
	PT0 = 1;
	ET0 = 1;
	EA = 1;	
}


/****初始化函数end****/
void Serial_Int(void) interrupt 4 using 1
{
        ES = 0;
        if(RI == 1)   //当硬件接收到一个数据时，RI会置位
        {
          if(SBUF == 0x0A&&intko == 0)//只要遇到T就置0 从0开始计数，这个为了读取计算机发送的物料数据准确
					{
							 key_zero = 1;
							 sd_kind = 2;								
					}
					if(SBUF == 0x01&&intko == 0)//只要遇到T就置0 从0开始计数，这个为了读取计算机发送的物料数据准确
					{
							 key_zero = 1;
							 sd_kind = 1;												
					}
					if(SBUF == 0x03&&intko == 0)//只要遇到T就置0 从0开始计数，这个为了读取计算机发送的物料数据准确
					{
							 key_zero = 1;
							 sd_kind = 3;												
					}
				  recv_buf[intko] = SBUF;                //把从串口读出的数存到数组
                RI = 0;                 
                if(intko == 6)
                {
                   if(recv_buf[6] == 0x0d)	
											{					
												intko = 0;
												key_zero = 2;                //确认收到字符串
												if(sd_kind == 1&&sys_status==1)//模式判断入口
												{
													sys_res_status = 1;
												}
												if(sd_kind == 2&&sys_status==44)//模式判断入口
												{
													PFID = recv_buf[1];
													WLID = recv_buf[2];
													weightL[0] = recv_buf[3];
													weightL[1] = recv_buf[4];
												  //重量赋值
												}
											}
                }
				else
				{
				  intko++;
				}
        } 
        ES = 1;
}


/************************************************************/
//void Serial_Int(void) interrupt 4 using 1
//{
//        ES = 0;
//        if(RI == 1)   //当硬件接收到一个数据时，RI会置位
//        {
//                if(SBUF == 0x54)//只要遇到T就置0 从0开始计数，这个为了读取计算机发送的物料数据准确
//				{intko = 0;}
//				recv_buf[intko] = SBUF;                //把从串口读出的数存到数组
//                RI = 0; 
//                intko++;
//                if(intko == 7)
//                {
//                        intko = 0;                //当com_dat = 21时，清0，防止数组溢出
//
//						if(sys_status==17)//当17时，进入系统启动信号接收，其他信号不做处理
//						{
//						  if(recv_buf[0] == 0x4F)//O确认开机指令接收
//						  {
//						     if(recv_buf[1]== 0x50)//P
//						     {sys_res_status = 31;}					
//						  }												
//						}
//						if((sys_status==1||sys_status==18))//当1的时候，做物料ID物料重量字串接收，其他状态不做处理。
//						{
//						  if((recv_buf[0]!=0x30) && (recv_buf[0]!=0x31) && (recv_buf[0]!= 0x41) &&(recv_buf[0]!= 0x45)&&(recv_buf[0]!= 0x54))
//						  {;}
//						  else
//						  {
//						    if(recv_buf[0] == 0x41)//all
//						      {sys_status = 7;
//							   tick=0;}
//						    else if(recv_buf[0] == 0x45) //err
//						      {
//						        if(recv_buf[1]== 0x52)
//						          {sys_status = 6;}
//						        if(recv_buf[1]== 0x50)
//						          {sys_status = 11;}
//						        if(recv_buf[1]== 0x47)
//						          {sys_status = 12;}						 						 
//						      }
//						    else {sys_res_status = 1;sys_status=1;}
//						  }
//						}
//                }
//        } 
//        ES = 1;
//}
