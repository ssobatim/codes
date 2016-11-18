
/* This is for testing libS4  */



#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "libS4.h"

#define N (128)



int main()
{
    
    unsigned int j,i  = 0,k;
    mpz_t secret1, secret2, firstPrime, xxs1[N], xxs2[N],sum[N];
    mpz_t Rec_secret1, Rec_secret2, Rec_sum;
    size_t prime_size = 0;
    

    printf("Enter the value of k: \n");
    scanf("%d", &k);


    mpz_init(firstPrime);    
    mpz_init_set_str(firstPrime, "909360333829", 10);
    mpz_init_set_str(secret1, "9", 10);
    mpz_init_set_str(secret2, "12", 10);
    prime_size = mpz_sizeinbase(firstPrime, 2);
  
   



    gmp_randstate_t rng_state;
    srand(time(NULL));
    gmp_randinit_default(rng_state);
    gmp_randseed_ui(rng_state, rand());
    mpz_t * XX=NULL;    
    XX = (mpz_t *) malloc((N ) * sizeof(mpz_t));
    for (i = 0; i < (k - 1); i++) 
        mpz_init(XX[i]);        
              
    
    
    mpz_t xk, vk;
    mpz_init(xk);
    mpz_init(vk);

    set_X(k, XX,  firstPrime);
    set_xk_vk( xk,  vk, firstPrime, rng_state);
    
 
    S4_split_secret((const mpz_t *)XX, xk, vk, secret1,  k, firstPrime, xxs1,rng_state);
    S4_split_secret((const mpz_t *)XX, xk, vk, secret2,  k, firstPrime, xxs2,rng_state);
for (j = 0; j < (k); j++)  
     gmp_printf ("\n X[%d]=%Zd, split1[%d]=%Zd ,split2[%d]=%Zd\n",j+1, XX[j], j+1,xxs1[j], j+1,xxs2[j]);

S4_secret_reconstruction( k,(const mpz_t *)XX,  xk,  vk,
                       (const mpz_t *) xxs1, firstPrime,
                       Rec_secret1
                 );


S4_secret_reconstruction( k,(const mpz_t *)XX,  xk,  vk,
                       (const mpz_t *) xxs2, firstPrime,
                       Rec_secret2
                 );

gmp_printf ("\n Re1=%Zd, Rec2=%Zd \n",Rec_secret1, Rec_secret2);

mpz_clear(firstPrime);
mpz_clear(secret1);
gmp_randclear(rng_state);
   
    return 0;
}
