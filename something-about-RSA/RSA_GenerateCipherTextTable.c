#include <stdio.h>
#include <string.h>
#include <math.h>
#include <gmp.h>

// After we done the work that the implementation of rsa
// we need to achieve the attack of rsa
// we choose a set of e, d, n and other value
// Like this:
// p: 36268---not know
// q: 57388---not know
// fain: 2081347984---not know
// n: 2081441641---know
// e: 1040674049---know
// d: 1880516161---not know

int main(int argc, char const *argv[])
{
    mpz_t e, n, tmp, end, cipherText;
    
    mpz_init(e);
    mpz_init(n);
    mpz_init(tmp);
    mpz_init(end);
    mpz_init(cipherText);

    // set the value of e
    mpz_set_str(e, "1040674049", 10);
    // set the value of n
    mpz_set_str(n, "2081441641", 10);

    // FILE* fp1, * fp2;
    // if((fp1 = fopen("tableCipherText1", "w")) == NULL){
    //     printf("Failed to open file tableCipherText1\n");
    //     return -1;
    // }
    // if((fp2 = fopen("tableCipherText2", "w")) == NULL){
    //     printf("Failed to open file tableCipherText2\n");
    //     return -1;
    // }

    mpz_set_str(tmp, "1", 10);
    mpz_set_str(end, "65535", 10);

    for(; mpz_cmp(tmp, end) <=0 ; mpz_add_ui(tmp, tmp, 1)){
        mpz_powm(cipherText, tmp, e, n);
        gmp_printf("%Zd\n", cipherText);
    }

    mpz_clear(e);
    mpz_clear(n);
    mpz_clear(tmp);
    mpz_clear(end);
    mpz_clear(cipherText);

    return 0;
}