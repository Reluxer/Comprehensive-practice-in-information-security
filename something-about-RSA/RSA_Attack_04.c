#include <stdio.h>
#include <string.h>
#include <gmp.h>

int main(int argc, char const *argv[])
{
    mpz_t nint, eint, p0, p1, p, q, q0, q1, bint, tmpint, message; 
    mpf_t nfloat, efloat, a, bfloat, tmpfloat, one;

    // init
    mpz_init(nint);
    mpz_init(eint);
    mpz_init(bint);
    mpz_init(message);
    mpz_init(tmpint);
    mpz_init(p0);
    mpz_init(p1);
    mpz_init(p);
    mpz_init(q);
    mpz_init(q0);
    mpz_init(q1);
    mpf_init(nfloat);
    mpf_init(efloat);
    mpf_init(a);
    mpf_init(bfloat);
    mpf_init(tmpfloat);
    mpf_init(one);


    mpz_set_str(nint, "9449868410449", 10);
    mpz_set_str(eint, "6792605526025", 10);
    mpz_set_str(message, "123456789", 10);
    mpf_set_ui(one, 1);
    mpf_set_z(nfloat, nint);
    mpf_set_z(efloat, eint);
    mpf_div(a, efloat, nfloat);
    mpf_floor(bfloat, a);
    mpz_set_ui(bint, (unsigned long int)mpf_get_d(bfloat));
    mpz_set(p0, bint);
    mpz_set_ui(q0, 1);

    mpf_sub(tmpfloat, a, bfloat);
    mpf_div(a, one, tmpfloat);
    mpf_floor(bfloat, a);
    mpz_set_ui(bint, (unsigned long int)mpf_get_d(bfloat));
    mpz_mul(tmpint, bint, p0);
    mpz_add_ui(p1, tmpint, 1);
    mpz_set(q1, bint);
    mpz_set(q, q1);


    // gmp_printf("nint: %Zd\n", nint);
    // gmp_printf("eint: %Zd\n", eint);
    // gmp_printf("nfloat: %Ff\n", nfloat);
    // gmp_printf("efloat: %Ff\n", efloat);
    // gmp_printf("a: %Ff\n", a);
    // gmp_printf("a: %Ff\n", a);
    // gmp_printf("q1: %Zd\n", q1);

    int cnt = 1 << 30;
    while(cnt--){
        mpz_mul(tmpint, eint, q);
        mpz_powm(tmpint, message, tmpint, nint);
        if(mpz_cmp(tmpint, message) == 0){
            printf("Success!!!\n");
            gmp_printf("d=%Zd\n", q);
            break;
        }
        mpf_sub(tmpfloat, a, bfloat);
        mpf_div(a, one, tmpfloat);
        mpf_floor(bfloat, a);
        mpz_set_ui(bint, (unsigned long int)mpf_get_d(bfloat));
        // mpz_mul(tmpint, bint, p1);
        // mpz_add(tmpint, tmpint, p0);
        // mpz_set(p, tmpint);
        // mpz_set(p0, p1);
        // mpz_set(p1, p);

        mpz_mul(tmpint, bint, q1);
        mpz_add(tmpint, tmpint, q0);
        mpz_set(q, tmpint);
        mpz_set(q0, q1);
        mpz_set(q1, q);
    }

    if(cnt <=0){
        printf("Failed!!\n");
    }

    // clear
    mpz_clear(nint);
    mpz_clear(eint);
    mpz_clear(p);
    mpz_clear(p0);
    mpz_clear(p1);
    mpz_clear(q);
    mpz_clear(message);
    mpz_clear(bint);
    mpz_clear(q1);
    mpz_clear(q0);
    mpz_clear(tmpint);
    mpf_clear(nfloat);
    mpf_clear(efloat);
    mpf_clear(a);
    mpf_clear(bfloat);
    mpf_clear(tmpfloat);
    mpf_clear(one);
    
    return 0;
}