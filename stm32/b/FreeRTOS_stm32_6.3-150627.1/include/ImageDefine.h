#ifndef __IMAGEDEFINE_H
#define __IMAGEDEFINE_H	

#define SizeZONE	             (u16)417	   //�������������


/*=========ϵͳ����==============*/
#define Config_Font	             (u32)2*4096   //�ֿ���ʼ��ַ

//�������ַ
#define Sound_ForElise	         2001   //38
#define Sound_JBells	         2002   //36
#define Sound_CPiano	         2003   //32
#define Sound_Dingdong	         2004  //10
#define Sound_PPiano	         2005  //32

													   
//����ͼƬ
#define Imag188x40_logo2         2001  		//����ҳlogo,4
#define Imag100x21_slogan        0xffff  		//�����ں�,1
#define Imag30x30_no	        2002        //δѡ���
#define Imag30x30_yes	         2003    //��ѡ���
#define Imag16x20_AL_A	         2004   //
#define Imag16x20_AL_P	         2005  //
#define Imag16x20_AL_M	         2006   //

#define Imag12x18_AL_0	        2007         //��������0
#define Imag12x18_AL_1	         2008   //��������1
#define Imag12x18_AL_2	         2009   //��������2
#define Imag12x18_AL_3	        2010   //��������3
#define Imag12x18_AL_4	         2011  //��������4
#define Imag12x18_AL_5	         2012  //��������5
#define Imag12x18_AL_6	         2013   //��������6
#define Imag12x18_AL_7	         2014   //��������7
#define Imag12x18_AL_8	         2015   //��������8
#define Imag12x18_AL_9	         2016   //��������9
#define Imag6x18_AL_mh	         2017   //:

#define Imag40x40_simCard	     2018             //sim��ͼ��
#define Imag40x40_away	         2019   //
#define Imag40x40_unlock	     2020      //
#define Imag40x40_home	         2021    //
#define Imag40x40_hist	         2022      // ��ʷ��¼
#define Imag40x40_alarm1	     2023      // ����
#define Imag40x40_timerAway      2024    //��ʱ����
#define Imag40x40_batt0	         2025 //��ص�ѹ 
#define Imag40x40_batt1	         2026      //
#define Imag40x40_batt2	         2027      //
#define Imag40x40_batt3	         2028      //
#define Imag40x40_cell1	         2029      //
#define Imag40x40_cell2	         2030     //
#define Imag40x40_sing1	         2031      //
#define Imag40x40_sing2	         2032     //
#define Imag40x40_sing3	         2033    //
#define Imag40x40_sing4	         2034     //
#define Imag40x40_ds	         2035     //ͯ��
#define Imag40x40_AC	         2036     //��ӵ��־
#define Imag24x24_al4            2037     //
#define Imag24x24_NoAL           2038     //������


#define Imag18x40_num_0	         2039             //��������0
#define Imag18x40_num_1	         2040    //��������1
#define Imag18x40_num_2	         2041   //��������2
#define Imag18x40_num_3	         2042    //��������3
#define Imag18x40_num_4	         2043    //��������4
#define Imag18x40_num_5	         2044    //��������5
#define Imag18x40_num_6	         2045    //��������6
#define Imag18x40_num_7	         2046    //��������7
#define Imag18x40_num_8	         2047    //��������8
#define Imag18x40_num_9	         2048    //��������9
#define Imag4x40_tao	         2049

#define Imag32x59_num0	         2050            //�¶�����0
#define Imag32x59_num1	         2051    //�¶�����1
#define Imag32x59_num2	         2052    //�¶�����2
#define Imag32x59_num3	         2053    //�¶�����3
#define Imag32x59_num4	         2054    //�¶�����4
#define Imag32x59_num5	         2055    //�¶�����5
#define Imag32x59_num6	         2056   //�¶�����6
#define Imag32x59_num7	         2057    //�¶�����7
#define Imag32x59_num8	         2058    //�¶�����8
#define Imag32x59_num9	         2059    //�¶�����9
#define Imag24x48_C	             2060
#define Imag24x48_F	             2061
#define Imag18x48_DU	         2062     //��
#define Imag12x59_wh	         2063     //-

#define Imag66x124_num0	         2064              //ʱ������0
#define Imag66x124_num1	         2065    //ʱ������1
#define Imag66x124_num2	         2066    //ʱ������2
#define Imag66x124_num3	         2067    //ʱ������3
#define Imag66x124_num4	         2068    //ʱ������4
#define Imag66x124_num5	         2069    //ʱ������5
#define Imag66x124_num6	         2070   //ʱ������6
#define Imag66x124_num7	         2071   //ʱ������7
#define Imag66x124_num8	         2072   //ʱ������8
#define Imag66x124_num9	         2073    //ʱ������9
#define Imag20x124_num10	     2074    //:


//ͼ��-����ѡ��
#define Imag140x143_luang1	     2075     //38������

/*=========Ӣ�Ĳ���==============*/

//��1������
#define Sound_Welcome	     1              //������ӭ����120K,ռ��30������	 
#define Sound_Hint	         2             //Զ�̲�����ʾ�������600K,ռ��150������	 

#define Imag_gap	             (u32)SizeZONE*4096	   //������ͼƬ��ʼ���������� 


//========��498-533��������������ͼ��=========== 
#define Imag153x40_month1	     1              //�·�1
#define Imag153x40_month2	     2     //�·�2
#define Imag153x40_month3	     3    //�·�3
#define Imag153x40_month4	     4     //�·�4
#define Imag153x40_month5	     5     //�·�5
#define Imag153x40_month6	     6    //�·�6
#define Imag153x40_month7	     7     //�·�7
#define Imag153x40_month8	     8     //�·�8
#define Imag153x40_month9	     9     //�·�9
#define Imag153x40_month10	     10     //�·�10
#define Imag153x40_month11	     11    //�·�11
#define Imag153x40_month12	     12    //�·�12

#define Imag164x40_week1	     13               //����1
#define Imag164x40_week2	     14      //����2
#define Imag164x40_week3	     15     //����3
#define Imag164x40_week4	     16     //����4
#define Imag164x40_week5	     17     //����5
#define Imag164x40_week6	     18      //����6
#define Imag164x40_week7	     19     //����7

#define Imag142x88_add	         20			     //������

#define Imag20x40_num0	         21               //0
#define Imag20x40_num1	         22       //
#define Imag20x40_num2	         23       //
#define Imag20x40_num3	         24      //3
#define Imag20x40_num4	         25       //4
#define Imag20x40_num5	         26       //5
#define Imag20x40_num6	         27       //6
#define Imag20x40_num7	         28       //7
#define Imag20x40_num8	         29       //8
#define Imag20x40_num9	         30       //9
#define Imag32x40_w1	         31      //һ
#define Imag32x40_w2	         32       //��
#define Imag32x40_w3	         33       //��
#define Imag32x40_w4	         34       //��
#define Imag32x40_w5	         35      //��
#define Imag32x40_w6	         36       //��
#define Imag32x40_w7	         37       //��
#define Imag32x40_m	             38       //��
#define Imag32x40_week	         39       //����

//========��573-591������ҳüҳ��ͼ��===========
#define Imag164x33_set	         40  //		  ����
#define Imag164x33_al	         41 //����
#define Imag164x33_doorbell      42  //������
#define Imag164x33_temp	         43  //
#define Imag164x33_sleep         44  //
#define Imag164x33_lang	         45  //

/* ���������ǿյ� */
//#define Imag38x38_page1	         (u32)629*4096  //
//#define Imag38x38_page2	         Imag38x38_page1 + 2896  //

//========��592-660����������ҳ���ͼ��===========
#define Imag190x200_menu1        46  //

#define Imag260x24_al1           47  //
#define Imag260x24_al2           48//
#define Imag260x24_al3           49//

#define Imag140x100_temp	     50

#define Imag140x200_sleep        51   //
 
//========��661-676����������ҳ�ұ�ͼ��===========
#define Imag120x180_r1	         52  //ѡ�������ʾ
#define Imag81x24_r2	         53  //����ҳ����
 

/*********************Ӣ�Ľ���*****************************/



#endif



