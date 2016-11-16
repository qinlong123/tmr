#include "main.h"
#include "LCD12864.h"
#include "stc12c56.h"
#include "pindef.h"
#include "bmp.h"
#include "eeprom.h"

uchar key_zero = 0;
uchar sd_kind = 0;
uchar PF[2];
//�ڴ涨��
/************************************************************/
//#define PF1     0x5A	//�䷽1	����
//#define PF2     0x5C	//�䷽2	����
//#define PF3     0x5E	//�䷽3	����
//#define PF4     0x60	//�䷽4	����
//#define PF5     0x62	//�䷽5	����
//#define PF6     0x64	//�䷽6	����
//#define PF7     0x66	//�䷽7	����
//#define PF8     0x68	//�䷽8	����
//#define PF9     0x6A	//�䷽9	����
//#define PF10    0x6C	//�䷽10����
//#define PF11    0x6E	//�䷽11����

//save
//#define PF1     0x4A	//�䷽1	����
//#define PF2     0x4C	//�䷽2	����
//#define PF3     0x4E	//�䷽3	����
//#define PF4     0x40	//�䷽4	����
//#define PF5     0x42	//�䷽5	����
//#define PF6     0x44	//�䷽6	����
//#define PF7     0x46	//�䷽7	����
//#define PF8     0x48	//�䷽8	����
//#define PF9     0x4A	//�䷽9	����
//#define PF10    0x4C	//�䷽10����
//#define PF11    0x4E	//�䷽11����

//PFID-> WLSL(��������)
//HBIT 0x46

//����������
//0x01,0x02 ����1
//0x03,0x04 ����1
//0x05,0x06 ����1
//0x07,0x08 ����1
//0x09,0x0a ����1
//0x0b,0x0c ����1
//0x0d,0x0e ����1
//0x0f,0x10 ����1
//0x11,0x12 ����1
//0x13,0x14 ����1

/************************************************************/
/****������****/
//644
//������ʾ�����ַ�����1��ͨ��ʶ��CLOCK��Ƶ�ʽ���λ�����л� 2��ͨ������LED����оƬ�ĸ�λ�źš�
////////ǰ4λ
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

//////////��1λ
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
//����ģ���ʼ��//////////////////////////////////////
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
 //IE=0; //�����ж�

 }
//���ͷ���////////////���ֽڷ��ͷ����в����������ã�/////////////////////////
void send(unsigned char text)
 {
  ES=0;
  SBUF=text+0x30;
  while(!TI); 
  TI=0;
  ES=1;
 }
//���ͷ���///////////���ֽ�/////������ʹ��///////////////////////////////////
void send_usual(unsigned char text)//��������
 {
  ES=0;
  SBUF=text;
  while(!TI); 
  TI=0;
  ES=1;
 }
//�����յ������ݱ��INTֵ
void recv_data()
{
sys_press = byte_read(0x58+(PFID*2),(WLID*2))*256+byte_read(0x58+(PFID*2),(WLID*2)+1);
// sys_kinds = (recv_buf[6]-0x30)+(recv_buf[5]-0x30)*10;

   if(sys_press > sys_press_temp)//����������������ϴ��趨���������ֵΪ����
    {
      addorarr = 0;
      addorarr_count = sys_press - sys_press_temp;
      sys_press_temp = sys_press;
    }
   if(sys_press < sys_press_temp)//����������������ϴ��趨���������ֵΪ����
    {
   addorarr = 1;
   addorarr_count = sys_press_temp - sys_press;
   sys_press_temp = sys_press;
    }
//if(sys_press == sys_press_temp)
//{addorarr = 2;}

//    sys_res_status = 1;//�Ѿ�ȷ�Ͻ��յ�����
    delayL(20000);
    delayL(20000);
    delayL(20000);
    delayL(20000);

	}
//���շ���//////////�����ж�////////////////////////////////////////////////

//���巢��ģ�飨�����ã�///////////////////////////////////////////////
void send_data(dis_V)
{
	uchar temp,i,sy,j;
	for(j=0;j<15;j++)
	{delayL(20000);}
//0xD1 ����ʱ��ͷ������������
	//send_usual(0x44);
	//send_usual(0x31);		
//���ڷ��� 
 	//send_usual(0x32);
	//send_usual(0x30);
	//send_usual(0x31);
	//send_usual(0x33);
	//send_usual(0x30);
	//send_usual(0x32);
	//send_usual(0x32);
	//send_usual(0x31);
//0xD2 ����ʱ��ͷ������������
	//send_usual(0x44);
	//send_usual(0x32);
//ʱ�䣬ID����
	//send_usual(0x31);
	//send_usual(0x34);
	//send_usual(0x31);
	//send_usual(0x33);
//ID
    //send_usual(0x30);
	//send_usual(0x32);
	//send_usual(0x33);
//�䷽����ID����
send_usual(0x4b);

				  sy = sys_subjects;
				  
				  sy=sy/10;
		          send_usual((sy % 10)+'0');
		          
				  sy = sys_subjects;
		          send_usual((sy % 10)+'0');
//��������
send_usual(0x4d);

	 			  sy = sys_kind_temp;
				  
				  sy=sy/10;
		          send_usual((sy % 10)+'0');
		          
				  sy = sys_kind_temp;
		          send_usual((sy % 10)+'0');
		          



//��������

/*//������ʶ��ͷ
	send_usual(0x41);
	send_usual(0x31);
//������ʶ����
   	for(i = 0; i < 3; i++)
	{
		temp = KEY_BUF % 10;
		send(temp);
		KEY_BUF=KEY_BUF/10;
	}
*/
//������ʶ��ͷ
	send_usual(0x41);
	send_usual(0x32);
//����Ϊ����ֵ ///////////////////////////////////////////////////////////////-------------------------------write---------
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

    //send(-0x26);//���з�
}
//���������������һ���γ�16������////////////////////////////////////////////
void Code2char()
{
uchar i;
//��һ������
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
//�ڶ�������
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

//����������
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
//����������
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
//����������
if(rev_1[4] == 182)
{//cvover = cvover+(1*10000); ����ԭ����λ��Ҫ�ˣ��°汾�и���
diswich++;
}
if((diswich == 4)&&(rev_1[4]==0))
{diswich++;}


if(diswich == 5)
 {
   //dis_data_led(cvover);
   send_data(cvover);
 }

//һ������֮�������������
diswich = 0;
cvover=0;
fu_hao = -0x05;
ZERO_SW = 0;

}
//ץȡ�ź�������/////////////////////////////////////////////////////
void ReadPS2()
{
   uchar i;
   while(PS_DATA == 1)
   {
     while(PS_CLK==0);
   	 while(PS_CLK==1);																					 
	 while(!PS_CLK); 	 
   }
   for(i=0;i<8;i++)	 //��һ����
   {
   while(PS_CLK);
   while(!PS_CLK);	
	if(PS_DATA == 1)
	 {
	   rev_1[0]++;
	 }
	 rev_1[0] = rev_1[0] << 1;	   
   }   
   for(i=0;i<8;i++)	 //�ڶ�����
   {
    while(PS_CLK);
    while(!PS_CLK);	
	if(PS_DATA == 1)
	 {
	   rev_1[1]++;
	 }
	 rev_1[1]=rev_1[1]<<1;
    }
   for(i=0;i<8;i++)	 //��������
   {
    while(PS_CLK);
    while(!PS_CLK);	
	if(PS_DATA == 1)
	 {
	   rev_1[2]++;
	 }
	 rev_1[2]=rev_1[2]<<1;
    }
   for(i=0;i<8;i++)	 //���Ķ���
   {
    while(PS_CLK);
    while(!PS_CLK);	
	if(PS_DATA == 1)
	 {
	   rev_1[3]++;
	 }
	 rev_1[3]=rev_1[3]<<1;
    }	   
   for(i=0;i<3;i++)	 //�������
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
     {i++;}//��ʱ������Ϊ�͵�ƽ��ʱ��i��������
	 else{i=0;}//��ʱ������Ϊ�ߵ�ƽ��ʱ��i����
	 if(i>300)
	 {sw_clkchk = 1;}//��i���������ʱ��֤��ʱ���߷����жϣ���ʱ�����Զ�ת����ʾ�������ɴ˴򿪡�
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
   {delayL(10000);}//�˴�֮ǰΪ�����趨ģʽ
   ARR_KEY=1;
   ADD_KEY=1;
   for(i=0;i<55;i++)
   {delayL(20000);}//�˴�֮ǰΪ�����趨ģʽ
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
   ADD_KEY=0;//��ס
   delayL(8500);
   ADD_KEY=1;//�Ϳ�
   delayL(6000);     
   }
 }
if(addorarr == 1)
 {
   for(j=0;j<addorarr_count;j++)
   {
   delayL(6000);
   ARR_KEY=0;//��ס
   delayL(8500);
   ARR_KEY=1;//�Ϳ�
   delayL(6000);  
   }
 }
   //�˴�Ӧ�����趨�����
   sys_res_status=20;  //��ʱϵͳ�������޵ȴ�״̬�������κη���

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
//��ʼ��
//�����η���
PS_CLK = 1;//�չ�һ���ߵ�ƽ
delayL(1000);//�ߵ�ƽ͹�𲿷�
PS_CLK = 0;//ʱ�Ӹߵ�ƽ����
delayL(500);//�����������
PS_DATA = 1;//��ǰ������ڸ�START�ź�
delayL(500);//����������ڣ�ʱ���źŵ�ʣ�������ڣ�
PS_CLK = 1;//ʱ�ӵڶ������ڿ�ʼ�������ź�׼������START��
delayL(1000);//����һ�����ڳ����ߵ�ƽ
PS_CLK = 0;//���͵�ƽʱ���־���һ������
delayL(500);//����������ڣ�ʱ���źŵ�ʣ�������ڣ�
PS_DATA = 0;//�����źŵ͵�ƽ��START�������
delayL(500);

 for(i=0;i<8;i++)
 {
 if(bit0o1(dis[0],i)==1)
 {
 PS_DATA = 1;//���ݽ���㣨��һ�飩
 }
 else
 {PS_DATA = 0;}
 PS_CLK = 1;//����ʱ��
 delayL(1000);//����һ���������� 
 PS_CLK = 0;//����ʱ��
 delayL(500);//���������������ʱ��
 PS_DATA = 0;//���������ͺ�
 delayL(500);//�ȴ�������ھ�����8λ����
 }
  for(i=0;i<8;i++)
 {
 if(bit0o1(dis[1],i)==1)
 {
 PS_DATA = 1;//���ݽ���㣨��һ�飩
 }
 else
 {PS_DATA = 0;}//���ݽ���㣨�ڶ��飩
 PS_CLK = 1;
 delayL(1000);
 PS_CLK = 0;
 delayL(500);
 PS_DATA = 0;
 delayL(500);//�ȴ�������ھ�����8λ����
 }

  for(i=0;i<8;i++)
 {
 if(bit0o1(dis[2],i)==1)
 {
 PS_DATA = 1;//���ݽ���㣨��һ�飩
 }
 else
 {PS_DATA = 0;}//���ݽ���㣨�����飩
 PS_CLK = 1;
 delayL(1000);
 PS_CLK = 0;
 delayL(500);
 PS_DATA = 0;
 delayL(500);//�ȴ�������ھ�����8λ����
 }


  for(i=0;i<8;i++)
 {
 if(bit0o1(dis[3],i)==1)
 {
 PS_DATA = 1;//���ݽ���㣨��һ�飩
 }
 else
 {PS_DATA = 0;}//���ݽ���㣨�����飩
 PS_CLK = 1;
 delayL(1000);
 PS_CLK = 0;
 delayL(500);
 PS_DATA = 0;
  delayL(500);//�ȴ�������ھ�����8λ����
 }

//33
 PS_DATA = 0;//���ݽ����33
 PS_CLK = 1;
 delayL(1000);
 PS_CLK = 0;
 delayL(500);
 PS_DATA = 0;
  delayL(500);//�ȴ�������ھ�����8λ����
//34
 PS_DATA = 0;//���ݽ����34
 PS_CLK = 1;
 delayL(1000);
 PS_CLK = 0;
 delayL(500);
 PS_DATA = 0;
  delayL(500);//�ȴ�������ھ�����8λ����
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
PS_CLK = 1;	//���仰һ�����ϰ����������ˡ�
PS_DATA = 1; 
//������Ҫ��˼��Ҫ��λоƬ�ߵ�ƽ�������ܽŲŻ�����������
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
		//sys_kinds ����ID

		u1 = sys_kinds;
		temp = u1%10;
		dis[0] = dis_data[temp];
		u1=u1/10;
		temp = u1%10;
		dis[1] = dis_data[temp];


		u2 = sys_subjects;
		//sys_subjects ����ID
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
	}	//�˴�Ӧ��׷�ӵ��ǽ����趨ģʽ������趨ģʽ����ֵ��֮���趨��ֵ���㡣�����㶯����Ҫ��ZERO������3000
    //addorarr_count=3000;
	//addorarr = 1;
	Set_press_int();
	ARR_KEY = 0;
	for(i=0;i<100;i++)//�˴���ʱ�Ѿ�׷��
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

	beep3();//������ʾ
}
void start_system()
{
  	clear_system(); //ϵͳһ������
//	send_usual(0x53);//ϵͳ׼����ϣ�ϵͳ���ߡ�
//	send_usual(0x45);//���ͽ�ֹ��
}

 
//���������///////////////////////////////////////////////////////////////---------------------------------------------------------MAIN
void main(void)
{
uint sy = 0;
  if(sys_status==0)//ϵͳ״̬Ϊ0ʱ���Զ���ʼ��ϵͳ
	{
	init();		//��ʼ������ /uint i=0;		
	//��ʼ���ն�0
	sys_status=77;//�ȴ���������	
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
			  sys_status=17;//���뿪����ѯ״̬;//����ϵͳ��ʼ��״̬��ϵͳ����
			 }//�ж����������Ƿ���
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
					sys_status=66;//����ϵͳ������Ŀ
		      sys_res_status=0;//���ձ�ʶ��λ?
				}
			}
	   if(sys_status==66)/////////////////////////////////////////////////////��Ļ�л�
	   {
	   	 clk_test();
		   if(sw_clkchk == 1)
		   {//SW_KEY = 0;
		      SW_LEDP=0;
		      sys_status=4;//����ϵͳ������Ŀ
		      sys_res_status=0;//���ձ�ʶ��λ��
		      sw_clkchk = 0;
          PS_DATA = 0;
          PS_CLK = 0;
		      delayL(20000);
		      PS_DATA = 1;
          PS_CLK = 1;
		   }
	   }
	   if(sys_status==4)//ѡ���趨�䷽ID
	   {
	     //EA = 0;//�ر����ж�
		 //ǰ���ж���ʾ����Դһ�Σ���ȷ�����������		 
		 //�˴�ִֻ��һ�Σ��������ƻᷢ��Ƶ����
		 delayL(20000);
		 delayL(20000);
		 delayL(20000);
		 delayL(20000);
		 clk_test(); //////////////////////////////////////////////////////////////////��ʾ�л�
		 SW_LED=0;//��ʱ�л���ʾ��		 ���л��ٷ���
		 sw_clkchk = 0;

		 if(sw_one == 0)
		 {
		 SW_LEDP=0;
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
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

		 if(PRINT_KEY==0) //��ȷ�ϰ���������䷽����ģʽ
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
			//EA = 1;//�������ж�
		 }
	   }

	  if(sys_status==1)//��ѡ���䷽���ݽ��մ���
	   {		 
		 //������ѡ����䷽����
		 if(sys_res_status==0)//���δ���յ�����������һֱ����
		 {
		   
		   if(tick == 0)
			{	  
			      //intko=0;
			    send_usual(0x01);//����ͨѶ��ʶ
		   //send(sys_subjects);//�����䷽ID ASCII //�˴��任����λ��������������
		   //��ѯ��������
				  send_usual(0x01);
				  send_usual(0x04);
				  send_usual(0x04);
				  send_usual(0x04);
				  send_usual(0x04);
				  send_usual(0x0d);				  
				  //���ͽ�ֹ��
		   	}
		   if(tick == 1)
		   {
		     intko=0;
			 if(sys_res_status==0)
		     {send_usual(0x4a);}//�任��J
			 {send_usual(0x45);}//�任��E
			 {send_usual(0x45);}//�任��E
			 {send_usual(0x45);}//�任��E
			 {send_usual(0x45);}//�任��E
			 {send_usual(0x45);}//�任��E
			 {send_usual(0x45);}//�任��E
			 {send_usual(0x45);}//�任��E
		   }
		   		  
		   //send_usual(0x0A);//���з�
		   delayL(20000);//��ʱ�������ȴ����������
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
		   		   delayL(20000);//��ʱ�������ȴ����������
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
		   		   delayL(20000);//��ʱ�������ȴ����������
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
		   		   delayL(20000);//��ʱ�������ȴ����������
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


		 if(sys_res_status==1)//���յ�����֮������������ݽ��մ���
			{
			  SW_LED=0;
			  dis[3] = 0x40;
			  dis[0] = 0x40;
			  dis[2] = 0x3F;
			  dis[1] = 0x38;
			  Display_LED();
			  //������ջ�����
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
		 if(sys_res_status==44&&key_zero == 2)//������ѵ���մ���״̬ //EEPRROM xieru
			{
			   	 //��ӦEEPROM�洢
				WLID = recv_buf[2]; 
				PFID = recv_buf[1]; 
				
				
				if(WLID == 0x01)//�������IDΪ��һλ��ʱ������ִ���������㶯��
				 {
				 	SectorErase(0x58+(PFID*2),0x00);//д��ǰҪ�����
					 delayL(20000);
					 delayL(20000);
					byte_write(recv_buf[3],0x58+(PFID*2),(WLID*2));//����EEPROM	��λ
				    delayL(20000);		            
					byte_write(recv_buf[4],0x58+(PFID*2),(WLID*2)+1);//����EEPROM	��λ
					delayL(20000);
					 //д���Ӧ�䷽��������

          //byte_read(0x58+(PFID*2),(WLID*2))					 
				 }
			   	 else
				 {
				 	byte_write(recv_buf[3],0x58+(PFID*2),(WLID*2));//����EEPROM	��λ
					delayL(20000);
					byte_write(recv_buf[4],0x58+(PFID*2),(WLID*2)+1);//����EEPROM	��λ
					delayL(20000);			 
				 }
				 delayL(20000);
				 delayL(20000);
				 delayL(20000);
				 delayL(20000);
				 //���ͽ��
				  send_usual(recv_buf[0]);
				  send_usual(recv_buf[1]);
				  send_usual(recv_buf[2]);
 				  send_usual(byte_read(0x58+(PFID*2),(WLID*2)));
 				  send_usual(byte_read(0x58+(PFID*2),(WLID*2)+1));
// 				 	send_usual(recv_buf[3]);
// 				  send_usual(recv_buf[4]);
				  send_usual(recv_buf[5]);
				  send_usual(recv_buf[6]);
				  if(recv_buf[5] == 0xFA)//��������Ѿ���������˳�������״̬
					{							
					  sys_status=66;
					}
					else
					{
            PF[0] = recv_buf[5];
						byte_write(0x46,recv_buf[5],PFID);//����EEPROM	��λ
          }


				  //������ջ�����
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
		    ES=0;//��ֹ�����ж�///////////////////////////////////////////////////////////���ж� 
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
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 SW_LEDP=1;
		 delayL(2000);
		 sw_one++;
		 }
			  SW_LED=0;//��ʱ�л���ʾ����
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
			  DOT_LED = 0;//��ʾС�����
			  			 
			  //�˴���ʾ�����������࣬����ID��ȷ��
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
		  if(sys_res_status == 3)//ȷ��ID��	//////////////////////////////////////////////�˴������趨λ�á�
			 {			 
			 delayL(20000);
			 delayL(20000);
			 delayL(20000);
			 delayL(20000);
			 delayL(20000);
			 delayL(20000);
			 clk_test(); //////////////////////////////////////////////////////////////////��ʾ�л�
			 SW_LED=1;//��ʱ�л���ʾ��
			 sw_clkchk = 0;
         SW_LEDP=0;
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 //SW_LEDP=1;
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ��
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 SW_LEDP=0;
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 SW_LEDP=1;
		 delayL(2000);
			 delayL(20000);
			 delayL(20000);
			 delayL(20000);
			 delayL(20000);
		     Set_press_int();//�趨����
			 sys_status=9; //����ȷ�ϱ�ʶȷ���趨����������ʼ����
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
			//sys_res_status = 0;//�����䷽�ȴ�����
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
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);
		 delayL(20000);
		 delayL(20000);
		 		 delayL(20000);
		 delayL(20000);
		 delayL(20000);
		 		 delayL(20000);
		 delayL(20000);
		 delayL(20000);
		  clk_test(); //////////////////////////////////////////////////////////////////��ʾ�л�
			 SW_LED=1;//��ʱ�л���ʾ��
			 sw_clkchk = 0;
		 			//SW_LED=1;//�л�����ʾ��������ѭ��
		 SW_LEDP=1;
		 delayL(2000);
		 sw_one++;
		 }
			ES=0;//��ֹ�����ж�///////////////////////////////////////////////////////////���ж� 
			delayL(20000);
		 }

		 if(sys_status == 9)
		 {
			   delayL(20000);
			   delayL(20000);
			   //send_usual(0x4f);//������ʾ��
			   delayL(40000);//Zero������ʼ������������ģʽ
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
		 {//���մ���
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
	     if(sys_status==2)//���ݼ�ش���
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
			//�ڴ˴�׷���Զ�����J�Ľ�������״̬�����������ϵͳ����ʱ����ֵ���Ա��´μ�����ȷ���ϡ�
			//if(fu_hao == -0x05)//��ʰȡ���ֲ�Ϊ0�͸�����ʱ��OKID�������㡣
			//{ok_id=0;}
			//if(cvover!=0)
			//{ok_id=0;}
			if(ok_id ==0 && ok_id2 == 0)//����ط�������
			{time0_tick = 0;}
      if(ok_id == 1||ok_id2 == 1)//��OK_ID����0��ʱ��֤��ϵͳ�Ѿ�һ�ν���0��״̬��֤�������Ѿ���������ߣ���������Ҫ���봦����ơ�
			{
			  time0_tick++;//���յ���ʶ�Զ����������Ա���ʱԽ��ϵͳ����TOTAL�Զ�����J
			  //delayL(20000);
			  //delayL(20000);
			  //delayL(20000);
			  //delayL(20000);
			  //delayL(20000);
			  //if(ok_id>2) //��ʱ���أ���ʱ���ٴκ����ִ�����ղ���
			  //{
			  if(time0_tick>4)
			   {
			     time0_tick=0;
			  	 ok_id=0;//�����ʱ�����ﵽ��ʱ��Ҫ��λOK_ID�Ա��´ν��롣
				   ok_id2=0;
				 //sys_press_temp=0;//ϵͳTEMP������������	  ����ԭ���°汾����Ҫ����
	if(fu_hao==-0x03)
	{cvover = cvover+(byte_read(0x58+(PFID*2),(WLID*2)))*256+byte_read(0x58+(PFID*2),(WLID*2)+1);}
	else{cvover = (byte_read(0x58+(PFID*2),(WLID*2)))*256+byte_read(0x58+(PFID*2),(WLID*2)+1)-cvover;}
//	send_usual(0x45);//���ͽ�ֹ��

				if(WLID == 0x01)//�������IDΪ��һλ��ʱ������ִ���������㶯��
				 {
				 	SectorErase(0x48+(PFID*2),0x00);//д��ǰҪ����?
					 delayL(20000);
					 delayL(20000);
					byte_write((cvover/256),0x48+(PFID*2),(WLID*2));//����EEPROM	��λ
				   delayL(20000);		            
					byte_write((cvover%256),0x48+(PFID*2),(WLID*2)+1);//����EEPROM	��λ
					delayL(20000);

          //byte_read(0x58+(PFID*2),(WLID*2))					 
				 }
			   	 else
				 {
				 	byte_write((cvover/256),0x48+(PFID*2),(WLID*2));//����EEPROM	��λ
					delayL(20000);
					byte_write((cvover%256),0x48+(PFID*2),(WLID*2)+1);//����EEPROM	��λ
					delayL(20000);			 
				 }
				 delayL(20000);
				 delayL(20000);
				 delayL(20000);
				 delayL(20000);
				 //�Զ�����J���뷢��״̬��SYS_STATUS.
				 //�˴�Ϊ��ʡ�¶�������һ�°���״̬
				 sys_res_status=0;
				 sys_status=1;
				 delayL(20000);
			     delayL(20000);
			     delayL(20000);
			     delayL(20000);
			     delayL(20000);
				 		 SW_LEDP=0;
		  clk_test(); //////////////////////////////////////////////////////////////////��ʾ�л�
			 SW_LED=0;//��ʱ�л���ʾ��
			 sw_clkchk = 0;
		 //SW_LED=0;
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 SW_LEDP=1;
				 
				 
				 			     
		 /*SW_LEDP=0;		 
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		  clk_test(); //////////////////////////////////////////////////////////////////��ʾ�л�
			 SW_LED=0;//��ʱ�л���ʾ��
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
				ES=1;//���������ж�//////////////////////////////////////////////////////////////���ж�
				 //�˴�Ӧ��ʾȷ�ϻ���
				delayL(20000);
				//���������
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
		   if(PRINT_KEY==0&&ADD_KEY==0) //��ȷ�ϰ���������䷽����ģʽ
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
		  clk_test(); //////////////////////////////////////////////////////////////////��ʾ�л�
			 SW_LED=0;//��ʱ�л���ʾ��
			 sw_clkchk = 0;
		 //SW_LED=0;
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
		 delayL(20000);//�ر���ʱʱ�䡣
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
				 //�˴�Ӧ��ʾȷ�ϻ���
				delayL(20000);
				//���������
				delayL(20000);
				delayL(20000);
				delayL(20000);
				delayL(20000);
				delayL(20000);
				delayL(20000);
				delayL(20000);
				//��TOTAL������
				//TOTAL_KEY = ;	
			   //SW_KEY = 0;//�رռ���
			   delayL(20000);//Zero������ʼ������������ģʽ
			   delayL(20000);
			   delayL(20000);//Zero������ʼ������������ģʽ
			   delayL(20000);
			   delayL(20000);//Zero������ʼ������������ģʽ
			   delayL(20000);
		       delayL(20000);//Zero������ʼ������������ģʽ
			   delayL(20000);
			   //while(PRINT_KEY==0){;}
			   //SW_KEY = 0;//�򿪼���
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
			   ES=1;//���������ж�//////////////////////////////////////////////////////////////���ж�		 
		   }
	      }

	}
}
//��ʼ���ܺ���////////////////////////////////////////////////////////////
void init()
{
	init_CPU();		//��ʼ��CPU����
	LCD_init();		//LCD��ʼ������
	init_uart();
	P26=0;
}

//CPU��ʼ������///////////////////////////////////////////////////////////
void init_CPU()
{
	EA = 1;			//��CPU�ж�
	CLK_DIV = 0x00;	//���񲻷�Ƶ 20MĬ��
	//P1M0=1;
	//P1M1=0;
	//init_TIMER();//��ʱ����ʼ����
}

//��ʱ����/////////��ʱʱ���Լ�����///////////////////////////////////////
void delayL(uint n)
{
	uchar i;
	while(n>0)
	{
		for(i=10;i>2;i--);
		n--;
	}	
}

//��ʱ����ʼ��������δʹ�ã�/////////////////////////////////////////////
void init_TIMER()
{
EA = 0;
TR0 = 0;
TMOD = 0x01;
	TH0 = (65536-10000)/256;				//��ʱ��0�Ĵ�����ֵ��8λ
	TL0 = (65536-10000)%256;				//��ʱ��0�Ĵ�����ֵ��8λ
	PT0 = 1;
	ET0 = 1;
	EA = 1;	
}


/****��ʼ������end****/
void Serial_Int(void) interrupt 4 using 1
{
        ES = 0;
        if(RI == 1)   //��Ӳ�����յ�һ������ʱ��RI����λ
        {
          if(SBUF == 0x0A&&intko == 0)//ֻҪ����T����0 ��0��ʼ���������Ϊ�˶�ȡ��������͵���������׼ȷ
					{
							 key_zero = 1;
							 sd_kind = 2;								
					}
					if(SBUF == 0x01&&intko == 0)//ֻҪ����T����0 ��0��ʼ���������Ϊ�˶�ȡ��������͵���������׼ȷ
					{
							 key_zero = 1;
							 sd_kind = 1;												
					}
					if(SBUF == 0x03&&intko == 0)//ֻҪ����T����0 ��0��ʼ���������Ϊ�˶�ȡ��������͵���������׼ȷ
					{
							 key_zero = 1;
							 sd_kind = 3;												
					}
				  recv_buf[intko] = SBUF;                //�ѴӴ��ڶ��������浽����
                RI = 0;                 
                if(intko == 6)
                {
                   if(recv_buf[6] == 0x0d)	
											{					
												intko = 0;
												key_zero = 2;                //ȷ���յ��ַ���
												if(sd_kind == 1&&sys_status==1)//ģʽ�ж����
												{
													sys_res_status = 1;
												}
												if(sd_kind == 2&&sys_status==44)//ģʽ�ж����
												{
													PFID = recv_buf[1];
													WLID = recv_buf[2];
													weightL[0] = recv_buf[3];
													weightL[1] = recv_buf[4];
												  //������ֵ
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
//        if(RI == 1)   //��Ӳ�����յ�һ������ʱ��RI����λ
//        {
//                if(SBUF == 0x54)//ֻҪ����T����0 ��0��ʼ���������Ϊ�˶�ȡ��������͵���������׼ȷ
//				{intko = 0;}
//				recv_buf[intko] = SBUF;                //�ѴӴ��ڶ��������浽����
//                RI = 0; 
//                intko++;
//                if(intko == 7)
//                {
//                        intko = 0;                //��com_dat = 21ʱ����0����ֹ�������
//
//						if(sys_status==17)//��17ʱ������ϵͳ�����źŽ��գ������źŲ�������
//						{
//						  if(recv_buf[0] == 0x4F)//Oȷ�Ͽ���ָ�����
//						  {
//						     if(recv_buf[1]== 0x50)//P
//						     {sys_res_status = 31;}					
//						  }												
//						}
//						if((sys_status==1||sys_status==18))//��1��ʱ��������ID���������ִ����գ�����״̬��������
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
