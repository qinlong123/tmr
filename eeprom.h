#define uint unsigned int
#define uchar unsigned char
#define ulong unsigned long
extern void byte_write(uchar str, uchar high_add,uchar low_add);
extern unsigned char byte_read(uchar high_add,uchar low_add);
extern void SectorErase(uchar high_add,uchar low_add);
extern void writein(uchar a[],uchar b1,uchar b);
extern unsigned char read_uSvhr_max(uchar b1 ,uchar k, uchar count);
extern void Refresh_all_k(char b,char b1);