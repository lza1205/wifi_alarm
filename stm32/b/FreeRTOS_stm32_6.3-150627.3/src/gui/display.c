
#define __DISPLAY_C_

#include "all.h"

#define defY1 80
#define defx1 30

u16 SeletFrameIndex=defY1;
u16 old_SeletFrameIndex=defY1;
u8 old_Browse_key=0xff;


//���������ʾ
u8 curr_batt=0xff;
void DispBatter(void)
{

}

//����ģʽͼ�����
void ico_SysWorkModel(void)
{
	  //ϵͳ����״̬
	if(SysWorkModel==MODE_BUFANG)LCD_ShowE2PIC(75,10,(u32)Imag40x40_away); 	 
	else if(SysWorkModel==MODE_HOME_BUFANG)LCD_ShowE2PIC(75,10,(u32)Imag40x40_home); 
	else  LCD_ShowE2PIC(75,10,(u32)Imag40x40_unlock);
}


//GSM�ź�ǿ����ʾ
//���Ӳ���

void DispGsmSignal(void)
{
}

void Temp_Show(void)
{

}

void clock_show(void)
{
	
}

u8 variable_show(u8 type)
{
	//�¶�����
	Temp_Show();
	
	DispGsmSignal();
	DispBatter();

	/* ���ʱ����Ҫˢ�� */
	clock_show();

	return 0;
}

void display_idle(void)
{
	u32 x, y;
	x=0;y=0;
	LCD_DrawFill(x,480,y,60,Back_Titel2);  //���ö�������ɫ
	
	y+=60;
	LCD_DrawFill(x,320,y,190,Back_Time);
	LCD_DrawFill(x+320,160,y,190,Back_Temp);
	y+=190;
	LCD_DrawFill(x,480,y,70,Back_Titel2);     //�����·�����ɫ

	//ϵͳ����״̬
	ico_SysWorkModel();

	
	if(FlagICO_TimerAway==1)LCD_ShowE2PIC(270,10,(u32)Imag40x40_timerAway);        //��ʱ����
	if(FlagICO_Alarm==1)LCD_ShowE2PIC(370,10,(u32)Imag40x40_alarm1);               //���ӱ�־

	//��������
	variable_show(0);
}


//���ñ���ɫ
void ImagBack_Set(void)
{
   u16 i,x,y;
	
   x=0;y=0;
   LCD_DrawFill(x,480,y,320,Set_Back_Color);   //����
   LCD_DrawFill(x,480,58,4,Set_Line_Color);    //�������ָ���
   LCD_DrawFill(297,4,58,262,Set_Line_Color);  //������ָ���  
}


//�˵�ѡ���
void MENU_Selet_Frame_150x38(unsigned int x, unsigned int y, unsigned int color, u16 w)
{
  u8 h,b;

  w -=40; h=38;
  b=1;  //�ʴ�
#if 0  
  if(sysLangBuffer[MENULangType]==Lang_fre || sysLangBuffer[MENULangType]==Lang_spa
  	|| sysLangBuffer[MENULangType]==Lang_ger || sysLangBuffer[MENULangType]==Lang_Italian) w=165;
 #endif
  LCD_DrawFill(x,w,y,b,color);
  LCD_DrawFill(x,w,y+h,b,color);

  LCD_DrawFill(x,b,y,h,color);
  LCD_DrawFill(x+w,b,y,h+2,color); 
}


u8 logo2_x=20;	 //30,20
u8 logo2_y=14;	 //23,14
u8 curr_logo;

void display_menu()
{
	u8 z;
	u16 w;
	
	flag_daiji_main=2;

	ImagBack_Set();
	
	LCD_ShowE2PIC(logo2_x,logo2_y,(u32)Imag188x40_logo2);
	LCD_ShowE2PIC(286,23,(u32)Imag164x33_set+(u32)Imag_gap*(u32)(MENULangType));		
	if(curr_logo==0x02)
	{//Fortress
	   LCD_ShowE2PIC(150,26,(u32)Imag100x21_slogan);
	}

	//��ѡ���
	SeletFrameIndex=defY1;
	old_SeletFrameIndex=defY1;	
	
	LCD_ShowE2PIC(322,86,(u32)Imag120x180_r1+Imag_gap*(MENULangType));//���ұ�ͼ��
	//�����ͼ��		
	LCD_ShowE2PIC(42,80,(u32)Imag190x200_menu1+Imag_gap*(MENULangType));
	
	SeletFrameIndex=1*72+defY1;  //����y����
	old_SeletFrameIndex=SeletFrameIndex;		

	w = LCD_GetE2PICWidth((u32)Imag190x200_menu1+Imag_gap*(MENULangType));
	
	MENU_Selet_Frame_150x38(92,SeletFrameIndex,Fore_Font, w);	//65,116,167,218   
					
}


