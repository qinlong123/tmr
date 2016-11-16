#include "main.h"
#include "LCD12864.h"
#include "stc12c56.h"
#include "pindef.h"
#include "bmp.h"
#include "eeprom.h"

uchar key_zero = 0;
uchar sd_kind = 0;
uchar PF[2];
//ÄÚ´æ¶¨Òå
/************************************************************/
//#define PF1     0x5A	//Åä·½1	ÉÈÇø
//#define PF2     0x5C	//Åä·½2	ÉÈÇø
//#define PF3     0x5E	//Åä·½3	ÉÈÇø
//#define PF4     0x60	//Åä·½4	ÉÈÇø
//#define PF5     0x62	//Åä·½5	ÉÈÇø
//#define PF6     0x64	//Åä·½6	ÉÈÇø
//#define PF7     0x66	//Åä·½7	ÉÈÇø
//#define PF8     0x68	//Åä·½8	ÉÈÇø
//#define PF9     0x6A	//Åä·½9	ÉÈÇø
//#define PF10    0x6C	//Åä·½10ÉÈÇø
//#define PF11    0x6E	//Åä·½11ÉÈÇø

//save
//#define PF1     0x4A	//Åä·½1	ÉÈÇø
//#define PF2     0x4C	//Åä·½2	ÉÈÇø
//#define PF3     0x4E	//Åä·½3	ÉÈÇø
//#define PF4     0x40	//Åä·½4	ÉÈÇø
//#define PF5     0x42	//Åä·½5	ÉÈÇø
//#define PF6     0x44	//Åä·½6	ÉÈÇø
//#define PF7     0x46	//Åä·½7	ÉÈÇø
//#define PF8     0x48	//Åä·½8	ÉÈÇø
//#define PF9     0x4A	//Åä·½9	ÉÈÇø
//#define PF10    0x4C	//Åä·½10ÉÈÇø
//#define PF11    0x4E	//Åä·½11ÉÈÇø

//PFID-> WLSL(ÎïÁÏÊıÁ¿)
//HBIT 0x46

//×ÓË÷Òı¶¨Òå
//0x01,0x02 ÎïÁÏ1
//0x03,0x04 ÎïÁÏ1
//0x05,0x06 ÎïÁÏ1
//0x07,0x08 ÎïÁÏ1
//0x09,0x0a ÎïÁÏ1
//0x0b,0x0c ÎïÁÏ1
//0x0d,0x0e ÎïÁÏ1
//0x0f,0x10 ÎïÁÏ1
//0x11,0x12 ÎïÁÏ1
//0x13,0x14 ÎïÁÏ1

/************************************************************/
/****Ö÷º¯Êı****/
//644
//´¦ÀíÏÔÊ¾µÄÁ½ÖÖ·½·¨£¬1¡¢Í¨¹ıÊ¶±ğCLOCKµÄÆµÂÊ½áÊøÎ»½øĞĞÇĞ»» 2¡¢Í¨¹ı·¢ËÍLED¿ØÖÆĞ¾Æ¬µÄ¸´Î»ĞÅºÅ¡£
////////Ç°4Î»
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

//////////ÉÏ1Î»
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
//·¢ËÍÄ£¿é³õÊ¼»¯//////////////////////////////////////
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
 //IE=0; //ÆÁ±ÎÖĞ¶Ï

 }
//·¢ËÍ·½·¨////////////µ¥×Ö½Ú·¢ËÍ·½·¨ÓĞ²¹³¥£¨²âÊÔÓÃ£©/////////////////////////
void send(unsigned char text)
 {
  ES=0;
  SBUF=text+0x30;
  while(!TI); 
  TI=0;
  ES=1;
 }
//·¢ËÍ·½·¨///////////µ¥×Ö½Ú/////·¢²¼ºóÊ¹ÓÃ///////////////////////////////////
void send_usual(unsigned char text)//Õı³£·¢ËÍ
 {
  ES=0;
  SBUF=text;
  while(!TI); 
  TI=0;
  ES=1;
 }
//½âÎöÊÕµ½µÄÊı¾İ±à³ÌINTÖµ
void recv_data()
{
sys_press = byte_read(0x58+(PFID*2),(WLID*2))*256+byte_read(0x58+(PFID*2),(WLID*2)+1);
// sys_kinds = (recv_buf[6]-0x30)+(recv_buf[5]-0x30)*10;

   if(sys_press > sys_press_temp)//Èç¹ûÏÂÁÏÖØÁ¿´óÓÚÉÏ´ÎÉè¶¨ÖØÁ¿Ôò¼ÓÁÏÖµÎªÔö¼Ó
    {
      addorarr = 0;
      addorarr_count = sys_press - sys_press_temp;
      sys_press_temp = sys_press;
    }
   if(sys_press < sys_press_temp)//Èç¹ûÏÂÁÏÖØÁ¿´óÓÚÉÏ´ÎÉè¶¨ÖØÁ¿Ôò¼ÓÁÏÖµÎªÔö¼Ó
    {
   addorarr = 1;
   addorarr_count = sys_press_temp - sys_press;
   sys_press_temp = sys_press;
    }
//if(sys_press == sys_press_temp)
//{addorarr = 2;}

//    sys_res_status = 1;//ÒÑ¾­È·ÈÏ½ÓÊÕµ½Êı¾İ
    delayL(20000);
    delayL(20000);
    delayL(20000);
    delayL(20000);

	}
//½ÓÊÕ·½·¨//////////²ÉÓÃÖĞ¶Ï////////////////////////////////////////////////

//ÕûÌå·¢ËÍÄ£¿é£¨²âÊÔÓÃ£©///////////////////////////////////////////////
void send_data(dis_V)
{
	uchar temp,i,sy,j;
	for(j=0;j<15;j++)
	{delayL(20000);}
//0xD1 ÈÕÆÚÊ±¼äÍ·£¬²¢·¢ËÍÄÚÈİ
	//send_usual(0x44);
	//send_usual(0x31);		
//ÈÕÆÚ·¢ËÍ 
 	//send_usual(0x32);
	//send_usual(0x30);
	//send_usual(0x31);
	//send_usual(0x33);
	//send_usual(0x30);
	//send_usual(0x32);
	//send_usual(0x32);
	//send_usual(0x31);
//0xD2 ÈÕÆÚÊ±¼äÍ·£¬²¢·¢ËÍÄÚÈİ
	//send_usual(0x44);
	//send_usual(0x32);
//Ê±¼ä£¬ID·¢ËÍ
	//send_usual(0x31);
	//send_usual(0x34);
	//send_usual(0x31);
	//send_usual(0x33);
//ID
    //send_usual(0x30);
	//send_usual(0x32);
	//send_usual(0x33);
//Åä·½ÖÖÀàID·¢ËÍ
send_usual(0x4b);

				  sy = sys_subjects;
				  
				  sy=sy/10;
		          send_usual((sy % 10)+'0');
		          
				  sy = sys_subjects;
		          send_usual((sy % 10)+'0');
//ÎïÁÏÖÖÀà
send_usual(0x4d);

	 			  sy = sys_kind_temp;
				  
				  sy=sy/10;
		          send_usual((sy % 10)+'0');
		          
				  sy = sys_kind_temp;
		          send_usual((sy % 10)+'0');
		          



//ÏŞÖÆÖØÁ¿

/*//°´¼ü±êÊ¶±¨Í·
	send_usual(0x41);
	send_usual(0x31);
//°´¼ü±êÊ¶ÄÚÈİ
   	for(i = 0; i < 3; i++)
	{
		temp = KEY_BUF % 10;
		send(temp);
		KEY_BUF=KEY_BUF/10;
	}
*/
//ÖØÁ¿±êÊ¶±¨Í·
	send_usual(0x41);
	send_usual(0x32);
//ÒÔÏÂÎªÖØÁ¿Öµ ///////////////////////////////////////////////////////////////-------------------------------write---------
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

    //send(-0x26);//»»ĞĞ·û
}
//½«°´¼üÊı¾İ×éºÏÔÚÒ»ÆğĞÎ³É16½øÖÆÊı////////////////////////////////////////////
void Code2char()
{
uchar i;
//µÚÒ»×éÊı¾İ
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
//µÚ¶ş×éÊı¾İ
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

//µÚÈı×éÊı¾İ
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
//µÚËÄ×éÊı¾İ
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
//µÚÎå×éÊı¾İ
if(rev_1[4] == 182)
{//cvover = cvover+(1*10000); ¸ü¸ÄÔ­Òò£¬Ê×Î»²»ÒªÁË£¬ĞÂ°æ±¾ÓĞ¸ÉÈÅ
diswich++;
}
if((diswich == 4)&&(rev_1[4]==0))
{diswich++;}


if(diswich == 5)
 {
   //dis_data_led(cvover);
   send_data(cvover);
 }

//Ò»¸öÖÜÆÚÖ®ºó½øĞĞÊı¾İÇåÀí
diswich = 0;
cvover=0;
fu_hao = -0x05;
ZERO_SW = 0;

}
//×¥È¡ĞÅºÅÖ÷º¯Êı/////////////////////////////////////////////////////
void ReadPS2()
{
   uchar i;
   while(PS_DATA == 1)
   {
     while(PS_CLK==0);
   	 while(PS_CLK==1);																					 
	 while(!PS_CLK); 	 
   }
   for(i=0;i<8;i++)	 //µÚÒ»¶ÓÁĞ
   {
   while(PS_CLK);
   while(!PS_CLK);	
	if(PS_DATA == 1)
	 {
	   rev_1[0]++;
	 }
	 rev_1[0] = rev_1[0] << 1;	   
   }   
   for(i=0;i<8;i++)	 //µÚ¶ş¶ÓÁĞ
   {
    while(PS_CLK);
    while(!PS_CLK);	
	if(PS_DATA == 1)
	 {
	   rev_1[1]++;
	 }
	 rev_1[1]=rev_1[1]<<1;
    }
   for(i=0;i<8;i++)	 //µÚÈı¶ÓÁĞ
   {
    while(PS_CLK);
    while(!PS_CLK);	
	if(PS_DATA == 1)
	 {
	   rev_1[2]++;
	 }
	 rev_1[2]=rev_1[2]<<1;
    }
   for(i=0;i<8;i++)	 //µÚËÄ¶ÓÁĞ
   {
    while(PS_CLK);
    while(!PS_CLK);	
	if(PS_DATA == 1)
	 {
	   rev_1[3]++;
	 }
	 rev_1[3]=rev_1[3]<<1;
    }	   
   for(i=0;i<3;i++)	 //µÚÎå¶ÓÁĞ
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
     {i++;}//ÔÚÊ±ÖÓÒı½ÅÎªµÍµçÆ½µÄÊ±ºòiµİÔö¼ÆËã
	 else{i=0;}//ÔÚÊ±ÖÓÒı½ÅÎª¸ßµçÆ½µÄÊ±ºòiÇåÁã
	 if(i>300)
	 {sw_clkchk = 1;}//µ±i¼ä¸ôÊı±ä´óµÄÊ±ºòÖ¤Ã÷Ê±ÖÓÏß·¢ËÍÖĞ¶Ï£¬´ËÊ±¿ÉÒÔ×Ô¶¯×ª»»ÏÔÊ¾¡£¿ª¹ØÓÉ´Ë´ò¿ª¡£
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
   {delayL(10000);}//´Ë´¦Ö®Ç°Îª½øÈëÉè¶¨Ä£Ê½
   ARR_KEY=1;
   ADD_KEY=1;
   for(i=0;i<55;i++)
   {delayL(20000);}//´Ë´¦Ö®Ç°Îª½øÈëÉè¶¨Ä£Ê½
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
   ADD_KEY=0;//°´×¡
   delayL(8500);
   ADD_KEY=1;//ËÍ¿ª
   delayL(6000);     
   }
 }
if(addorarr == 1)
 {
   for(j=0;j<addorarr_count;j++)
   {
   delayL(6000);
   ARR_KEY=0;//°´×¡
   delayL(8500);
   ARR_KEY=1;//ËÍ¿ª
   delayL(6000);  
   }
 }
   //´Ë´¦Ó¦·µ»ØÉè¶¨½á¹û¡£
   sys_res_status=20;  //´ËÊ±ÏµÍ³½øÈëÎŞÏŞµÈ´ı×´Ì¬£¬²»×öÈÎºÎ·¢ËÍ

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
//³õÊ¼»¯
//¿ª»ú¶Î·¢ËÍ
PS_CLK = 1;//¸Õ¹ıÒ»¸ö¸ßµçÆ½
delayL(1000);//¸ßµçÆ½Í¹Æğ²¿·Ö
PS_CLK = 0;//Ê±ÖÓ¸ßµçÆ½À­µÍ
delayL(500);//³ÖĞø°ë¸öÖÜÆÚ
PS_DATA = 1;//ÌáÇ°°ë¸öÖĞÆÚ¸øSTARTĞÅºÅ
delayL(500);//¾­¹ı°ë¸öÖÜÆÚ£¨Ê±ÖÓĞÅºÅµÄÊ£Óà°ë¸öÖÜÆÚ£©
PS_CLK = 1;//Ê±ÖÓµÚ¶ş¸öÖÜÆÚ¿ªÊ¼£¨À­¸ßĞÅºÅ×¼±¸´«ËÍSTART£©
delayL(1000);//¾­¹ıÒ»¸öÖÜÆÚ³ÖĞø¸ßµçÆ½
PS_CLK = 0;//À­µÍµçÆ½Ê±ÖÓÓÖ¾­¹ıÒ»¸öÖÜÆÚ
delayL(500);//¾­¹ı°ë¸öÖÜÆÚ£¨Ê±ÖÓĞÅºÅµÄÊ£Óà°ë¸öÖÜÆÚ£©
PS_DATA = 0;//Êı¾İĞÅºÅµÍµçÆ½£¬START·¢ËÍÍê±Ï
delayL(500);

 for(i=0;i<8;i++)
 {
 if(bit0o1(dis[0],i)==1)
 {
 PS_DATA = 1;//Êı¾İ½ÓÈëµã£¨µÚÒ»×é£©
 }
 else
 {PS_DATA = 0;}
 PS_CLK = 1;//À­¸ßÊ±ÖÓ
 delayL(1000);//¾­¹ıÒ»¸öÖÜÆÚÉÏÑØ 
 PS_CLK = 0;//À­µÍÊ±ÖÓ
 delayL(500);//¾­¹ı°ë¸öÖÜÆÚÀ­µÍÊ±ÖÓ
 PS_DATA = 0;//À­µÍÊı¾İĞÍºÅ
 delayL(500);//µÈ´ı°ë¸öÖÜÆÚ¾²ÈãÏÂ8Î»Êı¾İ
 }
  for(i=0;i<8;i++)
 {
 if(bit0o1(dis[1],i)==1)
 {
 PS_DATA = 1;//Êı¾İ½ÓÈëµã£¨µÚÒ»×é£©
 }
 else
 {PS_DATA = 0;}//Êı¾İ½ÓÈëµã£¨µÚ¶ş×é£©
 PS_CLK = 1;
 delayL(1000);
 PS_CLK = 0;
 delayL(500);
 PS_DATA = 0;
 delayL(500);//µÈ´ı°ë¸öÖÜÆÚ¾²ÈãÏÂ8Î»Êı¾İ
 }

  for(i=0;i<8;i++)
 {
 if(bit0o1(dis[2],i)==1)
 {
 PS_DATA = 1;//Êı¾İ½ÓÈëµã£¨µÚÒ»×é£©
 }
 else
 {PS_DATA = 0;}//Êı¾İ½ÓÈëµã£¨µÚÈı×é£©
 PS_CLK = 1;
 delayL(1000);
 PS_CLK = 0;
 delayL(500);
 PS_DATA = 0;
 delayL(500);//µÈ´ı°ë¸öÖÜÆÚ¾²ÈãÏÂ8Î»Êı¾İ
 }


  for(i=0;i<8;i++)
 {
 if(bit0o1(dis[3],i)==1)
 {
 PS_DATA = 1;//Êı¾İ½ÓÈëµã£¨µÚÒ»×é£©
 }
 else
 {PS_DATA = 0;}//Êı¾İ½ÓÈëµã£¨µÚËÄ×é£©
 PS_CLK = 1;
 delayL(1000);
 PS_CLK = 0;
 delayL(500);
 PS_DATA = 0;
  delayL(500);//µÈ´ı°ë¸öÖÜÆÚ¾²ÈãÏÂ8Î»Êı¾İ
 }

//33
 PS_DATA = 0;//Êı¾İ½ÓÈëµã33
 PS_CLK = 1;
 delayL(1000);
 PS_CLK = 0;
 delayL(500);
 PS_DATA = 0;
  delayL(500);//µÈ´ı°ë¸öÖÜÆÚ¾²ÈãÏÂ8Î»Êı¾İ
//34
 PS_DATA = 0;//Êı¾İ½ÓÈëµã34
 PS_CLK = 1;
 delayL(1000);
 PS_CLK = 0;
 delayL(500);
 PS_DATA = 0;
  delayL(500);//µÈ´ı°ë¸öÖÜÆÚ¾²ÈãÏÂ8Î»Êı¾İ
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
PS_CLK = 1;	//Á½¾ä»°Ò»¸öÍíÉÏ°¡£¬º¦ËÀÎÒÁË¡£
PS_DATA = 1; 
//ÉÏÃæÖ÷ÒªÒâË¼ÊÇÒª¸´Î»Ğ¾Æ¬¸ßµçÆ½£¬ÕâÑù¹Ü½Å²Å»áÕı³£´ı»ú¡£
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
		//sys_kinds ÖÖÀàID

		u1 = sys_kinds;
		temp = u1%10;
		dis[0] = dis_data[temp];
		u1=u1/10;
		temp = u1%10;
		dis[1] = dis_data[temp];


		u2 = sys_subjects;
		//sys_subjects ·ÖÀàID
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
	}	//´Ë´¦Ó¦¸Ã×·¼ÓµÄÊÇ½øÈëÉè¶¨Ä£Ê½£¬¼ì²âÉè¶¨Ä£Ê½µÄÊıÖµ£¬Ö®ºó½«Éè¶¨ÊıÖµÇåÁã¡££¨ÇåÁã¶¯×÷ĞèÒª°´ZERO¼ü£©Çå3000
    //addorarr_count=3000;
	//addorarr = 1;
	Set_press_int();
	ARR_KEY = 0;
	for(i=0;i<100;i++)//´Ë´¦ÑÓÊ±ÒÑ¾­×·¼Ó
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

	beep3();//ÈıÉùÌáÊ¾
}
void start_system()
{
  	clear_system(); //ÏµÍ³Ò»´ÎÇåÁã
//	send_usual(0x53);//ÏµÍ³×¼±¸Íê±Ï£¬ÏµÍ³ÔÚÏß¡£
//	send_usual(0x45);//·¢ËÍ½ØÖ¹·û
}

 
//³ÌĞòÖ÷Èë¿Ú///////////////////////////////////////////////////////////////---------------------------------------------------------MAIN
void main(void)
{
uint sy = 0;
  if(sys_status==0)//ÏµÍ³×´Ì¬Îª0Ê±£¬×Ô¶¯³õÊ¼»¯ÏµÍ³
	{
	init();		//³õÊ¼»¯ÉèÖÃ /uint i=0;		
	//³õÊ¼»¯ÖÕ¶Ë0
	sys_status=77;//µÈ´ıÆô¶¯°´¼ü	
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
			  sys_status=17;//½øÈë¿ª»úÂÖÑ¯×´Ì¬;//½øÈëÏµÍ³³õÊ¼»¯×´Ì¬£¬ÏµÍ³Æô¶¯
			 }//ÅĞ¶ÏÆô¶¯°´¼üÊÇ·ñ°´ÏÂ
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
					sys_status=66;//½øÈëÏµÍ³½ÓÊÜÏîÄ¿
		      sys_res_status=0;//½ÓÊÕ±êÊ¶¹éÎ»?
				}
			}
	   if(sys_status==66)/////////////////////////////////////////////////////ÆÁÄ»ÇĞ»»
	   {
	   	 clk_test();
		   if(sw_clkchk == 1)
		   {//SW_KEY = 0;
		      SW_LEDP=0;
		      sys_status=4;//½øÈëÏµÍ³½ÓÊÜÏîÄ¿
		      sys_res_status=0;//½ÓÊÕ±êÊ¶¹éÎ»¡£
		      sw_clkchk = 0;
          PS_DATA = 0;
          PS_CLK = 0;
		      delayL(20000);
		      PS_DATA = 1;
          PS_CLK = 1;
		   }
	   }
	   if(sys_status==4)//Ñ¡Ôñ²¢Éè¶¨Åä·½ID
	   {
	     //EA = 0;//¹Ø±Õ×ÜÖĞ¶Ï
		 //Ç°ÆÚÇĞ¶ÏÏÔÊ¾ÆÁµçÔ´Ò»´Î£¬ÕıÈ·¹©µç±ÜÃâÂÒÂë		 
		 //´Ë´¦Ö»Ö´ĞĞÒ»´Î£¬²»¼ÓÏŞÖÆ»á·¢ÉúÆµÉÁ¡£
		 delayL(20000);
		 delayL(20000);
		 delayL(20000);
		 delayL(20000);
		 clk_test(); //////////////////////////////////////////////////////////////////ÏÔÊ¾ÇĞ»»
		 SW_LED=0;//´ËÊ±ÇĞ»»ÏÔÊ¾Æ÷		 ÏÈÇĞ»»ÔÙ·¢ËÍ
		 sw_clkchk = 0;

		 if(sw_one == 0)
		 {
		 SW_LEDP=0;
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
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

		 if(PRINT_KEY==0) //°´È·ÈÏ°´¼üºó½øÈëÅä·½·¢ËÍÄ£Ê½
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
			//EA = 1;//¿ªÆô×ÜÖĞ¶Ï
		 }
	   }

	  if(sys_status==1)//ÓëÑ¡ÔñÅä·½Êı¾İ½ÓÊÕ´¦Àí
	   {		 
		 //·¢ËÍËùÑ¡ÔñµÄÅä·½ÖÖÀà
		 if(sys_res_status==0)//Èç¹ûÎ´½ÓÊÕµ½ÏÂÌõÊı¾İÔòÒ»Ö±·¢ËÍ
		 {
		   
		   if(tick == 0)
			{	  
			      //intko=0;
			    send_usual(0x01);//·¢ËÍÍ¨Ñ¶±êÊ¶
		   //send(sys_subjects);//·¢ËÍÅä·½ID ASCII //´Ë´¦±ä»»³ÉÁ½Î»ÕûÊı£¿£¿£¿£¿£¿
		   //ÂÖÑ¯·¢ËÍÊı¾İ
				  send_usual(0x01);
				  send_usual(0x04);
				  send_usual(0x04);
				  send_usual(0x04);
				  send_usual(0x04);
				  send_usual(0x0d);				  
				  //·¢ËÍ½ØÖ¹·û
		   	}
		   if(tick == 1)
		   {
		     intko=0;
			 if(sys_res_status==0)
		     {send_usual(0x4a);}//±ä»»·¢J
			 {send_usual(0x45);}//±ä»»·¢E
			 {send_usual(0x45);}//±ä»»·¢E
			 {send_usual(0x45);}//±ä»»·¢E
			 {send_usual(0x45);}//±ä»»·¢E
			 {send_usual(0x45);}//±ä»»·¢E
			 {send_usual(0x45);}//±ä»»·¢E
			 {send_usual(0x45);}//±ä»»·¢E
		   }
		   		  
		   //send_usual(0x0A);//»»ĞĞ·û
		   delayL(20000);//ÑÓÊ±µ÷Õû£¬µÈ´ı¼ÆËã»ú»ØÀ¡
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
		   		   delayL(20000);//ÑÓÊ±µ÷Õû£¬µÈ´ı¼ÆËã»ú»ØÀ¡
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
		   		   delayL(20000);//ÑÓÊ±µ÷Õû£¬µÈ´ı¼ÆËã»ú»ØÀ¡
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
		   		   delayL(20000);//ÑÓÊ±µ÷Õû£¬µÈ´ı¼ÆËã»ú»ØÀ¡
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


		 if(sys_res_status==1)//½ÓÊÕµ½Êı¾İÖ®ºó½øĞĞÕûÌåÊı¾İ½ÓÊÕ´ı»ú
			{
			  SW_LED=0;
			  dis[3] = 0x40;
			  dis[0] = 0x40;
			  dis[2] = 0x3F;
			  dis[1] = 0x38;
			  Display_LED();
			  //Çå³ı½ÓÊÕ»º³åÇø
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
		 if(sys_res_status==44&&key_zero == 2)//½øÈëÂÖÑµ½ÓÊÕ´ı»ú×´Ì¬ //EEPRROM xieru
			{
			   	 //¶ÔÓ¦EEPROM´æ´¢
				WLID = recv_buf[2]; 
				PFID = recv_buf[1]; 
				
				
				if(WLID == 0x01)//Èç¹ûÎïÁÏIDÎªµÚÒ»Î»µÄÊ±ºòÊ×ÏÈÖ´ĞĞÉÈÇøÇåÁã¶¯×÷
				 {
				 	SectorErase(0x58+(PFID*2),0x00);//Ğ´ÈëÇ°ÒªÇåÉÈÇ
					 delayL(20000);
					 delayL(20000);
					byte_write(recv_buf[3],0x58+(PFID*2),(WLID*2));//¸üĞÂEEPROM	¸ßÎ»
				    delayL(20000);		            
					byte_write(recv_buf[4],0x58+(PFID*2),(WLID*2)+1);//¸üĞÂEEPROM	µÍÎ»
					delayL(20000);
					 //Ğ´Èë¶ÔÓ¦Åä·½ÎïÁÏÊıÁ¿

          //byte_read(0x58+(PFID*2),(WLID*2))					 
				 }
			   	 else
				 {
				 	byte_write(recv_buf[3],0x58+(PFID*2),(WLID*2));//¸üĞÂEEPROM	¸ßÎ»
					delayL(20000);
					byte_write(recv_buf[4],0x58+(PFID*2),(WLID*2)+1);//¸üĞÂEEPROM	µÍÎ»
					delayL(20000);			 
				 }
				 delayL(20000);
				 delayL(20000);
				 delayL(20000);
				 delayL(20000);
				 //·µËÍ½á¹û
				  send_usual(recv_buf[0]);
				  send_usual(recv_buf[1]);
				  send_usual(recv_buf[2]);
 				  send_usual(byte_read(0x58+(PFID*2),(WLID*2)));
 				  send_usual(byte_read(0x58+(PFID*2),(WLID*2)+1));
// 				 	send_usual(recv_buf[3]);
// 				  send_usual(recv_buf[4]);
				  send_usual(recv_buf[5]);
				  send_usual(recv_buf[6]);
				  if(recv_buf[5] == 0xFA)//Èç¹ûÎïÁÏÒÑ¾­·¢ËÍÍê±ÏÍË³öµ½´ı»ú×´Ì¬
					{							
					  sys_status=66;
					}
					else
					{
            PF[0] = recv_buf[5];
						byte_write(0x46,recv_buf[5],PFID);//¸üĞÂEEPROM	µÍÎ»
          }


				  //Çå³ı½ÓÊÕ»º³åÇø
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
		    ES=0;//½ûÖ¹´®ĞĞÖĞ¶Ï///////////////////////////////////////////////////////////¹ØÖĞ¶Ï 
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
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 SW_LEDP=1;
		 delayL(2000);
		 sw_one++;
		 }
			  SW_LED=0;//´ËÊ±ÇĞ»»ÏÔÊ¾Æ÷¿ª
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
			  DOT_LED = 0;//ÏÔÊ¾Ğ¡Êıµã¶ù
			  			 
			  //´Ë´¦ÏÔÊ¾¸üĞÂÎïÁÏÖÖÀà£¬ÎïÁÏID¹©È·ÈÏ
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
		  if(sys_res_status == 3)//È·ÈÏIDºó	//////////////////////////////////////////////´Ë´¦°´¼üÉè¶¨Î»ÖÃ¡£
			 {			 
			 delayL(20000);
			 delayL(20000);
			 delayL(20000);
			 delayL(20000);
			 delayL(20000);
			 delayL(20000);
			 clk_test(); //////////////////////////////////////////////////////////////////ÏÔÊ¾ÇĞ»»
			 SW_LED=1;//´ËÊ±ÇĞ»»ÏÔÊ¾Æ÷
			 sw_clkchk = 0;
         SW_LEDP=0;
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 //SW_LEDP=1;
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 SW_LEDP=0;
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 SW_LEDP=1;
		 delayL(2000);
			 delayL(20000);
			 delayL(20000);
			 delayL(20000);
			 delayL(20000);
		     Set_press_int();//Éè¶¨ÖØÁ¿
			 sys_status=9; //·¢ËÍÈ·ÈÏ±êÊ¶È·¶¨Éè¶¨ºÃÖØÁ¿²¢¿ªÊ¼¼ÆÁ¿
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
			//sys_res_status = 0;//Æô¶¯Åä·½µÈ´ı¶¯×÷
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
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);
		 delayL(20000);
		 delayL(20000);
		 		 delayL(20000);
		 delayL(20000);
		 delayL(20000);
		 		 delayL(20000);
		 delayL(20000);
		 delayL(20000);
		  clk_test(); //////////////////////////////////////////////////////////////////ÏÔÊ¾ÇĞ»»
			 SW_LED=1;//´ËÊ±ÇĞ»»ÏÔÊ¾Æ÷
			 sw_clkchk = 0;
		 			//SW_LED=1;//ÇĞ»»µ½ÏÔÊ¾Æ÷£¬ÎŞÏŞÑ­»·
		 SW_LEDP=1;
		 delayL(2000);
		 sw_one++;
		 }
			ES=0;//½ûÖ¹´®ĞĞÖĞ¶Ï///////////////////////////////////////////////////////////¹ØÖĞ¶Ï 
			delayL(20000);
		 }

		 if(sys_status == 9)
		 {
			   delayL(20000);
			   delayL(20000);
			   //send_usual(0x4f);//½áÊø±êÊ¾·û
			   delayL(40000);//Zero°´¼ü¿ªÊ¼¼ÁÁ¿²¢½øÈë¼à¿ØÄ£Ê½
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
		 {//½ÓÊÕ´íÎó
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
	     if(sys_status==2)//Êı¾İ¼à¿Ø´¦Àí
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
			//ÔÚ´Ë´¦×·¼Ó×Ô¶¯·¢ËÍJµÄ½áÊø´¦Àí×´Ì¬£¬²¢Çå³ı¼¼ÊõÏµÍ³µÄÁÙÊ±±£´æÖµ¡£ÒÔ±ãÏÂ´Î¼ÌĞøÕıÈ·ÅäÁÏ¡£
			//if(fu_hao == -0x05)//ÔÚÊ°È¡Êı×Ö²»Îª0ºÍ¸ºÊıµÄÊ±ºò£¬OKIDÁ¢¼´ÇåÁã¡£
			//{ok_id=0;}
			//if(cvover!=0)
			//{ok_id=0;}
			if(ok_id ==0 && ok_id2 == 0)//Õâ¸öµØ·½ÓĞÎÊÌâ
			{time0_tick = 0;}
      if(ok_id == 1||ok_id2 == 1)//µ±OK_ID´óÓÚ0µÄÊ±ºòÖ¤Ã÷ÏµÍ³ÒÑ¾­Ò»´Î½øÈë0µÄ×´Ì¬£¬Ö¤Ã÷¼ÓÁÏÒÑ¾­µ½´ï¹ıµ×Ïß£¬ÕâÑù¾ÍĞèÒª½øÈë´¦Àí»úÖÆ¡£
			{
			  time0_tick++;//½ÓÊÕµ½±êÊ¶×Ô¶¯Ôö¼ÓÊı×ÖÒÔ±ãÑÓÊ±Ô½¹ıÏµÍ³½øÈëTOTAL×Ô¶¯·¢ËÍJ
			  //delayL(20000);
			  //delayL(20000);
			  //delayL(20000);
			  //delayL(20000);
			  //delayL(20000);
			  //if(ok_id>2) //ÑÓÊ±¿ª¹Ø£¬ÑÓÊ±¶àÉÙ´Îºó¿ÉÒÔÖ´ĞĞ×îÖÕ²Ù×÷
			  //{
			  if(time0_tick>4)
			   {
			     time0_tick=0;
			  	 ok_id=0;//Èç¹ûÑÓÊ±´ËÊı´ïµ½´ËÊ±ĞèÒª¸´Î»OK_IDÒÔ±ãÏÂ´Î½øÈë¡£
				   ok_id2=0;
				 //sys_press_temp=0;//ÏµÍ³TEMP¾¯±¨¼ÆÊıÇåÁã	  ĞŞÕıÔ­Òò£¬ĞÂ°æ±¾²»ĞèÒªÇåÁã
	if(fu_hao==-0x03)
	{cvover = cvover+(byte_read(0x58+(PFID*2),(WLID*2)))*256+byte_read(0x58+(PFID*2),(WLID*2)+1);}
	else{cvover = (byte_read(0x58+(PFID*2),(WLID*2)))*256+byte_read(0x58+(PFID*2),(WLID*2)+1)-cvover;}
//	send_usual(0x45);//·¢ËÍ½ØÖ¹·û

				if(WLID == 0x01)//Èç¹ûÎïÁÏIDÎªµÚÒ»Î»µÄÊ±ºòÊ×ÏÈÖ´ĞĞÉÈÇøÇåÁã¶¯×÷
				 {
				 	SectorErase(0x48+(PFID*2),0x00);//Ğ´ÈëÇ°ÒªÇåÉÈ?
					 delayL(20000);
					 delayL(20000);
					byte_write((cvover/256),0x48+(PFID*2),(WLID*2));//¸üĞÂEEPROM	¸ßÎ»
				   delayL(20000);		            
					byte_write((cvover%256),0x48+(PFID*2),(WLID*2)+1);//¸üĞÂEEPROM	µÍÎ»
					delayL(20000);

          //byte_read(0x58+(PFID*2),(WLID*2))					 
				 }
			   	 else
				 {
				 	byte_write((cvover/256),0x48+(PFID*2),(WLID*2));//¸üĞÂEEPROM	¸ßÎ»
					delayL(20000);
					byte_write((cvover%256),0x48+(PFID*2),(WLID*2)+1);//¸üĞÂEEPROM	µÍÎ»
					delayL(20000);			 
				 }
				 delayL(20000);
				 delayL(20000);
				 delayL(20000);
				 delayL(20000);
				 //×Ô¶¯·¢ËÍJ½øÈë·¢ËÍ×´Ì¬µÄSYS_STATUS.
				 //´Ë´¦ÎªÁËÊ¡ÊÂ¶ù£¬¸´ÖÆÒ»ÏÂ°´¼ü×´Ì¬
				 sys_res_status=0;
				 sys_status=1;
				 delayL(20000);
			     delayL(20000);
			     delayL(20000);
			     delayL(20000);
			     delayL(20000);
				 		 SW_LEDP=0;
		  clk_test(); //////////////////////////////////////////////////////////////////ÏÔÊ¾ÇĞ»»
			 SW_LED=0;//´ËÊ±ÇĞ»»ÏÔÊ¾Æ÷
			 sw_clkchk = 0;
		 //SW_LED=0;
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 SW_LEDP=1;
				 
				 
				 			     
		 /*SW_LEDP=0;		 
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		  clk_test(); //////////////////////////////////////////////////////////////////ÏÔÊ¾ÇĞ»»
			 SW_LED=0;//´ËÊ±ÇĞ»»ÏÔÊ¾Æ÷
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
				ES=1;//¿ªÆô´®ĞĞÖĞ¶Ï//////////////////////////////////////////////////////////////¿ªÖĞ¶Ï
				 //´Ë´¦Ó¦ÏÔÊ¾È·ÈÏ»­Ãæ
				delayL(20000);
				//ÇåÀí¼ÆÊıÆ÷
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
		   if(PRINT_KEY==0&&ADD_KEY==0) //°´È·ÈÏ°´¼üºó½øÈëÅä·½·¢ËÍÄ£Ê½
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
		  clk_test(); //////////////////////////////////////////////////////////////////ÏÔÊ¾ÇĞ»»
			 SW_LED=0;//´ËÊ±ÇĞ»»ÏÔÊ¾Æ÷
			 sw_clkchk = 0;
		 //SW_LED=0;
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
		 delayL(20000);//¹Ø±ÕÑÓÊ±Ê±¼ä¡£
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
				 //´Ë´¦Ó¦ÏÔÊ¾È·ÈÏ»­Ãæ
				delayL(20000);
				//ÇåÀí¼ÆÊıÆ÷
				delayL(20000);
				delayL(20000);
				delayL(20000);
				delayL(20000);
				delayL(20000);
				delayL(20000);
				delayL(20000);
				//°´TOTALÇåÀí¾¯±¨
				//TOTAL_KEY = ;	
			   //SW_KEY = 0;//¹Ø±Õ¼üÅÌ
			   delayL(20000);//Zero°´¼ü¿ªÊ¼¼ÁÁ¿²¢½øÈë¼à¿ØÄ£Ê½
			   delayL(20000);
			   delayL(20000);//Zero°´¼ü¿ªÊ¼¼ÁÁ¿²¢½øÈë¼à¿ØÄ£Ê½
			   delayL(20000);
			   delayL(20000);//Zero°´¼ü¿ªÊ¼¼ÁÁ¿²¢½øÈë¼à¿ØÄ£Ê½
			   delayL(20000);
		       delayL(20000);//Zero°´¼ü¿ªÊ¼¼ÁÁ¿²¢½øÈë¼à¿ØÄ£Ê½
			   delayL(20000);
			   //while(PRINT_KEY==0){;}
			   //SW_KEY = 0;//´ò¿ª¼üÅÌ
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
			   ES=1;//¿ªÆô´®ĞĞÖĞ¶Ï//////////////////////////////////////////////////////////////¿ªÖĞ¶Ï		 
		   }
	      }

	}
}
//³õÊ¼»¯×Üº¯Êı////////////////////////////////////////////////////////////
void init()
{
	init_CPU();		//³õÊ¼»¯CPUÉèÖÃ
	LCD_init();		//LCD³õÊ¼»¯ÉèÖÃ
	init_uart();
	P26=0;
}

//CPU³õÊ¼»¯º¯Êı///////////////////////////////////////////////////////////
void init_CPU()
{
	EA = 1;			//¿ªCPUÖĞ¶Ï
	CLK_DIV = 0x00;	//¾§Õñ²»·ÖÆµ 20MÄ¬ÈÏ
	//P1M0=1;
	//P1M1=0;
	//init_TIMER();//¶¨Ê±Æ÷³õÊ¼»¯¡£
}

//ÑÓÊ±º¯Êı/////////ÑÓÊ±Ê±¼ä×Ô¼º¼ÆËã///////////////////////////////////////
void delayL(uint n)
{
	uchar i;
	while(n>0)
	{
		for(i=10;i>2;i--);
		n--;
	}	
}

//¶¨Ê±Æ÷³õÊ¼»¯º¯Êı£¨Î´Ê¹ÓÃ£©/////////////////////////////////////////////
void init_TIMER()
{
EA = 0;
TR0 = 0;
TMOD = 0x01;
	TH0 = (65536-10000)/256;				//¶¨Ê±Æ÷0¼Ä´æÆ÷³õÖµ¸ß8Î»
	TL0 = (65536-10000)%256;				//¶¨Ê±Æ÷0¼Ä´æÆ÷³õÖµµÍ8Î»
	PT0 = 1;
	ET0 = 1;
	EA = 1;	
}


/****³õÊ¼»¯º¯Êıend****/
void Serial_Int(void) interrupt 4 using 1
{
        ES = 0;
        if(RI == 1)   //µ±Ó²¼ş½ÓÊÕµ½Ò»¸öÊı¾İÊ±£¬RI»áÖÃÎ»
        {
          if(SBUF == 0x0A&&intko == 0)//Ö»ÒªÓöµ½T¾ÍÖÃ0 ´Ó0¿ªÊ¼¼ÆÊı£¬Õâ¸öÎªÁË¶ÁÈ¡¼ÆËã»ú·¢ËÍµÄÎïÁÏÊı¾İ×¼È·
					{
							 key_zero = 1;
							 sd_kind = 2;								
					}
					if(SBUF == 0x01&&intko == 0)//Ö»ÒªÓöµ½T¾ÍÖÃ0 ´Ó0¿ªÊ¼¼ÆÊı£¬Õâ¸öÎªÁË¶ÁÈ¡¼ÆËã»ú·¢ËÍµÄÎïÁÏÊı¾İ×¼È·
					{
							 key_zero = 1;
							 sd_kind = 1;												
					}
					if(SBUF == 0x03&&intko == 0)//Ö»ÒªÓöµ½T¾ÍÖÃ0 ´Ó0¿ªÊ¼¼ÆÊı£¬Õâ¸öÎªÁË¶ÁÈ¡¼ÆËã»ú·¢ËÍµÄÎïÁÏÊı¾İ×¼È·
					{
							 key_zero = 1;
							 sd_kind = 3;												
					}
				  recv_buf[intko] = SBUF;                //°Ñ´Ó´®¿Ú¶Á³öµÄÊı´æµ½Êı×é
                RI = 0;                 
                if(intko == 6)
                {
                   if(recv_buf[6] == 0x0d)	
											{					
												intko = 0;
												key_zero = 2;                //È·ÈÏÊÕµ½×Ö·û´®
												if(sd_kind == 1&&sys_status==1)//Ä£Ê½ÅĞ¶ÏÈë¿Ú
												{
													sys_res_status = 1;
												}
												if(sd_kind == 2&&sys_status==44)//Ä£Ê½ÅĞ¶ÏÈë¿Ú
												{
													PFID = recv_buf[1];
													WLID = recv_buf[2];
													weightL[0] = recv_buf[3];
													weightL[1] = recv_buf[4];
												  //ÖØÁ¿¸³Öµ
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
//        if(RI == 1)   //µ±Ó²¼ş½ÓÊÕµ½Ò»¸öÊı¾İÊ±£¬RI»áÖÃÎ»
//        {
//                if(SBUF == 0x54)//Ö»ÒªÓöµ½T¾ÍÖÃ0 ´Ó0¿ªÊ¼¼ÆÊı£¬Õâ¸öÎªÁË¶ÁÈ¡¼ÆËã»ú·¢ËÍµÄÎïÁÏÊı¾İ×¼È·
//				{intko = 0;}
//				recv_buf[intko] = SBUF;                //°Ñ´Ó´®¿Ú¶Á³öµÄÊı´æµ½Êı×é
//                RI = 0; 
//                intko++;
//                if(intko == 7)
//                {
//                        intko = 0;                //µ±com_dat = 21Ê±£¬Çå0£¬·ÀÖ¹Êı×éÒç³ö
//
//						if(sys_status==17)//µ±17Ê±£¬½øÈëÏµÍ³Æô¶¯ĞÅºÅ½ÓÊÕ£¬ÆäËûĞÅºÅ²»×ö´¦Àí
//						{
//						  if(recv_buf[0] == 0x4F)//OÈ·ÈÏ¿ª»úÖ¸Áî½ÓÊÕ
//						  {
//						     if(recv_buf[1]== 0x50)//P
//						     {sys_res_status = 31;}					
//						  }												
//						}
//						if((sys_status==1||sys_status==18))//µ±1µÄÊ±ºò£¬×öÎïÁÏIDÎïÁÏÖØÁ¿×Ö´®½ÓÊÕ£¬ÆäËû×´Ì¬²»×ö´¦Àí¡£
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
