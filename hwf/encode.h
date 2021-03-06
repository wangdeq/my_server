#include "algs.h"

enum alg_map
{
	alg_1 = '1' ,		//aes128
	alg_2 = '2' ,		//des
	alg_3 = '3' ,		//3des
	alg_4 = '4' ,		//des for the front 64 bits & 3des for the last 64 bits
	alg_5 = '5' ,		//aes for the second encrypt & des for the front 64 bits in the first encrypt 
				// & 3des for the last 64 bits in the first encrypt
} ;

static void make_enc_choice (struct crypto_tfm *tfm , const char alg_choice , u8 *out, const u8 *in)
{
	printk ( "alg_choice : %c\n" , alg_choice ) ;
	switch (alg_choice)
	{
		case alg_1 :
			printk ( "hwf---choice enc 111111\n" ) ;
			return enc_alg_1 (tfm , out , in) ;
			break ;
		case alg_2 :
			printk ( "hwf---choice enc 222222\n" ) ;
			return enc_alg_2 (tfm , out , in) ;
			break ;
		case alg_3 :
			return enc_alg_3 (tfm , out , in) ;
			break ;
		case alg_4 :
			return enc_alg_4 (tfm , out , in) ;
			break ;
		case alg_5 :
			return enc_alg_5 (tfm , out , in) ;
			break ;
		default :
			return enc_default (tfm , out , in) ;
			break ;
	}
}

static void make_dec_choice (struct crypto_tfm *tfm , const char alg_choice , u8 *out, const u8 *in)
{
	switch (alg_choice)
	{
		case alg_1 :
			return dec_alg_1 (tfm , out , in) ;
			break ;
		case alg_2 :
			return dec_alg_2 (tfm , out , in) ;
			break ;
		case alg_3 :
			return dec_alg_3 (tfm , out , in) ;
			break ;
		case alg_4 :
			return dec_alg_4 (tfm , out , in) ;
			break ;
		case alg_5 :
			return dec_alg_5 (tfm , out , in) ;
			break ;
		default :
			return dec_default (tfm , out , in) ;
			break ;
	}
}
