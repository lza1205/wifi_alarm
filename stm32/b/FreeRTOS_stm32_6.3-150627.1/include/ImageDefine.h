#ifndef __IMAGEDEFINE_H
#define __IMAGEDEFINE_H	

#define SizeZONE	             (u16)417	   //多语言扇区间隔


/*=========系统配置==============*/
#define Config_Font	             (u32)2*4096   //字库起始地址

//铃声库地址
#define Sound_ForElise	         2001   //38
#define Sound_JBells	         2002   //36
#define Sound_CPiano	         2003   //32
#define Sound_Dingdong	         2004  //10
#define Sound_PPiano	         2005  //32

													   
//公共图片
#define Imag188x40_logo2         2001  		//设置页logo,4
#define Imag100x21_slogan        0xffff  		//宣传口号,1
#define Imag30x30_no	        2002        //未选择框
#define Imag30x30_yes	         2003    //已选择框
#define Imag16x20_AL_A	         2004   //
#define Imag16x20_AL_P	         2005  //
#define Imag16x20_AL_M	         2006   //

#define Imag12x18_AL_0	        2007         //闹钟数字0
#define Imag12x18_AL_1	         2008   //闹钟数字1
#define Imag12x18_AL_2	         2009   //闹钟数字2
#define Imag12x18_AL_3	        2010   //闹钟数字3
#define Imag12x18_AL_4	         2011  //闹钟数字4
#define Imag12x18_AL_5	         2012  //闹钟数字5
#define Imag12x18_AL_6	         2013   //闹钟数字6
#define Imag12x18_AL_7	         2014   //闹钟数字7
#define Imag12x18_AL_8	         2015   //闹钟数字8
#define Imag12x18_AL_9	         2016   //闹钟数字9
#define Imag6x18_AL_mh	         2017   //:

#define Imag40x40_simCard	     2018             //sim卡图标
#define Imag40x40_away	         2019   //
#define Imag40x40_unlock	     2020      //
#define Imag40x40_home	         2021    //
#define Imag40x40_hist	         2022      // 历史记录
#define Imag40x40_alarm1	     2023      // 闹钟
#define Imag40x40_timerAway      2024    //定时布防
#define Imag40x40_batt0	         2025 //电池低压 
#define Imag40x40_batt1	         2026      //
#define Imag40x40_batt2	         2027      //
#define Imag40x40_batt3	         2028      //
#define Imag40x40_cell1	         2029      //
#define Imag40x40_cell2	         2030     //
#define Imag40x40_sing1	         2031      //
#define Imag40x40_sing2	         2032     //
#define Imag40x40_sing3	         2033    //
#define Imag40x40_sing4	         2034     //
#define Imag40x40_ds	         2035     //童锁
#define Imag40x40_AC	         2036     //外接电标志
#define Imag24x24_al4            2037     //
#define Imag24x24_NoAL           2038     //无闹钟


#define Imag18x40_num_0	         2039             //日期数字0
#define Imag18x40_num_1	         2040    //日期数字1
#define Imag18x40_num_2	         2041   //日期数字2
#define Imag18x40_num_3	         2042    //日期数字3
#define Imag18x40_num_4	         2043    //日期数字4
#define Imag18x40_num_5	         2044    //日期数字5
#define Imag18x40_num_6	         2045    //日期数字6
#define Imag18x40_num_7	         2046    //日期数字7
#define Imag18x40_num_8	         2047    //日期数字8
#define Imag18x40_num_9	         2048    //日期数字9
#define Imag4x40_tao	         2049

#define Imag32x59_num0	         2050            //温度数字0
#define Imag32x59_num1	         2051    //温度数字1
#define Imag32x59_num2	         2052    //温度数字2
#define Imag32x59_num3	         2053    //温度数字3
#define Imag32x59_num4	         2054    //温度数字4
#define Imag32x59_num5	         2055    //温度数字5
#define Imag32x59_num6	         2056   //温度数字6
#define Imag32x59_num7	         2057    //温度数字7
#define Imag32x59_num8	         2058    //温度数字8
#define Imag32x59_num9	         2059    //温度数字9
#define Imag24x48_C	             2060
#define Imag24x48_F	             2061
#define Imag18x48_DU	         2062     //度
#define Imag12x59_wh	         2063     //-

#define Imag66x124_num0	         2064              //时钟数字0
#define Imag66x124_num1	         2065    //时钟数字1
#define Imag66x124_num2	         2066    //时钟数字2
#define Imag66x124_num3	         2067    //时钟数字3
#define Imag66x124_num4	         2068    //时钟数字4
#define Imag66x124_num5	         2069    //时钟数字5
#define Imag66x124_num6	         2070   //时钟数字6
#define Imag66x124_num7	         2071   //时钟数字7
#define Imag66x124_num8	         2072   //时钟数字8
#define Imag66x124_num9	         2073    //时钟数字9
#define Imag20x124_num10	     2074    //:


//图库-语言选择
#define Imag140x143_luang1	     2075     //38个扇区

/*=========英文部分==============*/

//第1套语音
#define Sound_Welcome	     1              //开机欢迎语，最大120K,占用30个扇区	 
#define Sound_Hint	         2             //远程操作提示音，最大600K,占用150个扇区	 

#define Imag_gap	             (u32)SizeZONE*4096	   //多语言图片起始扇区递增数 


//========第498-533扇区，待机界面图库=========== 
#define Imag153x40_month1	     1              //月份1
#define Imag153x40_month2	     2     //月份2
#define Imag153x40_month3	     3    //月份3
#define Imag153x40_month4	     4     //月份4
#define Imag153x40_month5	     5     //月份5
#define Imag153x40_month6	     6    //月份6
#define Imag153x40_month7	     7     //月份7
#define Imag153x40_month8	     8     //月份8
#define Imag153x40_month9	     9     //月份9
#define Imag153x40_month10	     10     //月份10
#define Imag153x40_month11	     11    //月份11
#define Imag153x40_month12	     12    //月份12

#define Imag164x40_week1	     13               //星期1
#define Imag164x40_week2	     14      //星期2
#define Imag164x40_week3	     15     //星期3
#define Imag164x40_week4	     16     //星期4
#define Imag164x40_week5	     17     //星期5
#define Imag164x40_week6	     18      //星期6
#define Imag164x40_week7	     19     //星期7

#define Imag142x88_add	         20			     //添加配件

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
#define Imag32x40_w1	         31      //一
#define Imag32x40_w2	         32       //二
#define Imag32x40_w3	         33       //三
#define Imag32x40_w4	         34       //四
#define Imag32x40_w5	         35      //五
#define Imag32x40_w6	         36       //六
#define Imag32x40_w7	         37       //日
#define Imag32x40_m	             38       //月
#define Imag32x40_week	         39       //星期

//========第573-591扇区，页眉页脚图库===========
#define Imag164x33_set	         40  //		  设置
#define Imag164x33_al	         41 //闹钟
#define Imag164x33_doorbell      42  //门铃声
#define Imag164x33_temp	         43  //
#define Imag164x33_sleep         44  //
#define Imag164x33_lang	         45  //

/* 这两个都是空的 */
//#define Imag38x38_page1	         (u32)629*4096  //
//#define Imag38x38_page2	         Imag38x38_page1 + 2896  //

//========第592-660扇区，设置页左边图库===========
#define Imag190x200_menu1        46  //

#define Imag260x24_al1           47  //
#define Imag260x24_al2           48//
#define Imag260x24_al3           49//

#define Imag140x100_temp	     50

#define Imag140x200_sleep        51   //
 
//========第661-676扇区，设置页右边图库===========
#define Imag120x180_r1	         52  //选择操作提示
#define Imag81x24_r2	         53  //闹钟页返回
 

/*********************英文结束*****************************/



#endif



