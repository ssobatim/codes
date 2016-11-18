/*
	libS4 - A library implementing the S4: A Secure Secret Splitting Scheme (S4).

	Somayeh Sobati Moghadam, Gérald Gavin and Jérôme Darmont. 

	 This code is available at http://eric.univ-lyon2.fr/~ssobati/libS4.h (adapt if necessary) under Creative Commons license Attribution-ShareAlike 4.0 International (CC BY-SA 4.0). https://creativecommons.org/licenses/by-sa/4.0/

You are free to:

    Share — copy and redistribute the material in any medium or format
    Adapt — remix, transform, and build upon the material for any purpose, even commercially.

    The licensor cannot revoke these freedoms as long as you follow the license terms.

Under the following terms:

    Attribution — You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.

    ShareAlike — If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.

    No additional restrictions — You may not apply legal terms or technological measures that legally restrict others from doing anything the license permits.

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gmp.h>
#include "libS4.h"


#define N 65
#define SUCCESS (EXIT_SUCCESS)





int set_xk_vk(const mpz_t xk, const mpz_t vk, const mpz_t prime, gmp_randstate_t rng_state)
{    

    mpz_t temp1,temp2;
    mpz_init_set_ui(xk, 0);
    mpz_urandomm(xk,rng_state, prime);    
    mpz_init_set_ui(vk, 0);
    mpz_urandomm(vk,rng_state, prime);
   
return 0;
}


int set_X(const int k, mpz_t * X, const mpz_t prime)
{   size_t prime_size = 0;
    int i;
    gmp_randstate_t M_rng_state;
    srand(time(NULL));
    gmp_randinit_default(M_rng_state);
    prime_size = mpz_sizeinbase(prime, 2);
    for (i = 0; i < (k - 1); i++) {
        mpz_init(X[i]);   
        mpz_urandomb(X[i], M_rng_state, prime_size - 1);
        mpz_add_ui(X[i], X[i], 1);
        
    }
return 0;
}


int S4_split_secret(const mpz_t * XX, 
                    mpz_t xk, 
		    mpz_t vk,
		    const mpz_t secret,
                    const unsigned int threshold,
                    const mpz_t prime,
                    mpz_t * shares_xs, gmp_randstate_t rng_state
                    )
{
    unsigned int i = 0, j = 0;
    size_t prime_size = 0;
    int retval = SUCCESS;
    
    prime_size = mpz_sizeinbase(prime, 2);
 
    


    mpz_t * rnd_x=NULL;
    mpz_t * rnd_y=NULL;
   
    rnd_x = (mpz_t *) malloc((N ) * sizeof(mpz_t));
    rnd_y = (mpz_t *) malloc((N) * sizeof(mpz_t));
       
    unsigned int  m = 0;
    
    mpz_t product, d, r;
    mpz_t reconstructed;



     mpz_init_set(XX[threshold - 1], xk);   

    /*  Random points  */
     for (i = 1; i <= (threshold - 2); i++) {
        mpz_init(rnd_x[i]);
        mpz_init(rnd_y[i]);
        mpz_urandomb(rnd_x[i], rng_state, prime_size - 1);
        mpz_urandomb(rnd_y[i], rng_state, prime_size - 1);
        mpz_add_ui(rnd_x[i], rnd_x[i], 1);
        mpz_add_ui(rnd_y[i], rnd_y[i], 1);
    }
    
          
    mpz_init_set(rnd_y[threshold - 1], vk);
    mpz_init_set(rnd_x[threshold - 1], xk);
    mpz_init_set(rnd_y[0], secret);
    mpz_init_set_ui(rnd_x[0], 0);


 
    long int lint;
    mpz_t d2;


/* Secret Splitting   */
    mpz_init_set_ui(reconstructed, 0);
   for(i=0; i < (threshold - 1); i++) {
         mpz_init_set_ui(reconstructed, 0);                     //Important to initialize with zero
        for (j = 0; j <=(threshold - 1); j++) {
        mpz_init_set_ui(product, 1);
        for (m = 0; m <= (threshold - 1); m++) {
            mpz_init(d);
            mpz_init(d2);
            mpz_init(r);
            if (m != j) {
                mpz_sub(d, rnd_x[m], rnd_x[j]);
                mpz_sub(d2, rnd_x[m], XX[i]);
                retval = mpz_invert(d, d, prime);                              
                mpz_mul(r, d2, d);
                mpz_mul(product, product, r);
            }
            mpz_clear(d);
            mpz_clear(d2);
            mpz_clear(r);
        }
         mpz_addmul(reconstructed, rnd_y[j], product);
         mpz_mod(reconstructed, reconstructed, prime);

        mpz_clear(product);
       }

    mpz_init_set(shares_xs[i], reconstructed);
    mpz_clear(reconstructed);
}

mpz_init_set(shares_xs[threshold-1], vk);
return 0;
}



int S4_secret_reconstruction(const int k,const mpz_t * XX, mpz_t xk, mpz_t vk,
                       const mpz_t * xxs, const mpz_t prime,
                       mpz_t secret
                 )
{
   unsigned int  m = 0, j;    
    mpz_t product, d, r;
    mpz_t reconstructed;
    int retval = SUCCESS;

mpz_init_set(xxs[k-1], vk);
mpz_init_set(XX[k-1], xk);

mpz_init_set_ui(reconstructed, 0);
    
    for (j = 0; j <= (k - 1); j++) {
        mpz_init_set_ui(product, 1);
        for (m = 0; m <= (k - 1); m++) {
            mpz_init(d);
            mpz_init(r);
            if (m != j) {
               
                mpz_sub(d, XX[m], XX[j]);
                retval = mpz_invert(d, d, prime);
                mpz_mul(r, XX[m], d);
                mpz_mul(product, product, r);
            }
            mpz_clear(d);
            mpz_clear(r);
        }
        
        mpz_addmul(reconstructed, xxs[j], product);
    
        mpz_mod(reconstructed, reconstructed, prime);
     
        mpz_clear(product);
    }
    mpz_init_set(secret, reconstructed);


 
    mpz_clear(reconstructed);
return 0; }


int S4_add(const int k, mpz_t vk, const mpz_t * xxs1,const mpz_t * xxs2,  mpz_t * sum, const mpz_t prime)
{
int i;
mpz_t temp;

for (i=0; i<=(k-1); i++)
     mpz_init(sum[i]);
for (i=0; i<(k-1); i++){    
   mpz_add(sum[i], xxs1[i], xxs2[i]);
        mpz_mod(sum[i], sum[i], prime);
}
     mpz_init(temp);
   mpz_add(temp, vk, vk);
    mpz_init_set(sum[k-1], temp);

return 0;
}


