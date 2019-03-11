
#ifndef __MYSTRING_H_
#define __MYSTRING_H_

#include "all.h"

extern u8 *ret_pointer;//函数返回的数组指针

extern unsigned int STR_strlen(u8 *str);
  
//将长整数转换为字符串
extern void STR_itoa(u32 val, u8 *buf, u8 radix);     


/*---------------------------
功能:把一个字节的高低位对调
参数: pSrc=为欲翻转的字节
      sLen=长度
返回：pDst      
*----------------------------*/
extern void STR_Byte_Invert(u8 *pSrc,u16 sLen,u8 *pDst);


/*
功能：找出str2字符串在str1字符串中第一次出现的位置（不包括str2的串结束符）。
返回值：返回该位置的指针，如找不到，返回空指针。
*/
extern u8 *STR_strstr(u8 *s1, u8 *s2 );

/*
功能：查找字符串s中首次出现字符c的位置
说明：返回首次出现c的位置的索引号，如果s中不存在c则返回0。
*/
extern u8 STR_strchr(u8 *str,u8 len, char c);


/*
功能：计算字符串s的（unsigned int型）长度，不包括'\0'在内
说明：返回s的长度，不包括结束符NULL。
*/
extern unsigned int STR_strlen(u8 *str); 

//计算字符串指定位置的数值,返回数值和查找长度
extern u32 STR_GetVal(u8* s,u8* retlen);


/*
功能：在字符串中查找指定字符串的第一次出现,不区分大小写 
参数：sLen=源长度；
      *pSrc=源字符串
      wLen=要查找的字符串长度
      q=要查找的字符串
返回:目标串长度StringLength
*/
extern u16 STR_Find(u8 *pSrc,u16 sLen,u8 *q,u8 wLen);


//将ascii码转为ucs2,返回转换后的长度
extern u16 STR_ASCIItoUCS2(u8* pSrc,u16 srclen,u8* pDst);

//将ascii码转为ucs2文本,返回转换后的长度
u16 STR_ASCIItoUCS2_str(u8* pSrc,u16 srclen,u8* pDst);

//将ucs2码格式的字符转为ascii,如果失败则返回ucs2,否则返回转换后的长度
extern u16 STR_UCS2toASCII(u8* pSrc,u16 srclen,u8* pDst);

//此函数用来代替strncpy,不知什么原因，strncpy函数有时不能起作用
extern void STR_strncpy(u8* s1,u8* s2,u16 len);


extern u8* STR_strcpy(u8 *strDest, u8 *strSrc);


/* -------------------------------------------------------
功  能：将数值转换成字符的长度。
参  数：nSrcLength--源长度,
        pSrc--源字符串指针,
	    pDst--目标字符串指针,	    
返回值：转后的长度
---------------------------------------------------------*/ 
extern u16 STR_Bytes2String(u16 nSrcLength,u8* pSrc, u8* pDst);


/* -------------------------------------------------------
功  能：将unicode字符转换成字符byte。
参  数：nSrcLength--源长度,
        pSrc--源字符串指针,
	    pDst--目标字符串指针,	    
返回值：转后的长度
---------------------------------------------------------*/ 
extern u16 STR_String2Bytes(u16 nSrcLength,u8* pSrc, u8* pDst);


/*-------------------------------------------------------------------------
* 描述：正常顺序的字符串转换为两两颠倒的字符串，若长度为奇数，补 ' 'F ' '凑成偶数 
*       如： "8613851872468 " --> "683158812764F8 " 
* 参数：nSrcLength: 源字符串长度
        pSrc: 源字符串指针 
*       pDst: 目标字符串指针
*        
* 返回: 目标字符串长度
*--------------------------------------------------------------------------*/
extern u16 STR_InvertNumbers(u16 nSrcLength, u8* pSrc, u8* pDst); 


/*-------------字符串的加密解密------------------*/

//字符串加密和解密
extern void STR_Enc_Dec(u8* str,u16 len,u8* password,u8 pwdlen,u8* newstr);


//字符串置换
extern void STR_Exchange(u8* str,u16 len);

/*-------------------------------------------------------------------------
* 描述：将字符串中的A、B、C、D、E分别转换逗号、*、#、分号、问号这5个字符! ,如果长度为奇数，就用'F'表示*       
* 参数：len: 源字符串长度
        pSrc: 源字符串指针 
*       pDst: 目标字符串指针
* 返回: 新长度
*--------------------------------------------------------------------------*/
extern u8 STR_Convert_ABCDE(u8* pSrc,u16 len, u8* pDst);


//Hex字符串置换,返回长度
extern u8 STR_HexExchange(u8* pSrc,u16 len, u8* pDst);


//异或运算校验
extern u8 STR_Calcxor(u8 *buf,u8 len);


//判断是否是字符还是汉字,如果是ucs2，返回1,如果是ascii，则同时将ucs2转为ascii
extern u8 STR_DetUCS2(u8* str ,u16 len);

extern u8 *Find_Array(u8* s1, u8 len1, u8* s2, u8 len2);

extern char __str_cmp(char *str1, char *str2);



#endif

