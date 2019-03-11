#ifndef _DATAPYTE_H_
#define _DATAPYTE_H_



//��һ������Ķ� 
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

#define uint8   unsigned char                 /* �޷���8λ���ͱ���                       	*/
#define int8    char                /* �з���8λ���ͱ���                        */
#define uint16  unsigned short                 /* �޷���16λ���ͱ���                       */
#define int16   short                /* �з���16λ���ͱ���                       */
#define uint32  unsigned long                 /* �޷���32λ���ͱ���                       */
#define int32   long                /* �з���32λ���ͱ���                       */
#define fp32    float                /* �����ȸ�������32λ���ȣ�                 */
#define fp64    double                 /* ˫���ȸ�������64λ���ȣ�                 */

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

