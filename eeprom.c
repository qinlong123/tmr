#include "eeprom.H"
#include "stc12c56.h"
#include "INTRINS.H"

#define uint unsigned int
#define uchar unsigned char
#define ulong unsigned long
//��������

/*EEPROM������(�����޸�)*/
//eeprom------------------------------------------
#define RdCommand 0x01 //����ISP�Ĳ�������
#define PrgCommand 0x02
#define EraseCommand 0x03 
#define Error 1
#define Ok 0
#define WaitTime 0x03 //����CPU�ĵȴ�ʱ��

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
/*�� ISP,IAP ����*/
void ISP_IAP_enable(void)
{
	 EA = 0;       //���ж�   
	 ISP_CONTR = ISP_CONTR & 0x18;		//0001,1000 
	 ISP_CONTR = ISP_CONTR | WaitTime;	//д��Ӳ����ʱ 
	 ISP_CONTR = ISP_CONTR | 0x80;      //ISPEN=1  
}

/*�ر� ISP,IAP ����*/
void ISP_IAP_disable(void)
{
	 ISP_CONTR = ISP_CONTR & 0x7f; //ISPEN = 0 
	 ISP_TRIG = 0x00;
	 EA   =   1;   //���ж� 
}

/*����ISP*/
void ISPgoon(void)
{
	 ISP_IAP_enable();	//�� ISP,IAP ���� 
	 ISP_TRIG = 0x46;	//����ISP_IAP�����ֽ�1 
	 ISP_TRIG = 0xb9;	//����ISP_IAP�����ֽ�2 
 	 _nop_();
}

/*�ֽڶ�*/
unsigned char byte_read(uchar high_add,uchar low_add)
{
	 ISP_ADDRH = high_add;	//��ַ��ֵ 
	 ISP_ADDRL = low_add;
	 ISP_CMD   = ISP_CMD & 0xf8;		//�����3λ  
	 ISP_CMD   = ISP_CMD | RdCommand;	//д������� 
	 ISPgoon();       		//����ִ��  
	 ISP_IAP_disable();    	//�ر�ISP,IAP���� 
	 return (ISP_DATA);    	//���ض���������
}

/*��������*/
void SectorErase(uchar high_add,uchar low_add)
{
	 ISP_ADDRH = high_add;	//��ַ��ֵ
	 ISP_ADDRL = low_add;
	 ISP_CMD = ISP_CMD & 0xf8;   		//��յ�3λ
	 ISP_CMD = ISP_CMD | EraseCommand; 	//��������3
	 ISPgoon();       					//����ִ��
	 _nop_();
	 ISP_IAP_disable();    				//�ر�ISP,IAP����
}

/*�ֽ�д*/
void byte_write(uchar str, uchar high_add,uchar low_add)
{
	 ISP_ADDRH = high_add;	//��ַ��ֵ
	 ISP_ADDRL = low_add;
	 ISP_CMD  = ISP_CMD & 0xf8;    		//���3λ
	 ISP_CMD  = ISP_CMD | PrgCommand;  	//д����2
	 ISP_DATA = str;   					//д������׼��
	 ISPgoon();       					//����ִ��
	 _nop_();
	 ISP_IAP_disable();     			//�ر�IAP����
}

/*EEPROM������end(�����޸�)*/
/**************************************************/
/***************************************************************/

/****EEPROM���ֵ������***/


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
//SectorErase(XQ4,0x00);//д��ǰҪ������
/*-----------------------------------------------28------------------------
read_uSvhr_max: �����ֵ	<06.1.3>			
�����������
����ֵ	���� 
����޸����ڣ�2012-02-17 
�޸���	��skyman
����	�������ֵ	
-----------------------------------------------------------------------*/
unsigned char read_uSvhr_max(uchar b1 ,uchar k, uchar count)//��������count �±�	 b1���� ��ַ k ����ʱ���
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

