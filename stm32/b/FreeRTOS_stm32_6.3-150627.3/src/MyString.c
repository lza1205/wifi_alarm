#include "all.h"
#include "stdio.h"
#include "string.h"

/*==================================================================================
====================================================================================
*******************************�ַ���������****************************************
====================================================================================
====================================================================================*/
u8 * ret_pointer;//�������ص�����ָ��
const u8 hex_tab[]="0123456789ABCDEF";    // 0x0-0xf���ַ����ұ�
  
//��������ת��Ϊ�ַ���
void STR_itoa(u32 val, u8 *buf, u8 radix)     
{     
  u8   *p;                          //pointer   to   traverse   string     
  u8   *firstdig;                   //pointer   to   first   digit     
  u8   temp;                        //temp   char     
  u8   digval;                  //value   of   digit     
  
  p   =   buf;     
  firstdig   =   p;                   //save   pointer   to   first   digit     
  
  do{     
  digval   =   (unsigned)   (val   %   radix);     
  val   /=   radix;               //get   next   digit    
  
  //convert   to   ascii   and   store     
  if   (digval   >   9)     
  *p++   =   (char   )   (digval   -   10   +   'a');     //a   letter     
  else     
  *p++   =   (char   )   (digval   +   '0');               //a   digit     
  }   while   (val   >   0);     
  
  /*   We   now   have   the   digit   of   the   number   in   the   buffer,   but   in   reverse    
  order.     Thus   we   reverse   them   now.   */     
  
  *p--   =   '\0';                         //terminate   string;   p   points   to   last   digit    
  
  do   {     
  temp   =   *p;     
  *p   =   *firstdig;     
  *firstdig   =   temp;       //swap   *p   and   *firstdig     
  --p;     
  ++firstdig;                   //advance   to   next   two   digits     
  }   while   (firstdig   <   p);   //repeat   until   halfway     
}  



/*---------------------------
����:��һ���ֽڵĸߵ�λ�Ե�
����: pSrc=Ϊ����ת���ֽ�
      sLen=����
���أ�pDst      
*----------------------------*/
void STR_Byte_Invert(u8 *pSrc,u16 sLen,u8 *pDst)
{
  u16 len;
  u8 data;
    
  for(len=0;len<sLen;len++)
  {
    data=*(pSrc+len);
    data=(data<<4)|(data>>4);                // ֮������Ϊ  1 0 0 1  0 1 0 1
    //data=((data<<2)&0xcc)|((data>>2)&0x33);  // ֮������Ϊ  0 1 1 0  0 1 0 1  
    //data=((data<<1)&0xaa)|((data>>1)&0x55);  // ֮������Ϊ  1 0 0 1  1 0 1 0
    *(pDst+len) = data;
  }
}

//���ַ���תΪСд,
/*u8* STR_ToLower(u8* str)
{//����Ĳ���str������������ʽ��������ָ���ֱ����������
    u8 i;
    
	for(i=0;i<STR_strlen(str);i++)
	{
		GsmTempBuff[i]=str[i];
		if(GsmTempBuff[i] >= 'A' && GsmTempBuff[i] <= 'Z')
		{	
		   GsmTempBuff[i]+=0x20;
		}
	}
	GsmTempBuff[i]='\0';
	return GsmTempBuff;
}*/


/*
���ܣ��ҳ�str2�ַ�����str1�ַ����е�һ�γ��ֵ�λ�ã�������str2�Ĵ�����������
����ֵ�����ظ�λ�õ�ָ�룬���Ҳ��������ؿ�ָ�롣
*/
u8 *STR_strstr(u8 *s1, u8 *s2 )
{ 
  unsigned int len2; 
  if ( (len2 = STR_strlen(s2)) == 0 ) 
  	return s1; 
  for ( ; *s1; ++s1 )
  { 
    if ( *s1 == *s2 && strncmp((const char*) s1, (const char*)s2, len2 )==0 ) 
    return s1; 
  } 
  return NULL; 
} 

/* ������1 �ҵ�����2 ���ֵ�λ�� */
u8 *Find_Array(u8* s1, u8 len1, u8* s2, u8 len2)
{
	u8 i, j;
	u8 flg;
	for(i = 0; i < len1; i ++){
		flg = 1;
		for(j = 0; j < len2; j++){
			if(s1[i + j] != s2[j]){
				flg = 0;
			}
		}
		if(flg == 1)
			return s1 + i;
	}
	return NULL;
}

/*
���ܣ������ַ���s���״γ����ַ�c��λ��
˵���������״γ���c��λ�õ������ţ����s�в�����c�򷵻�0��
*/
u8 STR_strchr(u8 *str,u8 len, char c)
{
  u8 i;
  
  for(i=0;i<len;i++)
  {
    if(*(str+i)==c)
    {
      return (i+1);
    }
  }  
  return (0);
}

/*
���ܣ������ַ���s�ģ�unsigned int�ͣ����ȣ�������'\0'����
˵��������s�ĳ��ȣ�������������NULL��
*/
unsigned int STR_strlen(u8 *str) 
{ 
  unsigned int len = 0; 
  while((*str++) != '\0') 
  len++; 
  return len; 
}

//�����ַ���ָ��λ�õ���ֵ,������ֵ�Ͳ��ҳ���
u32 STR_GetVal(u8* s,u8* retlen)
{
  u8 i,j;
  u32 val;
  
  val=0;
  for(j = 0;j < 10; j++)
  {
    if(*(s+j) >='0' && *(s+j) <='9')
    {
      i=*(s+j)-0x30;
      val = val*10 + i; //�ϲ�
    }
    else if(*(s+j) ==' ')
    {//���˿ո�
      
    }
    else
      break;	
  }
  *retlen=j;
  return(val);
}

/*
���ܣ����ַ����в���ָ���ַ����ĵ�һ�γ���,�����ִ�Сд 
������sLen=Դ���ȣ�
      *pSrc=Դ�ַ���
      wLen=Ҫ���ҵ��ַ�������
      q=Ҫ���ҵ��ַ���
����:Ŀ�괮����StringLength
*/
u16 STR_Find(u8 *pSrc,u16 sLen,u8 *q,u8 wLen)
{
  u8 i,j,k;	
  u8 z1,z2;
  u16 len;
  
  j=0;
  k=0;
  len=0;
  for(i=0;i<sLen;i++)
  {
    z1=pSrc[i];
    if(z1 >= 'A' && z1 <= 'Z') z1 += 0x20;//���ַ�תΪСд
    
    z2=q[j];
    if(z2 >= 'A' && z2 <= 'Z') z2 += 0x20;//���ַ�תΪСд
    
    if(z1==z2)
    {
      if(j==0)k=i;
      j++;
      if(j>=wLen)
      {
        len=sLen-k; 
        ret_pointer=&pSrc[k];
        return(len);
      }
    }
    else
     j=0;  
  } 
  ret_pointer=(u8 *)NULL;
  return len;
}

//��ascii��תΪucs2,����ת����ĳ���
u16 STR_ASCIItoUCS2(u8* pSrc,u16 srclen,u8* pDst)
{
  u16 ia,ja;
  static u16 meglen;
  
  if(srclen==0)return(0);
  meglen=srclen;
  ja=0;
  for(ia=0;ia<meglen;ia++)
  {
    //if(pSrc[ia]<0x80)	// ��ʾ����
    {
      pDst[ja]=0x00;
      pDst[ja+1]=pSrc[ia];
      ja+=2;
    }
    /*else
    {
      pDst[ja]=pSrc[ia];
      ja++;
    }*/
  }  
  return(ja);
}

//��ascii��תΪucs2�ı�,����ת����ĳ���
u16 STR_ASCIItoUCS2_str(u8* pSrc,u16 srclen,u8* pDst)
{
  u16 ia,ja;
  static u16 meglen;
  u8 buff[6];

  if(srclen==0)return(0);
  meglen=srclen;
  ja=0;
  for(ia=0;ia<meglen;ia++)
  {
    //if(pSrc[ia]<0x80)	// ��ʾ����
    {
      buff[0]=0x00;
      buff[1]=pSrc[ia];
	  STR_Bytes2String(2,buff,&buff[2]);
	  STR_strncpy(&pDst[ja],&buff[2],4);
	  ja+=4;
    }
    /*else
    {
      buff[0]=pSrc[ia];
	  STR_Bytes2String(1,buff,&buff[2]);
	  STR_strncpy(&pDst[ja],&buff[2],2);
	  ja+=2;
    }*/
  }  
  return(ja);
}

//��ucs2���ʽ���ַ�תΪascii,���ʧ���򷵻�ucs2,���򷵻�ת����ĳ���
u16 STR_UCS2toASCII(u8* pSrc,u16 srclen,u8* pDst)
{
  u16 i,j;
  u8 c;

  /*for(i=0;i<srclen;i++)
  {//�ж��Ƿ�Ϊucs2��
    c=*(pSrc+i); 
	if((c > 127 && c < 0xff) || (c>0 && c<0x20))
	{
		if(c != '\r' && c != '\n')
		{
			return(0);
		}
	}   
  } */
  
  j=0;  
  for(i=0;i<srclen;i++)
  {
  	 c=*(pSrc+i);
	 if(c != 0)
	 {
		*(pDst+j)=c;
		j++;
	 }
	 *(pDst+j)='\0';
  } 
  return(j);
}

//�˺�����������strncpy,��֪ʲôԭ��strncpy������ʱ����������
void STR_strncpy(u8* s1,u8* s2,u16 len)
{
  u16 i;
  
  for(i=0;i<len;i++)
  {
    s1[i]=s2[i];
  }
}

u8* STR_strcpy(u8 *strDest, u8 *strSrc)
{
  u8* tmp=strDest;

  if(!strDest||!strSrc)return NULL; 
  if (!STR_strlen(strSrc)) 
  {
    *strDest='\0';  
    return strDest;
  }    
  do  
  {  
    *tmp++=*strSrc; 
  } 
  while (*strSrc++); 
  return strDest;
}


/* -------------------------------------------------------
��  �ܣ�����ֵת�����ַ��ĳ��ȡ�
��  ����nSrcLength--Դ����,
        pSrc--Դ�ַ���ָ��,
	    pDst--Ŀ���ַ���ָ��,	    
����ֵ��ת��ĳ���
---------------------------------------------------------*/ 
u16 STR_Bytes2String(u16 nSrcLength,u8* pSrc, u8* pDst)
{	
  u16 i,j;
  u8 k;
  
  j=0;
  for (i=0; i<nSrcLength;i++)
  {
    k=*(pSrc+i);    
    // �����4λ
    pDst[j]=hex_tab[k>>4];//�ѱ���һ���ֽڱ�������ֽ�
    
    // �����4λ
    j++;
    pDst[j]=hex_tab[k&0x0f];
    
    j++;
  }  
  // ����ַ����Ӹ�������
  pDst[j]='\0';
  // ����Ŀ���ַ�������
  return (nSrcLength*2);
}

/* -------------------------------------------------------
��  �ܣ���unicode�ַ�ת�����ַ�byte,ͬʱ�ܽ�����ת����BCD��,���Զ�����+��F���ϵķǷ��ַ���
��  ����nSrcLength--Դ����,
        pSrc--Դ�ַ���ָ��,
	    pDst--Ŀ���ַ���ָ��,	    
����ֵ��ת��ĳ���
---------------------------------------------------------*/ 
u16 STR_String2Bytes(u16 nSrcLength,u8* pSrc, u8* pDst)
{	
  u16 i;
  u16 jsb=nSrcLength;
  u8 nDstJO;

  if(*pSrc=='+')
  {
    pSrc++;
	jsb--;
  }
  nDstJO = jsb % 2;
  if(nDstJO > 0)
  {//���� 
	jsb++;  
  }
  jsb=jsb/2;

  for (i = 0; i < jsb; i++)
  {
    // �����4λ
	if (*pSrc>='0' && *pSrc<='9')
    {
      *pDst=(*pSrc-'0')<<4;	  
    }
    else if (*pSrc>='A' && *pSrc<='F')
    {
      *pDst = (*pSrc - 'A' + 10) << 4;
    }
    else if (*pSrc>='a' && *pSrc<='f') 
    {
      *pDst = (*pSrc - 'a' + 10) << 4;
    }
	else if (*pSrc<=15)
	{
	   *pDst=(*pSrc)<<4;
	}
	if(i==(jsb-1) && nDstJO > 0) 
	{	   
	  *(pDst) |= 0x0f;
	  break;
	}
	pSrc++;

    // �����4λ
    if (*pSrc>='0'&&*pSrc<='9')    
    {
      *pDst |= *pSrc - '0';
    }
    else if (*pSrc>='A' && *pSrc<='F')
    {
      *pDst |= *pSrc - 'A' + 10;
    }
    else if (*pSrc>='a' && *pSrc<='f') 
    {
      *pDst |= *pSrc - 'a' + 10;
    }
	else if (*pSrc<=15)
	{
	   *pDst |= *pSrc;
	}
	
	pSrc++;
    pDst++;
  }
  
  // ����Ŀ�����ݳ���
  return (jsb);
}

//ɾ���ո�

/*-------------------------------------------------------------------------
* ����������˳����ַ���ת��Ϊ�����ߵ����ַ�����������Ϊ�������� ' 'F ' '�ճ�ż�� 
*       �磺 "8613851872468 " --> "683158812764F8 " 
* ������nSrcLength: Դ�ַ�������
        pSrc: Դ�ַ���ָ�� 
*       pDst: Ŀ���ַ���ָ��
*        
* ����: Ŀ���ַ�������
*--------------------------------------------------------------------------*/
u16 STR_InvertNumbers(u16 nSrcLength, u8* pSrc, u8* pDst) 
{ 
  u8 nDstJO,nlen; // �ж��Ƿ����Ϊ��
  u8 ch; // ���ڱ���һ���ַ� 
  u16 i;
  // ���ƴ����� 
  nlen = (u8)nSrcLength/2;
  nDstJO= (u8)nSrcLength%2; 
  // �����ߵ� 
  for (i = 0; i < nlen; i ++) 
  { 
    ch = *pSrc++; // �����ȳ��ֵ��ַ� 
    *pDst++ = *pSrc++; // ���ƺ���ֵ��ַ� 
    *pDst++ = ch; // �����ȳ��ֵ��ַ� 
  }
  if(nDstJO)
  {
    *pDst++ = 'F';
    *pDst++ = *pSrc;
  }
  *pDst = '\0';
  return (2*(nlen+nDstJO));
}

/*-------------�ַ����ļ��ܽ���------------------*/
#define MAP_LEN 18    

//�û���1
const char mappings1 [MAP_LEN][2] = {
                        {'0','A'},  
                        {'C','1'},  
                        {'G','E'},  
                        {'2','I'},  
                        {'K','M'},  
                        {'O','3'},  
                        {'Q','S'},  
                        {'4','U'},  
                        {'Y','W'},  
                        {'Z','5'},  
                        {'V','X'},  
                        {'6','T'},
                        {'R','P'},
                        {'N','7'},  
                        {'J','L'}, 
                        {'8','H'}, 
                        {'F','D'},  
                        {'B','9'}} ;  

//�û���1
const char mappings_hex2 [5][2] = {
                        {'A',','},  
                        {'B','*'},  
                        {'C','#'},  
                        {'D',';'},  
                        {'E','?'}, 
                        } ;  
//Hex�û���                  0     1     2     3     4     5     6     7     8     9     A     B    C      D     E     F
const char mappings_Hex []={'A' , 'S' , 'D' , 'F' , 'G' , 'H' , 'J' , 'K' , 'L' , 'Z' , 'X' , 'C' , 'V' , 'B' , 'N' , 'M'};

//�����ڵ��ַ�����0xff
u8 map(u8 chr)
{  
  u8 i = 0 ;  
  while(i < MAP_LEN)
  {  
    if(chr == mappings1[i][0])
    {  
      return mappings1[i][1] ; 
    }
    else if(chr == mappings1[i][1])
    {  
      return mappings1[i][0] ;  
    }  
    ++i ;  
  }
  return(0xff); 
}  


//�ַ������ܺͽ���
void STR_Enc_Dec(u8* str,u16 len,u8* password,u8 pwdlen,u8* newstr)
{
  u16 i,k;
  u8 v;
  
  k=0;
  for(i=0;i<len;i++)
  {
    v=str[i] ^ password[k];
    newstr[i]=v;
    k++;
    if(k==pwdlen)k=0;
  }  
}

//�ַ����û�
void STR_Exchange(u8* str,u16 len)
{
  u16 i;
  
  for(i=0;i<len;i++)
  {
    *(str+i)=map(*(str+i));
  }  
}

/*-------------------------------------------------------------------------
* ���������ַ����е�A��B��C��D��E�ֱ�ת�����š�*��#���ֺš��ʺ���5���ַ�! ,�������Ϊ����������'F'��ʾ*       
* ������len: Դ�ַ�������
        pSrc: Դ�ַ���ָ�� 
*       pDst: Ŀ���ַ���ָ��
* ����: �³���
*--------------------------------------------------------------------------*/
u8 STR_Convert_ABCDE(u8* pSrc,u16 len, u8* pDst)
{
  u16 i,j,k;
  u8 nDstJO;
  
  for(i=0;i<len;i++)
  {
    k=*(pSrc+i);
    for(j=0;j<5;j++)
    {
      if(k == mappings_hex2[j][0])
      {  
        k= mappings_hex2[j][1] ; 
        break;
      }
      else if(k == mappings_hex2[j][1])
      {  
        k= mappings_hex2[j][0] ; 
        break;
      }  
    }
    *(pDst+i)=k;    
  }
  
  nDstJO = len % 2;
  if(nDstJO > 0)
  {//����    
    *(pDst+i)='F'; 
    i++;    
  }
  *(pDst+i)='\0';
  return(i);
}

/*
���ܣ���̬����BCD��
������pSrc��Ҫ���ܵ��ַ�
      len,Ҫ���ܵ��ֽڳ���
      time,���ܵĶ�̬ʱ��
      pDst,���ܺ���ַ���
���أ����ܺ���ֽڳ���
*/
/*u8 STR_zEncBCD(u8* pSrc,u16 len, u8* time,u8* pDst)
{
  u8 j;
  
  j=STR_Convert_ABCDE(pSrc,j,pDst); 
}*/


//Hex�ַ����û�,���س���
u8 STR_HexExchange(u8* pSrc,u16 len, u8* pDst)
{
  u16 i,j,k,g,m;
  
  j=STR_Bytes2String(len,pSrc,pDst);
  for(i=0;i<j;i++)
  {
    m=0;      
    k=*(pDst+i);
    if(k>='0' && k<='9')k=k-0x30;
    else if(k>='A' && k<='F')k=k-0x37;
    else
    {
      for(g=0;g<16;g++)
      {
        if(k==mappings_Hex[g])
        {
          if(g<10)*(pDst+i)=g+0x30;
          else *(pDst+i)=g+0x37;            
          m=1;
          break;
        }
      }
    }
    if(m==0)*(pDst+i)=mappings_Hex[k];
  } 
  
  return(j);
}

//�������У��
u8 STR_Calcxor(u8 *buf,u8 len)
{
  u8 xorval,i;
  xorval=0;
  for(i=0;i<len;i++) xorval^=buf[i];
  return(xorval);
}

//�ж��Ƿ����ַ����Ǻ���,�����ucs2������1,�����ascii����ͬʱ��ucs2תΪascii
u8 STR_DetUCS2(u8* str ,u16 len)
{
  u16 i,j;
  u8 g,isucs2,iszero;
  
  isucs2=0;  
  for(i=0;i<len;i++)
  {
    g=*(str+i);    
    /*if(g>=0 && g <=127)
    {//����ȫ���ַ��� 

    }
    else
    {
      isucs2=1;
      break;
    }*/
    if((g > 127 && g < 0xff) || (g>0 && g<0x20))
    {
      if(g != '\r' && g != '\n')
      {
        isucs2=1;
        break;
      }
    }   
  }
  
  if(isucs2==0)
  {//�ڶ����ж�����   
    j=0;
    for(i=0;i<len/2;i++)
    {
      if(*(str+j)==0 /*&& *(str+j+1) >0*/)
      {
        iszero=1;
      }
      else
      {
        if(iszero==1)
        {
          isucs2=1;
          break;
        }
      }
      j+=2;
    }
  }
  
  if(isucs2==0)
  {//��usc2תΪascii    
    j=0;
    for(i=0;i<len;i++)
    {
      g=*(str+i);      
      if(g != 0)
      {
        *(str+j)=g;
        j++;
      }      
    }
    *(str+j)='\0';
  }    
  
  return(isucs2);
}


/* �Ƚ������ַ����Ƿ����
��ȷ���0
*/
char __str_cmp(char *str1, char *str2)
{
	char i = 0;
	while(((*str2) != '\0') && ((*str1) != '\0'))
	{
		if((*str1) != (*str2))
			return 1;
		str1 ++;
		str2 ++;
		i ++;
	}
	if(((*str2) == '\0') && (((*str1) == '\0')))
	{
		return 0;
	}
	return 1;
}


/*****
������ת�����ַ���
**/
#if 0
volatile u8 __tmp_buf[50];

u8 hex2str(u8 *dest, u8 *src, u8 len)
{
	u8 i;
	volatile u8 cnt = 0;
	for(i = 0; i < len; i++, cnt += 2)
	{
		dest[cnt] = (src[i] >> 4)    + '0';
		__tmp_buf[cnt] = (src[i] >> 4)    + '0';
		dest[cnt + 1] = (src[i] << 4)>>4 + '0';
		__tmp_buf[cnt + 1] =  (src[i] << 4)>>4 + '0';
	}
	return cnt;
}
#endif


