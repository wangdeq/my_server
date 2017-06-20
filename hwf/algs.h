#include "aes.h"

#include "3des.h"
//#include "hwf.h"
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/crypto.h>
#include <linux/scatterlist.h>
#include <asm/byteorder.h>
#include <linux/slab.h>
#include <linux/syscalls.h>
#include <linux/gfp.h>
#include <linux/highmem.h>



static void enc_alg_1 (struct crypto_tfm *tfm , u8 *out, const u8 *in)
{
	printk ( "enc_alg_1 is working...\n" ) ;
	hwf_aes_encrypt (tfm , out , in) ;
}

static void dec_alg_1 (struct crypto_tfm *tfm , u8 *out, const u8 *in)
{
	printk ( "dec_alg_1 is working...\n" ) ;
	hwf_aes_decrypt (tfm , out , in) ;
}

//*****************************************************************************//

static void enc_alg_2 (struct crypto_tfm *tfm , u8 *out, const u8 *in)
{
	u8 dst1[8] , dst2[8] ;
	u8 src1[8] , src2[8] ;
	int i = 0 ;


	while ( i < 8 )
	{
		dst1[i] = out[i] ;
		dst2[i] = out[i+8] ;
		src1[i] = in[i] ;
		src2[i] = in[i+8] ;

		++ i ;
	}
	 
	printk ( "enc_alg_2 is working...\n" ) ;
	hwf_des_encrypt (tfm , dst1 , src1) ;
	hwf_des_encrypt (tfm , dst2 , src2) ;

	i = 0 ; 
	while ( i < 8 )
	{
		out[i] = dst1[i] ;
		out[i+8] = dst2[i] ;

		++ i ;
	}

	hex_print ( (u32*) out , "enc_out_end" , 16 ) ;
}

static void dec_alg_2 (struct crypto_tfm *tfm , u8 *out, const u8 *in)
{
	u8 dst1[8] , dst2[8] ;
	u8 src1[8] , src2[8] ;
	int i = 0 ;

	while ( i < 8 )
	{
		dst1[i] = out[i] ;
		dst2[i] = out[i+8] ;
		src1[i] = in[i] ;
		src2[i] = in[i+8] ;

		++ i ;
	}

	printk ( "dec_alg_2 is working...\n" ) ;
	hwf_des_decrypt (tfm , dst1 , src1) ;
	hwf_des_decrypt (tfm , dst2 , src2) ;
	
	i = 0 ;
	while ( i < 8 )
	{
		out[i] = dst1[i] ;
		out[i+8] = dst2[i] ;

		++ i ;
	}

hex_print ( (u32*) out , "dec_out_end" , 16  ) ;
}

//*****************************************************************************//


static void enc_alg_3 (struct crypto_tfm *tfm , u8 *out, const u8 *in)
{
	u8 dst1[8] , dst2[8] ;
	u8 src1[8] , src2[8] ;
	int i = 0 ;

	while ( i < 8 )
	{
		dst1[i] = out[i] ;
		dst2[i] = out[i+8] ;
		src1[i] = in[i] ;
		src2[i] = in[i+8] ;

		++ i ;
	}
	 
	printk ( "enc_alg_2 is working...\n" ) ;
	hwf_des3_ede_encrypt (tfm , dst1 , src1) ;
	hwf_des3_ede_encrypt (tfm , dst2 , src2) ;

	i = 0 ;
	while ( i < 8 )
	{
		out[i] = dst1[i] ;
		out[i+8] = dst2[i] ;

		++ i ;
	}
}

static void dec_alg_3 (struct crypto_tfm *tfm , u8 *out, const u8 *in)
{
	u8 dst1[8] , dst2[8] ;
	u8 src1[8] , src2[8] ;
	int i = 0 ;

	while ( i < 8 )
	{
		dst1[i] = out[i] ;
		dst2[i] = out[i+8] ;
		src1[i] = in[i] ;
		src2[i] = in[i+8] ;

		++ i ;
	}

	printk ( "dec_alg_2 is working...\n" ) ;
	hwf_des3_ede_decrypt (tfm , dst1 , src1) ;
	hwf_des3_ede_decrypt (tfm , dst2 , src2) ;

	i = 0 ;
	while ( i < 8 )
	{
		out[i] = dst1[i] ;
		out[i+8] = dst2[i] ;

		++ i ;
	}
}

/*
static void enc_alg_3 (struct crypto_tfm *tfm , u8 *out, const u8 *in)
{
	hwf_des3_ede_encrypt (tfm , out , in) ;
}

static void dec_alg_3 (struct crypto_tfm *tfm , u8 *out, const u8 *in)
{
	hwf_des3_ede_decrypt (tfm , out , in) ;
}
*/

//*****************************************************************************//
//************************     alg_4    ***************************************//
//******       try to combin different algorithms together        *************//
//*****************************************************************************//

static void enc_alg_4 (struct crypto_tfm *tfm , u8 *out, const u8 *in)
{
	u8 dst1[8] , dst2[8] ;
	u8 src1[8] , src2[8] ;
	int i = 0 ;

	while ( i < 8 )
	{
		dst1[i] = out[i] ;
		dst2[i] = out[i+8] ;
		src1[i] = in[i] ;
		src2[i] = in[i+8] ;

		++ i ;
	}
	 
	printk ( "enc_alg_2 is working...\n" ) ;
	hwf_des_encrypt (tfm , dst1 , src1) ;
	hwf_des3_ede_encrypt (tfm , dst2 , src2) ;

	i = 0 ;
	while ( i < 8 )
	{
		out[i] = dst1[i] ;
		out[i+8] = dst2[i] ;

		++ i ;
	}
}

static void dec_alg_4 (struct crypto_tfm *tfm , u8 *out, const u8 *in)
{
	u8 dst1[8] , dst2[8] ;
	u8 src1[8] , src2[8] ;
	int i = 0 ;

	while ( i < 8 )
	{
		dst1[i] = out[i] ;
		dst2[i] = out[i+8] ;
		src1[i] = in[i] ;
		src2[i] = in[i+8] ;

		++ i ;
	}

	printk ( "dec_alg_2 is working...\n" ) ;
	hwf_des_decrypt (tfm , dst1 , src1) ;
	hwf_des3_ede_decrypt (tfm , dst2 , src2) ;

	i = 0 ;
	while ( i < 8 )
	{
		out[i] = dst1[i] ;
		out[i+8] = dst2[i] ;

		++ i ;
	}
}

//*****************************************************************************//
//************************     alg_5    ***************************************//
//******       try to combin different algorithms together        *************//
//*****************************************************************************//

static void enc_alg_5 (struct crypto_tfm *tfm , u8 *out, const u8 *in)
{
	u8 dst1[8] , dst2[8] , dst3[16];
	u8 src1[8] , src2[8] , src3[16];
	int i = 0 ;

printk ( "enc_alg_5 is working |\n" ) ;

	while ( i < 8 )
	{
		dst1[i] = out[i] ;
		dst2[i] = out[i+8] ;
		src1[i] = in[i] ;
		src2[i] = in[i+8] ;

		++ i ;
	}
	 
	printk ( "enc_alg_2 is working...\n" ) ;
	hwf_des_encrypt (tfm , dst1 , src1) ;
	hwf_des3_ede_encrypt (tfm , dst2 , src2) ;

	i = 0 ;
	while ( i < 8 )
	{
		dst3[i] = src3[i] = dst1[i] ;
		dst3[i+8] = src3[i+8] = dst2[i] ;
	
		++ i ;
	}
	hwf_aes_encrypt (tfm , dst3 , src3) ;

	i = 0 ;
	while ( i < 16 )
	{
		out[i] = dst3[i] ;

		++ i ;
	}
}

static void dec_alg_5 (struct crypto_tfm *tfm , u8 *out, const u8 *in)
{
	u8 dst1[8] , dst2[8] , dst3[16] ;
	u8 src1[8] , src2[8] , src3[16] ;
	int i = 0 ;

printk ( "dec_alg_5 is working!\n" ) ;

	while ( i < 16 )
	{
		dst3[i] = out[i] ;
		src3[i] = in[i] ;

		++ i ;
	}
	hwf_aes_decrypt (tfm , dst3 , src3) ;

	i = 0 ; 

	while ( i < 8 )
	{
		dst1[i] = src1[i] = dst3[i] ;
		dst2[i] = src2[i] = dst3[i+8] ;

		++ i ;
	}

	printk ( "dec_alg_2 is working...\n" ) ;
	hwf_des_decrypt (tfm , dst1 , src1) ;
	hwf_des3_ede_decrypt (tfm , dst2 , src2) ;

	i = 0 ;
	while ( i < 8 )
	{
		out[i] = dst1[i] ;
		out[i+8] = dst2[i] ;

		++ i ;
	}
}

//*****************************************************************************//

static void enc_default (struct crypto_tfm *tfm , u8 *out, const u8 *in)
{
	const struct crypto_hwf_ctx *ctx = crypto_tfm_ctx(tfm);
	const __le32 *src = (const __le32 *)in;
	__le32 *dst = (__le32 *)out;
	u32 tmp[4] ;

	hex_print ( ctx->key , "enc_default is working with" , 32 ) ;
		
	tmp[0] = le32_to_cpu(src[0]) ^  ctx->key[0];
	tmp[1] = le32_to_cpu(src[1]) ^  ctx->key[1];
	tmp[2] = le32_to_cpu(src[2]) ^  ctx->key[2];
	tmp[3] = le32_to_cpu(src[3]) ^  ctx->key[3];
	
	dst[0] = cpu_to_le32(tmp[0]);
	dst[1] = cpu_to_le32(tmp[1]);
	dst[2] = cpu_to_le32(tmp[2]);
	dst[3] = cpu_to_le32(tmp[3]);
}

static void dec_default (struct crypto_tfm *tfm , u8 *out, const u8 *in)
{
	const struct crypto_hwf_ctx *ctx = crypto_tfm_ctx(tfm);
	const __le32 *src = (const __le32 *)in;
	__le32 *dst = (__le32 *)out;
	u32 tmp[4] ;

	hex_print ( ctx->key , "dec_default is working with" , 32 ) ;

	tmp[0] = le32_to_cpu(src[0]) ^  ctx->key[0];
	tmp[1] = le32_to_cpu(src[1]) ^  ctx->key[1];
	tmp[2] = le32_to_cpu(src[2]) ^  ctx->key[2];
	tmp[3] = le32_to_cpu(src[3]) ^  ctx->key[3];

	dst[0] = cpu_to_le32(tmp[0]);
	dst[1] = cpu_to_le32(tmp[1]);
	dst[2] = cpu_to_le32(tmp[2]);
	dst[3] = cpu_to_le32(tmp[3]);
}
