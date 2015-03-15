#include <stdio.h>
#include <string.h>
#include <math.h>
#include <gmp.h>
#include <time.h>
#include <stdlib.h>
#define N 16

int main()
{
    mpz_t e, n, c, m1, m2, tmp, message;
    mpz_init(e);
    mpz_init(n);
    //set the value of e
    mpz_set_str(e, "1040674049", 10);
    // gmp_printf("e: %Zd\n", e);

    // set the value of n
    mpz_set_str(n, "2081441641", 10);//已知n
    // gmp_printf("n: %Zd\n", n);
    
    // init
    mpz_init(c);
    mpz_init(m1);
    mpz_init(m2);
    mpz_init(tmp);
    mpz_init(message);

    time_t seed;
    time(&seed);

    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, (long)seed);

    // generate a cipher text
    do{
        mpz_urandomb(m1, state, N);
        mpz_urandomb(m2, state, N);
        mpz_mul(message, m1, m2);
    }while(mpz_cmp(message, n)>=0);
    
    mpz_powm(c, message, e, n);

    gmp_printf("Now, I have a message, %Zd.\n%Zd=%Zd*%Zd.\n", message, message, m1, m2);
    gmp_printf("And the cipher text is %Zd.\n", c);
    printf("Next, let us begin the attack.\n");
    printf("Enter to start.Go?");
    getchar();

    int x, y, result;
    int done = 0;
    char strx[35], stry[35];

    FILE* fp1, * fp2;
    if((fp1 = fopen("tableCipherText1", "r")) == NULL){
        printf("Failed to open file tableCipherText1\n");
        return -1;
    }

    if((fp2 = fopen("tableCipherText2", "r")) == NULL){
        printf("Failed to open file tableCipherText2\n");
        return -1;
    }

    // Start brute force cracking...
    for(x = 1;fgets(strx, 35 ,fp1)!=NULL && done==0; x++){
        mpz_set_str(m1, strx ,10);
        fseek(fp2, 0, SEEK_SET);
        for(y=1; fgets(stry, 35, fp2) != NULL; y++){
            mpz_set_str(m2, stry, 10);
            mpz_mul(tmp, m1, m2);
            mpz_mod(tmp, tmp, n);
            if(mpz_cmp(c, tmp) == 0){
                // printf("%d*%d=%d\n",x,y,x*y);
                result = x*y;
                done = 1;
                break;
            }
        }
        if(x%500==0)
            printf("Waiting...\n");
    }

    fclose(fp1);
    fclose(fp2);
    if(done == 0)
        printf("Failed, I am sorry 0.0\n");
    else
        printf("OK, I have done, the message is %d, ^_^\n", result);

    mpz_clear(c);
    mpz_clear(m1);
    mpz_clear(m2);
    mpz_clear(tmp);
    mpz_clear(message);
    mpz_clear(e);
    mpz_clear(n);
    gmp_randclear(state);
    return 0;
}
