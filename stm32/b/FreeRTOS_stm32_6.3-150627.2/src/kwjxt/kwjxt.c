#define __KWJXT_C_

#include "all.h"

#include "kwjxt.h"



struct Kwjxt_Head kwjxt_head[] = {
	{SYSTEM_IMAGE_TYPE, 		SYSTEM_IMAGE_ADDR},
	{SYSTEM_AUDIO_TYPE, 		SYSTEM_AUDIO_ADDR},

	{LANG_1_SMS_TYPE, 			LANG_1_SMS_ADDR},
	{LANG_1_IMAGE_TYPE,			LANG_1_IMAGE_ADDR},
	{LANG_1_AUDIO_TYPE,			LANG_1_AUDIO_ADDR},

	{LANG_2_SMS_TYPE, 			LANG_2_SMS_ADDR},
	{LANG_2_AUDIO_TYPE,			LANG_2_AUDIO_ADDR},
	{LANG_2_IMAGE_TYPE,			LANG_2_IMAGE_ADDR},

	{LANG_3_SMS_TYPE, 			LANG_3_SMS_ADDR},
	{LANG_3_AUDIO_TYPE,			LANG_3_AUDIO_ADDR},
	{LANG_3_IMAGE_TYPE,			LANG_3_IMAGE_ADDR},

	{LANG_4_SMS_TYPE, 			LANG_4_SMS_ADDR},
	{LANG_4_AUDIO_TYPE,			LANG_4_AUDIO_ADDR},
	{LANG_4_IMAGE_TYPE,			LANG_4_IMAGE_ADDR},
	
	{0, 0},
};



u8 __temp_buf[1024];


static u8 __cmp_buf(u8 *buf1, u8* buf2, u16 len)
{
	u16 i;
	for(i = 0; i < len; i++)
	{
		if(buf1[i] != buf2[i])
			return 1;
	}
	return 0;
}

void __debug(void)
{
	W25X_Any_Read(0x106000 + 1024, __temp_buf, 1024);
}

u8 kwjxt_read(u16 type, u16 item, u8 *buf)
{
	u8 i;
	u32 addr_head, addr_base;
	u32 addr_offset;
	u16 len;
	u8 read[3];
	u32 addr_item;
	
	union ulong_type lsb;// new_lsb;

//	if(item)
//		item -= 1;

	sms_lange_ucs2=0;
	sms_Length_e2 = 0;


	
	if(item > 255)
		return 0;
	
		
	FlagUartE2Command = 1;
	
	for(i = 0; i < (sizeof(kwjxt_head) / sizeof(kwjxt_head[0])); i ++){
		if(kwjxt_head[i].type == type){
			addr_base = kwjxt_head[i].addr;
			goto out1;
		}
	}
	return 0;

out1:
	W25X_Any_Read(addr_base, __temp_buf, 1024);
	W25X_Any_Read(addr_base + 1024, __temp_buf, 1024);
	
	addr_head = addr_base + 0x13;
	addr_offset = 0;
	for(i = 0; i < item; i++){
		W25X_Any_Read(addr_head, read, 3);
		lsb.bb[0] = read[2];
		lsb.bb[1] = read[1];
		lsb.bb[2] = read[0];
		lsb.bb[3] = 0;

		if(lsb.ss[0] > 1000)
			return 0;
		addr_head += 3;

		addr_offset += lsb.ss[0];
		
	}
	
	W25X_Any_Read(addr_head, read, 3);	//读出此条的长度
	lsb.bb[0] = read[2];
	lsb.bb[1] = read[1];
	lsb.bb[2] = read[0];
	lsb.bb[3] = 0;

	if(lsb.bb[2] == 1)
		sms_lange_ucs2=1;

	len = lsb.ss[0];
	
	addr_item = addr_base + 1024 + addr_offset;

	if(len > 1024)
		return 0;

	for(i = 0; i < 3; i++)
	{
		W25X_Any_Read(addr_item, __temp_buf, len); 		//读出内容
		W25X_Any_Read(addr_item, buf, len);			//读出内容
		if(__cmp_buf(__temp_buf, buf, len) == 0){
			break;
		}else{
			printp("\r\nread w25q64 err \r\n");
		}
	}
	
	sms_Length_e2 = len;
	
	FlagUartE2Command = 0;

	return len;
}



u32 kwjxt_find_image(u16 type, u16 item)
{
	u8 i;
	u32 addr_head, addr_base;
	u32 addr_offset;
//	u16 len;
	u8 read[3];
	u32 addr_item;
	
	union ulong_type lsb, new_lsb;
	
	if(item > 2000)		//是公共部分
	{
		type = SYSTEM_IMAGE_TYPE;
		item -= 2000;
	}

	if(item)
		item -= 1;
	
	if(item > 255)
		return 0;
	
	FlagUartE2Command = 1;
	
	for(i = 0; i < (sizeof(kwjxt_head) / sizeof(kwjxt_head[0])); i ++){
		if(kwjxt_head[i].type == type){
			addr_base = kwjxt_head[i].addr;
			goto out1;
		}
	}
	return 0;

out1:
	W25X_Any_Read(addr_base, __temp_buf, 1024);
	
	addr_head = addr_base + 0x13;
	addr_offset = 0;
	for(i = 0; i < item; i++){
		W25X_Any_Read(addr_head, read, 3);
		lsb.bb[0] = read[2];
		lsb.bb[1] = read[1];
		lsb.bb[2] = read[0];
		lsb.bb[3] = 0;
		addr_head += 3;

		new_lsb.ss[0] = lsb.ss[0];
		new_lsb.ss[1] = lsb.ss[1];
		addr_offset += new_lsb.hex;
		
	}
	W25X_Any_Read(addr_head, read, 3);	//读出此条的长度
	lsb.bb[0] = read[2];
	lsb.bb[1] = read[1];
	lsb.bb[2] = read[0];
	lsb.bb[3] = 0;


	new_lsb.ss[0] = lsb.ss[0];
	new_lsb.ss[1] = lsb.ss[1];
//	len = new_lsb.hex;

	addr_item = addr_base + 1024 + addr_offset;

	
	W25X_Any_Read(addr_item, __temp_buf, sizeof(__temp_buf)); 		//读出内容

	FlagUartE2Command = 0;

	return addr_item - 4096;
}



u32 kwjxt_find_audio(u16 type, u16 item)
{
	u8 i;
	u32 addr_head, addr_base;
	u32 addr_offset;
//	u16 len;
	u8 read[3];
	u32 addr_item;
	
	union ulong_type lsb, new_lsb;

	if(item > 2000) 	//是公共部分
	{
		type = SYSTEM_AUDIO_TYPE;
		item -= 2000;
	}

	if(item)
		item -= 1;
	
	if(item > 255)
		return 0;
	
		
	FlagUartE2Command = 1;
	
	for(i = 0; i < (sizeof(kwjxt_head) / sizeof(kwjxt_head[0])); i ++){
		if(kwjxt_head[i].type == type){
			addr_base = kwjxt_head[i].addr;
			goto out1;
		}
	}
	return 0;

out1:
	W25X_Any_Read(addr_base, __temp_buf, 1024);
	W25X_Any_Read(addr_base + 1024, __temp_buf, 1024);
	
	addr_head = addr_base + 0x13;
	addr_offset = 0;
	for(i = 0; i < item; i++){
		W25X_Any_Read(addr_head, read, 3);
		lsb.bb[0] = read[2];
		lsb.bb[1] = read[1];
		lsb.bb[2] = read[0];
		lsb.bb[3] = 0;
		addr_head += 3;

			new_lsb.ss[0] = lsb.ss[0];
			new_lsb.ss[1] = lsb.ss[1];
			addr_offset += new_lsb.hex;
		
	}
	W25X_Any_Read(addr_head, read, 3);	//读出此条的长度
	lsb.bb[0] = read[2];
	lsb.bb[1] = read[1];
	lsb.bb[2] = read[0];
	lsb.bb[3] = 0;


		new_lsb.ss[0] = lsb.ss[0];
		new_lsb.ss[1] = lsb.ss[1];
//		len = new_lsb.hex;

	addr_item = addr_base + 1024 + addr_offset;

	
	W25X_Any_Read(addr_item, __temp_buf, sizeof(__temp_buf));		//读出内容

	FlagUartE2Command = 0;

	return addr_item - 4096;

}


void send_u32(u32 var)
{
	u8 i,j = 1;
	u32 temp = 1, num_tmp, var_tmp;

	var_tmp = var;
	for(i = 0; i < 10; i++)
	{
		temp = temp * 10;
		if(var_tmp < temp)	break;
		j++;
	}
	for(i = 0; i < j; i++)
	{
		temp = temp/10;
		num_tmp = var_tmp/temp;
		var_tmp = var_tmp % temp;
		sys_uart1_sendchar(num_tmp + 0x30);	
	}
}

void debug_kwjxt(void)
{
	u8 i, j = 1;
	my_send_str("kwjxt : \r\n");

	for(i = 0; kwjxt_head[i].type != 0; i++){

		if( (kwjxt_head[i].type & 0x00ff) == 0x02){
			my_send_str("\r\n");
			my_send_str("lang ");
			send_u32(j ++);
			my_send_str("\r\n");
		}
		send_u32(kwjxt_head[i].addr);
		my_send_str("\r\n");
	}
}

