#include <stdio.h>
#include <time.h>
#include <gmp.h>
#include <stdlib.h>
#include <string.h>
#define N 1024

mpz_t c[10], n[10], m[10], inverseofm[10], mulofn, message,tmp;
int maxn;

void init()
{
    int i;
    mpz_init(mulofn);
    mpz_set_ui(mulofn, 1);
    mpz_init(tmp);
    mpz_init(message);
    mpz_set_ui(message, 0);

    for(i=0; i<10; i++){
        mpz_init(c[i]);
        mpz_init(n[i]);
        mpz_init(m[i]);
        mpz_init(inverseofm[i]);
    }

}

void clear()
{
    int i;
    mpz_clear(mulofn);
    mpz_clear(tmp);
    mpz_clear(message);

    for(i=0; i<10; i++){
        mpz_clear(c[i]);
        mpz_clear(n[i]);
        mpz_clear(m[i]);
        mpz_clear(inverseofm[i]);
    }
}

int fromFiletoArray(char filepath[], mpz_t arr[])
{
    FILE* fp;
    char str[2048];
    int i=0;
    if((fp = fopen(filepath, "r")) == NULL){
        printf("Failed to open %s\n", filepath);
        return -1;
    }
    while(fgets(str, 1024, fp) != NULL){
        if(strlen(str) <= 0) break;
        mpz_set_str(arr[i], str, 10);
        i++;
    }
    fclose(fp);
    return i;
}

int main(int argc, char const *argv[])
{

    init();
    int i;
    maxn = fromFiletoArray("table_C", c);
    if(maxn == -1){
        printf("Error!\n");
        exit(-1);
    }

    maxn = fromFiletoArray("table_N", n);
    if(maxn == -1){
        printf("Error!\n");
        exit(-1);
    }

    for(i=0; i<maxn;i++)
        mpz_mul(mulofn, mulofn, n[i]);

    for(i=0; i<maxn; i++){
        mpz_divexact(m[i], mulofn, n[i]);
        mpz_invert(inverseofm[i], m[i], n[i]);
    }

    for(i=0; i<maxn; i++){
        mpz_mul(tmp, m[i], inverseofm[i]);
        mpz_mul(tmp, tmp, c[i]);
        mpz_mod(tmp, tmp, mulofn);
        mpz_add(message, message, tmp);
        mpz_mod(message, message, mulofn);
    }
    mpz_root(message, message, 3);
    printf("Success!\n");
    gmp_printf("The message is: %Zd\n", message);
    
    clear();
    return 0;
}