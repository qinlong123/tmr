#include <string.h>

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long

/*************************************************

*****          ����������          ***************

*************************************************/

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

/*��ʼ������*/
void init();		//��ʼ������	<00>
void init_CPU();	//CPU��ʼ��		<00.1>
void init_TIMER();	//��ʱ����ʼ��	<00.2>
void init_AD();		//ADת����ʼ��	<00.3>
void delayL(uint n);	//��ʱ����	<00.3.1>
/*��ʼ������end*/

/*ÿ�붯������*/





uint sys_kind_temp = 1;
uchar  rev_1[7] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uchar  weightL[2] = {0x00,0x00};
uchar  PFID = 0;
uchar  WLID = 0;
uchar  RWSW = 0;
//uchar  BitCounter = 8;
uchar  ok_id = 0;//���ſ���
uchar  ok_id2 = 0;//0����

uint time0_tick = 0; //��������,�˴�������Ϊ��ʱʹ��
uchar tick = 0;	//�������
uchar count[4] = {0,0,0,0};
//bit flag = 1;  //���㿪ʼ����
uint sw_clkchk = 0;
uint sw_one = 0; //ִ��һ�ι�ܿ���

uchar int0sw = 0; //Ѱַ���ȡ���ݿ���  0ΪѰַ 1Ϊ��ȡ����
//bit tim8 = 0; //Ѱַ��ʶ
//uint counter1 = 32;
//uint counter2 = 0;
uint cvover = 0;
uchar convertsw[] = {252,96,218,242,102,182,190,224,254,246,2};
uchar diswich = 0; //�����ݴ���
uchar fu_hao = -0x05;
uchar ZERO_SW = 0;
uchar sys_status = 0;//ϵͳ״̬��0 ��ʼ��״̬ 1 �ȴ����ݽ��� 2���ݼ��״̬ 3�����ȴ�״̬��ѡ���䷽���ࣩ	4�����ȴ�״̬��PRINT��
uchar sys_subjects = 0;//�䷽���ࣻ��0~19
uchar sys_kinds = 1;//��������:��0~30
uint sys_press = 0;//��Ӧ�����������������
uchar sys_res_status = 0;//���ݽ���״̬ 0 δ���յ� 1 �ѽ��յ�
uint sys_press_temp = 0;//��Ӧ�ϴ����ϵ��趨������Ϊ���´ο��ټ���
uchar addorarr = 0;//���������趨�ķ��ţ�0Ϊ�� 1Ϊ��
uint addorarr_count = 0;//���ӻ��߼��ٵ���ֵ
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
//G->��ͬ��9
//- ->0x40

uint intko = 0;


/*************************************************

*****          ����������END          ************

**************************************************/