#include <stc12c56.h>
#include "bmp.h"
#include "lcd12864.h"
#include "bmp_model.h"
/**************************************/
/*��ʾ����*/

//void show_Poweron()
//{
//	LCD_clear();
//	LCD_write2(30,1,dianyuan_on,60);	//��ʾ����ԴON��	
//	LCD_write2(7,4,ceding,112);			//��ʾ���y���򰲶������뤿��ˡ�
//	LCD_write1(7,6,ceding2,112);		//��ʾ��30��ۤɤ���������������
//}
/*void dis_ak()
{
LCD_writek(0,0,AK,8);
}
void dis_bk()
{
LCD_writek(0,0,BK,8);
}

void dis_counter_zong()
{
	//LCD_write1(100,2,zong,1);
}*/
/*
void dis_counter_heng()
{
	LCD_write3(11,2,heng100,89);
	LCD_write3(11,3,heng80,89);
	LCD_write3(11,5,heng60,89);
	LCD_write3(11,6,heng40,89);
}
*/   


/*-----------------------------------------------------------------------
digit_M: ���������̶ȱ�ʾ		<03.1>
���������ulong uSvhr
����ֵ	���� 
����޸����ڣ�2011-11-19 	 
�޸���	��skyman
����	����ʾ���������ֵ����ʾλ�ã�(X_uSvhr[],2)
----------------------------------------------------------------------*/
/*void dis_counter_uSvhr(dis_V)
{
	uchar temp,i,k,b;
			
   	for(i = 0; i < 3; i++)
	{
		temp = dis_V % 10;
		LCD_digit_M(X_fenshu[i],0,temp+'0');
		dis_V=dis_V/10;
	}

					
}*/
/*void dis_counter_uSvhr2(dis_V)
{
	uchar temp,i,k,b;
			
   	for(i = 0; i < 3; i++)
	{
		temp = dis_V % 10;
		LCD_digit_M(X_fenshu[i],2,temp+'0');
		dis_V=dis_V/10;
	}

					
}
void dis_counter_uSvhr3(dis_V)
{
	uchar temp,i,k,b;
			
   	for(i = 0; i < 3; i++)
	{
		temp = dis_V % 10;
		LCD_digit_M(X_fenshu[i],4,temp+'0');
		dis_V=dis_V/10;
	}

}
void dis_counter_uSvhr4(dis_V)
{
	uchar temp,i,k,b;
			
   	for(i = 0; i < 3; i++)
	{
		temp = dis_V % 10;
		LCD_digit_M(X_fenshu[i],6,temp+'0');
		dis_V=dis_V/10;
	}

}
void dis_counter_uSvhr5(dis_V)
{
	uchar temp,i,k,b;
			
   	for(i = 0; i < 3; i++)
	{
		temp = dis_V % 10;
		LCD_digit_M(X_fenshu2[i],6,temp+'0');
		dis_V=dis_V/10;
	}

}
void dis_data_led(dis_V)
{
	uchar temp,i,k,b;
			
   	for(i = 0; i < 5; i++)
	{
		temp = dis_V % 10;
		LCD_digit_M(X_fenshu3[i],2,temp+'0');
		dis_V=dis_V/10;
	}

}*/
