/***********************************************************
�ļ���	:	aes256_data.c
����		:	Faker
�汾��	:	1.0
����		:	2015.05.15

˵��:	
	���ļ���ΪAES256 ���ܡ�key Ϊ aes256_key
	����� aes256_key_init ���г�ʼ��������

���ܽ��ܶ���һ�º���
	aes256_data

����ĺ���û��
	aes256_data_enc		����
	aes256_data_dec		����

***********************************************************/


#include <stdio.h>  
#include <string.h>  
#include <openssl/aes.h>  
#include <openssl/rand.h>  

#include<sys/time.h>
#include<unistd.h>



void aes256_data(char *in, char *out, int len, char *rkey, int rkey_len, int enc);

void aes256_data_dec(char *in, char *out, int len, 
						char *rkey, int rkey_len, int enc);
void aes256_data_enc(char *in, char *out, int len, 
						char *rkey, int rkey_len, int enc);


/***************************************************
AES256 �ܳ�
***************************************************/
char aes256_key[32] = {0x56, 0x96, 0xa5, 0x2f, 
						0xb8, 0x17, 0xc3, 0xd4,
						0x28, 0x7e, 0xa1, 0x32,
						0x15, 0x89, 0x35, 0x01,
						
						0x18, 0x4a, 0xb5, 0x3c,
						0x7b, 0x53, 0xf2, 0x4e,
						0xa9, 0x76, 0x18, 0x6b,
						0xfe, 0xac, 0xbd, 0xad
};

/* ��ӡ���� */
void hexdump(  
                FILE *f,  
                char *title,  
                char *s,  
                int l)  
{  
    int n = 0;  
  
    fprintf(f, "%s", title);  
    for (; n < l; ++n) {  
        if ((n % 16) == 0) {  
                fprintf(f, "\n%04x", n);  
        }  
        fprintf(f, " %d", s[n]);  
    }  
  
    fprintf(f, "\n");  
}

/***************************************
������: aes256_key_init
����: ��ʼ��AES256 key
***************************************/

void aes256_key_init(void)
{
	struct timeval tv1, tv2;
	struct timezone tz1, tz2;

    char   plaintext[AES_BLOCK_SIZE * 4 + 4] =  
    {  
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'i', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'i',  
            '0', '1', '2', '3', '4', '5', '6', '7', '0', '1', '2', '3', '4', '5', '6', '7',  
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'i', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'i',  
            '0', '1', '2', '3', '4', '5', '6', '7', '0', '1', '2', '3', '4', '5', '6', '7',
            'a', 'b', 'c', 'd'
    };  
	
    char   ciphertext[AES_BLOCK_SIZE * 4 + 4];  
    char   checktext[AES_BLOCK_SIZE * 4 + 4];  

#if 0
	printf("sizeof(plaintext) %d\n", (int)sizeof(plaintext));


    hexdump(stdout, "== aes256_key ==",  
                    aes256_key,  
                    sizeof(aes256_key));  


    hexdump(stdout, "== plaintext ==",  
                    plaintext,  
                    sizeof(plaintext));  
    printf("\n");  
#endif

	
	gettimeofday (&tv1 , &tz1);
	printf("--->> tv_sec; %d\n", (int)tv1.tv_sec) ;
	printf("--->> tv_usec; %d\n",(int)tv1.tv_usec);
	printf("--->> tz_minuteswest; %d\n", (int)tz1.tz_minuteswest);
	printf("--->> tz_dsttime, %d\n",(int)tz1.tz_dsttime);

	aes256_data_enc(plaintext, ciphertext, sizeof(plaintext), 
				aes256_key, sizeof(aes256_key), AES_ENCRYPT); 
		
	aes256_data_dec(ciphertext, checktext, sizeof(checktext), 
				aes256_key, sizeof(aes256_key), AES_DECRYPT);
		
	
	gettimeofday (&tv2 , &tz2);
	printf("--->>2 tv_sec; %d\n", (int)tv2.tv_sec) ;
	printf("--->>2 tv_usec; %d\n",(int)tv2.tv_usec);
	printf("--->>2 tz_minuteswest; %d\n", (int)tz2.tz_minuteswest);
	printf("--->>2 tz_dsttime, %d\n\n",(int)tz2.tz_dsttime);

#if 0
	hexdump(stdout, "== ciphertext ==",  
                    ciphertext,  
                    sizeof(ciphertext));  

	hexdump(stdout, "== checktext ==",  
                    checktext,  
                    sizeof(checktext));  
    printf("\n");  
#endif
}


/***************************************
������: aes256_data
����: �����ݽ��м��ܣ��м�����һ��Ҫ��
		16 �ı���

in 		����ǰ������
out		���ܺ������
len		����
rkey		�ܳ�
rkey_len	�ܳ׳���
enc		AES_DECRYPT		����
		AES_ENCRYPT		����

***************************************/
void aes256_data(char *in, char *out, int len, char *rkey, int rkey_len, int enc)
{
	int nr_of_bits = 0; 
	unsigned int i = 0;
	int len_s = len;
    AES_KEY         key;  

	
	while(len_s > 0){
		if(len_s >= 16){
			if(enc == AES_ENCRYPT){
		        nr_of_bits = 8 * rkey_len;  
		        AES_set_encrypt_key((unsigned char*)rkey, nr_of_bits, &key);  
		        AES_ecb_encrypt((unsigned char*)(in + i),  
		                        (unsigned char*)(out + i),   
		                        &key, AES_ENCRYPT);  
			}else if(enc == AES_DECRYPT){
				nr_of_bits = 8 * rkey_len;	
				AES_set_decrypt_key((unsigned char*)rkey, nr_of_bits, &key);
				AES_ecb_encrypt((unsigned char*)(in + i),  
								(unsigned char*)(out + i),	
								&key, AES_DECRYPT);  
			}
		}
		len_s -= 16;
		i += 16;
	}
}

#if 1
unsigned char buff_enc[1000];

void aes256_data_enc(char *in, char *out, int len, 
						char *rkey, int rkey_len, int enc)
{
	int ss, len_s;
	unsigned char buff_in[1000];
	
	len_s = len;

	memcpy(buff_in, in, len);
	
	if((ss = (len % 16)) != 0){
		memset(&buff_in[len], 1, 16);
		len_s = len + (16 - ss);
	}
#if 0	
	hexdump(stdout, "== buff_in ==",  
                    (char *)buff_in,  
                    len_s);
#endif
	aes256_data((char *)buff_in, (char *)buff_enc, len_s, 
				aes256_key, sizeof(aes256_key), AES_ENCRYPT);
#if 0	
	hexdump(stdout, "== buff_enc ==",  
                (char *)buff_enc,  
                len_s);
#endif	
	memcpy(out, buff_enc, len);
}

void aes256_data_dec(char *in, char *out, int len, 
						char *rkey, int rkey_len, int enc)
{
	
	int ss, len_s;
	unsigned char buff_out[1000];
	
	len_s = len;

	if((ss = (len % 16)) != 0){
		len_s = len + (16 - ss);
	}
#if 0
	hexdump(stdout, "== buff_enc ==",  
                (char *)buff_enc,  
                len_s);
#endif	
	aes256_data((char *)buff_enc, (char *)buff_out, len_s, 
				rkey, rkey_len, AES_DECRYPT);
#if 0
	hexdump(stdout, "== buff_out ==",  
                (char *)buff_out,  
                len_s);
#endif
	memcpy(out, buff_out, len);
}


#endif

