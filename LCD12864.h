


#include <intrins.h>

#define nop() _nop_()


#define RdCommand 0x01 //定义ISP的操作命令
#define PrgCommand 0x02
#define EraseCommand 0x03 
#define Error 1
#define Ok 0
#define WaitTime 0x03 //定义CPU的等待时间





void wr_com(unsigned char com);

void LCD_init(void);
void LCD_clear(void);
void LCD_move_chinese_string(unsigned char X, unsigned char Y, unsigned char T); 


void LCD_draw_bmp(unsigned char X,unsigned char Y,unsigned char *map,
                  unsigned char Pix_x,unsigned char Pix_y);

void LCD_digit_M(unsigned char X, unsigned char Y, unsigned char d);


void LCD_write_byte(unsigned char dat, unsigned char dc);
void LCD_set_XY(unsigned char X, unsigned char Y);  

void LCD_write_char(unsigned char c);
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s);

void LCD_write1(unsigned char X, unsigned char Y,unsigned char* c,unsigned char with);
void LCD_write2(unsigned char X, unsigned char Y,unsigned char* c,unsigned char with);
void LCD_write3(unsigned char X, unsigned char Y,unsigned char* c,unsigned char with);
void LCD_writek(unsigned char X, unsigned char Y,unsigned char* c,unsigned char with);


          
 