#ifndef _DATAPYTE_H_
#define _DATAPYTE_H_



//这一段无需改动 
#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL  0
#endif

#define u8		unsigned char
#define u16		unsigned short
#define u32 	unsigned long

#define uint8   unsigned char                 /* 无符号8位整型变量                       	*/
#define int8    char                /* 有符号8位整型变量                        */
#define uint16  unsigned short                 /* 无符号16位整型变量                       */
#define int16   short                /* 有符号16位整型变量                       */
#define uint32  unsigned long                 /* 无符号32位整型变量                       */
#define int32   long                /* 有符号32位整型变量                       */
#define fp32    float                /* 单精度浮点数（32位长度）                 */
#define fp64    double                 /* 双精度浮点数（64位长度）                 */

#define WCHAR	unsigned short



typedef unsigned char INT8U;
typedef signed char INT8S;
typedef unsigned short INT16U;
typedef signed short INT16S;
typedef unsigned long INT32U;
typedef signed long INT32S;
typedef float FP32;
typedef double FP64;
#define SET_B(x,y) (x|=(1<<y))
#define CLR_B(x,y) (x&=~(1<<y))

//#define NULL (void *)0

union ulong_type{
	INT32U hex;
	INT16U ss[2];
	INT8U bb[4];
};



#define bool		INT8U

#endif

