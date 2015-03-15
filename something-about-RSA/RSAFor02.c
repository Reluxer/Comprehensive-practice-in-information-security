#include <stdio.h>
#include <time.h>
#include <gmp.h>
#define N 1024

mpz_t n, e, d;

void rsaGenKey()
{
mpz_t p, q, fain;
time_t seed;
time(&seed);

gmp_randstate_t state;
gmp_randinit_default(state);
gmp_randseed_ui(state, (long)seed);

mpz_init(p);
mpz_urandomb(p, state, N);

mpz_nextprime(p,p);

gmp_randseed_ui(state, (long)(seed+1));
mpz_init(q);
mpz_urandomb(q, state, N);
mpz_nextprime(q,q);

mpz_init(n);

mpz_mul(n, p, q);
mpz_sub_ui(p, p, 1);
mpz_sub_ui(q, q, 1);
// fain = (p-1)*(q-1)
mpz_init(fain);
mpz_mul(fain, p, q);

mpz_init(e);
mpz_set_str(e, "3", 10);

// calculate d
mpz_init(d);
// ed ≡ 1 (mod fain)
mpz_invert(d, e, fain);

mpz_clear(p);
mpz_clear(q);
mpz_clear(fain);
gmp_randclear(state);
}

int main(int argc, char const *argv[])
{
    mpz_t messageDecrypted, messageEncrypted, message, zero;

    mpz_init(zero);
    mpz_set_str(zero, "0", 10);

    //init all
    mpz_init(message);
    mpz_init(messageDecrypted);
    mpz_init(messageEncrypted);
    gmp_randstate_t state;
    gmp_randinit_default(state);

    time_t seed;
    time(&seed);
    gmp_randseed_ui(state, (long)seed);

    mpz_set_str(message, "5126527318048144844832968115889706713283888978125012742238513282511339715516597357900182592447812166345444449837945136376706334859303904467602595422870737368628804302546885675400691338457905960059012201889714716412383110373481104688697270020", 10);

    do{
        rsaGenKey();
    }while(mpz_cmp(d, zero)<=0&&mpz_cmp(message, n) <=0);

    gmp_printf("n: %Zd\n",n);
    gmp_printf("e: %Zd\n",e);
    gmp_printf("d: %Zd\n",d);
    gmp_printf("message: %Zd\n", message);
    // rsa encrypt
    mpz_powm(messageEncrypted, message, e, n);
    gmp_printf("After encrypted: %Zd\n", messageEncrypted);

    // rsa decrypt
    mpz_powm(messageDecrypted, messageEncrypted, d, n);
    gmp_printf("After decrypted: %Zd\n", messageDecrypted);

    if(0 == mpz_cmp(messageDecrypted, message))
    printf("Success!\n");
    else printf("Fail!\n");

    // clear all
    mpz_clear(message);
    mpz_clear(messageDecrypted);
    mpz_clear(messageEncrypted);
    gmp_randclear(state);

return 0;
}
