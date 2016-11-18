/*
	libS4 - A library implementing the S4: A Secure Secret Splitting Scheme (S4).

	Somayeh Sobati Moghadam, Gérald Gavin and Jérôme Darmont. 

	 This code is available at http://eric.univ-lyon2.fr/~ssobati/libS4.h (adapt if necessary) 
         under Creative Commons license Attribution-ShareAlike 4.0 International (CC BY-SA 4.0). https://creativecommons.org/licenses/by-sa/4.0/

You are free to:

    Share — copy and redistribute the material in any medium or format
    Adapt — remix, transform, and build upon the material for any purpose, even commercially.

    The licensor cannot revoke these freedoms as long as you follow the license terms.

Under the following terms:

    Attribution — You must give appropriate credit, provide a link to the license, and indicate if changes were made.
     You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.

    ShareAlike — If you remix, transform, or build upon the material, you must distribute your contributions under the 
     same license as the original.

    No additional restrictions — You may not apply legal terms or technological measures that legally restrict others from doing 
      anything the license permits.

*/


int set_X(const int k, mpz_t * X, const mpz_t prime);

int set_xk_vk(const mpz_t xk, const mpz_t vk, const mpz_t prime, gmp_randstate_t rng_state);






int S4_split_secret(const mpz_t * XX, mpz_t xk, mpz_t vk,const mpz_t secret,
                 
                 const unsigned int threshold,
                 const mpz_t prime,
                 mpz_t * shares_xs, gmp_randstate_t rng_state
                 );

int S4_secret_reconstruction(const int k,const mpz_t * XX, mpz_t xk, mpz_t vk,
                       const mpz_t * xxs, const mpz_t prime,
                       mpz_t secret
                 );

int S4_add(const int k,mpz_t vk,const mpz_t * xxs1,const mpz_t * xxs2, mpz_t * sum , const mpz_t prime
                 );

