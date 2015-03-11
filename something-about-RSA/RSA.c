#include <stdio.h>
#include <time.h>
#include <gmp.h>
#define N 16

mpz_t n, e, d;

// Generate p, q
// calculate n, fain, e, d
// publickey:e, n
// privatekey:d, n
void rsaGenKey()
{
    mpz_t p, q, fain;
    time_t seed;
    time(&seed);

    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, (long)seed);

    // gmp_printf("state: %Zd\n",state);

    mpz_init(p);
    mpz_urandomb(p, state, N);
    // Generate a random number between 0-2^N-1
    // we get a random number p
    // but not sure the p is a prime number
    // so let us make a prime number for p

    mpz_nextprime(p,p);
    // make p be a bigger prime number
    // Now, we get a prime p
    // also need a prime q

    // use the same way, generate a prime q
    gmp_randseed_ui(state, (long)(seed+1));
    mpz_init(q);
    mpz_urandomb(q, state, N);
    mpz_nextprime(q,q);

    // Next, we need to calculate the n and fain

    mpz_init(n);
    // n=p*q;
    mpz_mul(n, p, q);

    //p = p - 1
    mpz_sub_ui(p, p, 1);
    //q = q - 1
    mpz_sub_ui(q, q, 1);
    // fain = (p-1)*(q-1)
    mpz_init(fain);
    mpz_mul(fain, p, q);

    // ok, last step, get the value of e,d
    // e = fain / 2
    mpz_init(e);
    mpz_divexact_ui(e, fain, 2);
    // get the prime which is bigger than current e
    // That make gcd(e, fain)=1
    mpz_nextprime(e, e);

    // calculate d
    mpz_init(d);
    // ed ≡ 1 (mod fain)
    mpz_invert(d, e, fain);

    gmp_printf("p: %Zd\n",p);
    gmp_printf("q: %Zd\n",q);
    gmp_printf("fain: %Zd\n",fain);
    gmp_printf("n: %Zd\n",n);
    gmp_printf("e: %Zd\n",e);
    gmp_printf("d: %Zd\n",d);

    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(fain);
    gmp_randclear(state);
}

int main(int argc, char const *argv[])
{
    rsaGenKey(); 

    mpz_t messageDecrypted, messageEncrypted, message, tmpp, tmpq;

    //init all
    mpz_init(message);
    mpz_init(messageDecrypted);
    mpz_init(messageEncrypted);
    mpz_init(tmpq);
    mpz_init(tmpp);
    gmp_randstate_t state;
    gmp_randinit_default(state);

    time_t seed;
    time(&seed);
    gmp_randseed_ui(state, (long)seed);

    // Generate a message by random way
    // fit message < n
    // also make message meet the prerequisite
    // message = tmpq * tmpq
    do{
        mpz_urandomb(tmpp, state, N);
        mpz_urandomb(tmpq, state, N);
        mpz_mul(message, tmpp, tmpq);
    }while(mpz_cmp(message, n)>=0);

    gmp_printf("tmpq: %Zd\n", tmpq);
    gmp_printf("tmpp: %Zd\n", tmpp);
    gmp_printf("message=tmpp*tmpq: %Zd\n", message);

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
    mpz_clear(tmpq);
    mpz_clear(tmpp);
    gmp_randclear(state);

    return 0;
}