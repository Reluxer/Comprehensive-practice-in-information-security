#include <stdio.h>
#include <time.h>
#include <gmp.h>
#define N 1024

mpz_t p, q, n, e1, e2, c1, c2, a, b, Gcd, m;

void rsaGenN()
{
    
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

    gmp_randclear(state);
    // gmp_printf("n: %Zd\n", n);
}

void rsaGenDoubleE()
{
    mpz_t one;
    time_t seed;
    time(&seed);

    gmp_randstate_t state;
    gmp_randinit_default(state);


    mpz_init(e1);
    mpz_init(e2);
    mpz_init(Gcd);
    mpz_init(one);
    mpz_set_ui(one, 1);  
    gmp_randseed_ui(state, (long)seed);                  
    do{
        // gmp_randseed_ui(state, (long)seed);
        mpz_urandomb(e1, state, N);

        // gmp_randseed_ui(state, (long)(seed+1));
        mpz_urandomb(e2, state, N);
        mpz_gcd(Gcd, e1, e2);
    }while(mpz_cmp(Gcd, one) != 0);
    

    gmp_randclear(state);
    mpz_clear(one);
    // gmp_printf("e1: %Zd\n", e1);
    // gmp_printf("e2: %Zd\n", e2);
}

void rsaGenab()
{
    mpz_init(a);
    mpz_init(b);
    mpz_gcdext(Gcd, a, b, e1, e2);
    // gmp_printf("a: %Zd\n", a);
    // gmp_printf("b: %Zd\n", b);
}

void rsaGenM()
{
    time_t seed;
    time(&seed);
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, (long)seed);
    mpz_init(m);

    do{
        mpz_urandomb(m, state, 1024);
    }while(mpz_cmp(m, n)>=0);
    
    gmp_randclear(state);
    // gmp_printf("m: %Zd\n", m);
}

void rsaGenCs()
{
    mpz_init(c1);
    mpz_init(c2);

    mpz_powm(c1, m, e1, n);
    mpz_powm(c2, m, e2, n);
    // gmp_printf("c1: %Zd\n", c1);
    // gmp_printf("c2: %Zd\n", c2);

}

void Attack()
{
    mpz_t ca, cb, mul;
    // unsigned long int aa, bb;
    mpz_init(ca);
    mpz_init(cb);
    mpz_init(mul);
    mpz_powm(ca, c1, a, n);
    mpz_powm(cb, c2, b, n);
    mpz_mul(mul, ca, cb);
    mpz_mod(mul, mul, n);

    if(mpz_cmp(mul, m) == 0){
        printf("Success!0^0\n");
        gmp_printf("The message is: %Zd\n", mul);
    }
    else{
        printf("Failed!0_0\n");
    }
}

void clear()
{
    mpz_clear(n);
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(e1);
    mpz_clear(e2);
    mpz_clear(c1);
    mpz_clear(c2);
    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(Gcd);
    mpz_clear(m);
}

int main(int argc, char const *argv[])
{
    rsaGenN();
    rsaGenDoubleE();
    rsaGenab();
    rsaGenM();
    rsaGenCs();
    gmp_printf("n: %Zd\n", n);
    gmp_printf("e1: %Zd\n", e1);
    gmp_printf("e2: %Zd\n", e2);
    gmp_printf("a: %Zd\n", a);
    gmp_printf("b: %Zd\n", b);
    // gmp_printf("m: %Zd\n", m);
    gmp_printf("c1: %Zd\n", c1);
    gmp_printf("c2: %Zd\n", c2);

    Attack();

    clear();
    return 0;
}