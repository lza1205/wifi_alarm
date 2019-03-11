
#ifndef __MYSTRING_H_
#define __MYSTRING_H_

#include "all.h"

extern u8 *ret_pointer;//�������ص�����ָ��

extern unsigned int STR_strlen(u8 *str);
  
//��������ת��Ϊ�ַ���
extern void STR_itoa(u32 val, u8 *buf, u8 radix);     


/*---------------------------
����:��һ���ֽڵĸߵ�λ�Ե�
����: pSrc=Ϊ����ת���ֽ�
      sLen=����
���أ�pDst      
*----------------------------*/
extern void STR_Byte_Invert(u8 *pSrc,u16 sLen,u8 *pDst);


/*
���ܣ��ҳ�str2�ַ�����str1�ַ����е�һ�γ��ֵ�λ�ã�������str2�Ĵ�����������
����ֵ�����ظ�λ�õ�ָ�룬���Ҳ��������ؿ�ָ�롣
*/
extern u8 *STR_strstr(u8 *s1, u8 *s2 );

/*
���ܣ������ַ���s���״γ����ַ�c��λ��
˵���������״γ���c��λ�õ������ţ����s�в�����c�򷵻�0��
*/
extern u8 STR_strchr(u8 *str,u8 len, char c);


/*
���ܣ������ַ���s�ģ�unsigned int�ͣ����ȣ�������'\0'����
˵��������s�ĳ��ȣ�������������NULL��
*/
extern unsigned int STR_strlen(u8 *str); 

//�����ַ���ָ��λ�õ���ֵ,������ֵ�Ͳ��ҳ���
extern u32 STR_GetVal(u8* s,u8* retlen);


/*
���ܣ����ַ����в���ָ���ַ����ĵ�һ�γ���,�����ִ�Сд 
������sLen=Դ���ȣ�
      *pSrc=Դ�ַ���
      wLen=Ҫ���ҵ��ַ�������
      q=Ҫ���ҵ��ַ���
����:Ŀ�괮����StringLength
*/
extern u16 STR_Find(u8 *pSrc,u16 sLen,u8 *q,u8 wLen);


//��ascii��תΪucs2,����ת����ĳ���
extern u16 STR_ASCIItoUCS2(u8* pSrc,u16 srclen,u8* pDst);

//��ascii��תΪucs2�ı�,����ת����ĳ���
u16 STR_ASCIItoUCS2_str(u8* pSrc,u16 srclen,u8* pDst);

//��ucs2���ʽ���ַ�תΪascii,���ʧ���򷵻�ucs2,���򷵻�ת����ĳ���
extern u16 STR_UCS2toASCII(u8* pSrc,u16 srclen,u8* pDst);

//�˺�����������strncpy,��֪ʲôԭ��strncpy������ʱ����������
extern void STR_strncpy(u8* s1,u8* s2,u16 len);


extern u8* STR_strcpy(u8 *strDest, u8 *strSrc);


/* -------------------------------------------------------
��  �ܣ�����ֵת�����ַ��ĳ��ȡ�
��  ����nSrcLength--Դ����,
        pSrc--Դ�ַ���ָ��,
	    pDst--Ŀ���ַ���ָ��,	    
����ֵ��ת��ĳ���
---------------------------------------------------------*/ 
extern u16 STR_Bytes2String(u16 nSrcLength,u8* pSrc, u8* pDst);


/* -------------------------------------------------------
��  �ܣ���unicode�ַ�ת�����ַ�byte��
��  ����nSrcLength--Դ����,
        pSrc--Դ�ַ���ָ��,
	    pDst--Ŀ���ַ���ָ��,	    
����ֵ��ת��ĳ���
---------------------------------------------------------*/ 
extern u16 STR_String2Bytes(u16 nSrcLength,u8* pSrc, u8* pDst);


/*-------------------------------------------------------------------------
* ����������˳����ַ���ת��Ϊ�����ߵ����ַ�����������Ϊ�������� ' 'F ' '�ճ�ż�� 
*       �磺 "8613851872468 " --> "683158812764F8 " 
* ������nSrcLength: Դ�ַ�������
        pSrc: Դ�ַ���ָ�� 
*       pDst: Ŀ���ַ���ָ��
*        
* ����: Ŀ���ַ�������
*--------------------------------------------------------------------------*/
extern u16 STR_InvertNumbers(u16 nSrcLength, u8* pSrc, u8* pDst); 


/*-------------�ַ����ļ��ܽ���------------------*/

//�ַ������ܺͽ���
extern void STR_Enc_Dec(u8* str,u16 len,u8* password,u8 pwdlen,u8* newstr);


//�ַ����û�
extern void STR_Exchange(u8* str,u16 len);

/*-------------------------------------------------------------------------
* ���������ַ����е�A��B��C��D��E�ֱ�ת�����š�*��#���ֺš��ʺ���5���ַ�! ,�������Ϊ����������'F'��ʾ*       
* ������len: Դ�ַ�������
        pSrc: Դ�ַ���ָ�� 
*       pDst: Ŀ���ַ���ָ��
* ����: �³���
*--------------------------------------------------------------------------*/
extern u8 STR_Convert_ABCDE(u8* pSrc,u16 len, u8* pDst);


//Hex�ַ����û�,���س���
extern u8 STR_HexExchange(u8* pSrc,u16 len, u8* pDst);


//�������У��
extern u8 STR_Calcxor(u8 *buf,u8 len);


//�ж��Ƿ����ַ����Ǻ���,�����ucs2������1,�����ascii����ͬʱ��ucs2תΪascii
extern u8 STR_DetUCS2(u8* str ,u16 len);

extern u8 *Find_Array(u8* s1, u8 len1, u8* s2, u8 len2);

extern char __str_cmp(char *str1, char *str2);



#endif

