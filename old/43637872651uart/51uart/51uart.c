#include <REG52.H>

#define  uchar unsigned char
#define  uint  unsigned int
sbit D1=P1^0;
sbit D2=P1^1;
sbit D3=P1^2;
sbit D4=P1^3;
sbit D5=P1^4;
sbit D6=P1^5;
sbit D7=P1^6;  
void light(uchar d)	;
void delay(uint ms)
{
        uint i,j;
	for(i=0;i<ms;i++)
	   {
	   for(j=0;j<1141;j++);
       }
}
void init_uart(void)
{
 SCON=0x50;
 PCON=0x00;
 ES=1;
 TMOD=0x21;
 TL1=0xFD;
 TH1=0xFD;
 TR1=1;
 EA=1;
 }
 
 void send(unsigned char text)
 {
  ES=0;
  SBUF=text;
  while(!TI);
  TI=0;
  ES=1;
  }

void receive() interrupt 4	 using 3
{ unsigned char temp;
  RI=0;
  temp=SBUF;
  light(temp);
}

void light(uchar d)
{
 switch(d)
 {
  case 0x01:D1=!D1;break;
  case 0x02:D2=!D2;break;
  case 0x03:D3=!D3;break;
  case 0x04:D4=!D4;break;
  case 0x05:D5=!D5;break;
  case 0x06:D6=!D6;break;
  case 0x07:D7=!D7;break;
 }
}


uchar key_press()
{
    uchar j,k;
    P0=0xFF;
	P2=0xFF;
	
	j=P0;
    k=P2&0x7F;
	if(j==0XFF&&k==0x7F)
	{
	  return 0;
	}
	else
	{
	 return 1;
	}
	
}

void key_scan()
{
    uchar key0,key2,key;
    delay(10);
	if(key_press())
	{
	key0=P0;
	key2=P2;
	key2&=0x7F;
	  if(key0!=0xff)
      {
          switch(key0)
              {  
			   case 0xfe:send(0x01);break;   //1111   1110
	           case 0xfd:send(0x02);break;    //1111 1101
	           case 0xfb:send(0x03);break;      //  *1111 1011
	           case 0xf7:send(0x04);break;       //   *   1111 0111
	           case 0xef:send(0x05);break;			//*1110 1111
	           case 0xdf:send(0x06);break;			//*1101 1111
	           case 0xbf:send(0x07);break;			//1011 1111
	           case 0x7f:send(0x08);break;			//0111 1111
	           default: key=0xff;   
			   }
		}
      if(key2!=0x7F)
       {		 
	       switch(key2)
	         {
	          case 0x7e:send(0x15);break;   //1111   1110
	          case 0x7d:send(0x14);break;    //1111 1101
	          case 0x7b:send(0x13);break;      //  *1111 1011
	          case 0x77:send(0x12);break;       //   *   1111 0111
	          case 0x6F:send(0x11);break;			//*1110 1111
           	  case 0x5F:send(0x10);break;			//*1101 1111
	          case 0x3F:send(0x09);break;			//1011 1111
	          default:key=0xff;
           	}	  
    	}
	  while(key_press());
	}
	else
	{
	  key=0xff;
	}
	}
void main()
{
 init_uart();
 while(1)
 {	 send(0x00);
     /*
	 if(key_press())
	 {
	  key_scan();
	  }
	   */
	  }
 
 
 }
     