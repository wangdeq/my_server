
//**************************************************//
#ifndef _CRYPTO_AES_H
#define _CRYPTO_AES_H

#include <linux/types.h>
#include <linux/crypto.h>

#define AES_MIN_KEY_SIZE	16
#define AES_MAX_KEY_SIZE	32
#define AES_KEYSIZE_128		16
#define AES_KEYSIZE_192		24
#define AES_KEYSIZE_256		32
#define AES_BLOCK_SIZE		16
#define AES_MAX_KEYLENGTH	(15 * 16)
#define AES_MAX_KEYLENGTH_U32	(AES_MAX_KEYLENGTH / sizeof(u32))

/*
 * Please ensure that the first two fields are 16-byte aligned
 * relative to the start of the structure, i.e., don't move them!
 */


#endif

//**************************************************//

#ifndef __CRYPTO_DES_H
#define __CRYPTO_DES_H

#define DES_KEY_SIZE		8
#define DES_EXPKEY_WORDS	32
#define DES_BLOCK_SIZE		8

#define DES3_EDE_KEY_SIZE	(3 * DES_KEY_SIZE)
#define DES3_EDE_EXPKEY_WORDS	(3 * DES_EXPKEY_WORDS)
#define DES3_EDE_BLOCK_SIZE	DES_BLOCK_SIZE

#endif 

//**************************************************//

#ifndef _CRYPTO_HWF_H
#define _CRYPTO_HWF_H

#include <linux/types.h>
#include <linux/crypto.h>

#define HWF_MIN_KEY_SIZE	16
#define HWF_MAX_KEY_SIZE	32
#define HWF_KEYSIZE_128		16
#define HWF_KEYSIZE_192		24
#define HWF_KEYSIZE_256		32
#define HWF_BLOCK_SIZE		16
#define HWF_MAX_KEYLENGTH	(15 * 16)
#define HWF_MAX_KEYLENGTH_U32	(HWF_MAX_KEYLENGTH / sizeof(u32))

/*
 * Please ensure that the first two fields are 16-byte aligned
 * relative to the start of the structure, i.e., don't move them!
 */
struct crypto_hwf_ctx {
	u32 key[4];	
	u32 key_length;
	u32 aes_enc[AES_MAX_KEYLENGTH_U32];
	u32 aes_dec[AES_MAX_KEYLENGTH_U32];
	u32 aes_length;
	u32 des_expkey[DES_EXPKEY_WORDS];
	u32 des_ede_expkey[DES3_EDE_EXPKEY_WORDS];
};


int crypto_hwf_set_key(struct crypto_tfm *tfm, const u8 *in_key,
		unsigned int key_len);
int crypto_hwf_expand_key(struct crypto_hwf_ctx *ctx, const u8 *in_key,
		unsigned int key_len);

int hwf_aes_set_key(struct crypto_tfm *tfm, const u8 *in_key,
		unsigned int key_len);
int hwf_aes_expand_key(struct crypto_hwf_ctx *ctx, const u8 *in_key,
		unsigned int key_len);

int key_flag = 0 ;

int hex_print ( const u32 * key , char * hint , unsigned int key_len )
{
	char tmp[400] ;
	char hex[100] ;
	int i = 0 ;
	int j = 0 ;
 	int count = 0 ;
	int bit = 0 ;
	int sum = 0 ;

	while ( count < key_len / 4 )
	{
		i = 0 ; 
		while ( i < 32 )
		{
			bit = key[count] >> i & 0x01 ;
			++ i ;
 			tmp[j++] = bit + '0' ;
		}
		++ count ;
	}
	tmp[j] = '\0' ;

	i = j = 0 ;
	while ( tmp[i] != '\0' )
	{
		count = sum = 0 ; 
		while ( count < 4 )
		{
			sum *= 2 ;
			sum += tmp[i++] - '0' ;
			++ count ; 
		}
		
		if ( sum < 10 )
		{
			hex[j++] = sum + '0' ;
		}
		else 
		{
			hex[j++] = sum + 'a' - 10 ;
		}
	}
	hex[j] = '\0' ;

	i = j = count = 0 ;
	while ( hex[i] != '\0' )
	{
		if ( count < 4 )
		{
			tmp[j++] = hex[i++] ;
			++ count ;
		}
		else
		{
			tmp[j++] = ' ' ;
			count = 0 ;
		}
	}
	tmp[j] = '\0' ;

	printk ( "%s: key : %s , (key_len : %d i = %d)\n" , hint , tmp , key_len , key_flag ) ;

	return 0 ;
}
#endif
