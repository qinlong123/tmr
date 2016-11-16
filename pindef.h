#include	<stc12c56.h>

//��ʱ��0��ʼֵ
#define T0_H	0xcf
#define T0_L	0x2c
//EEPROM��ַ
#define PERIOD  		20	//ÿ��������λ��1/��ʱ������ʱ�� ��
/*************************/
/*12864���Ŷ���*/

sbit di		= P2^4;			//����ģʽ ʱ�Ӷ�				   7
sbit scl 	= P2^2;			//SI  ����ģʽ���ݶ�			   8
sbit cs 	= P2^1;			//оƬѡͨ�ϣ��͵�ƽ��Ч		   12
sbit rst 	= P2^0;			//��λ�źţ��͵�ƽ��Ч		   13
sbit a0 	= P2^3;			//��������ѡ��  H ����  L ����   14
sbit pin_bg = P2^6;
sbit PS_DATA = P1^0;
sbit PS_CLK = P1^2;

//�����ܽŶ���
sbit PRINT_KEY = P1^5;
sbit ADD_KEY = P1^4;
sbit ARR_KEY = P1^3;
sbit ZERO_KEY = P1^6;
sbit TOTAL_KEY = P1^7;
sbit SW_LED = P3^4;
sbit SW_KEY = P2^5;
sbit SW_LEDP = P3^5; //��ʾ����Դ����

//CLOCK�ܽŶ���
sbit CHK_CLK = P2^7;
sbit DOT_LED = P3^7;

/*12864���Ŷ���end*/
/*************************/