#include "word.h"


char* int2word(mpz_t nb, char* charset, int base, char* word)
{
    //printf(" == %d : %s : %d == ", base, charset, nb);


    int i = 0;
    int t = 0;
    int k = 0;

    int nb_char= 0;
    int start = 0;

    mpz_t tmp, tmp2, tmp3, inf, number;

    mpz_init_set(number, nb);
    mpz_init_set(tmp, number);
    mpz_init(tmp2);
    mpz_init(tmp3);
    mpz_init_set_si(inf, 0);

    while(mpz_cmp_si(tmp, 0) >= 0)
    {
        //tmp -= (long)pow(base, nb_char+1);
        mpz_ui_pow_ui(tmp2, base, nb_char+1);
        mpz_sub(tmp, tmp, tmp2);
        nb_char++;
    }

    //printf("\nnb_char : %d\n", nb_char);



    start = nb_char-1;
    do{

        for(i=1;i<start;i++)
        {
            mpz_ui_pow_ui(tmp2, base, i);
            mpz_add(inf, inf, tmp2);
            //inf += (long)pow(base,i);
        }
        //gmp_printf("\nk : %d, inf : %Zd\n", k, inf);

        mpz_set_si(tmp, base-1);

        if(start != 0)
        {
            mpz_add_ui(tmp2, tmp, 1);
            mpz_ui_pow_ui(tmp3, base, start);
            mpz_mul(tmp2, tmp2, tmp3);
            mpz_add(tmp2, tmp2, inf);
            //while(((tmp+1)*(long)pow(base,start)+inf > nb))
            while(mpz_cmp(tmp2, number) > 0)
            {
                /* tmp++ */
                mpz_sub_ui(tmp, tmp, 1);

                mpz_add_ui(tmp2, tmp, 1);
                /* tmp3 doesn't change; useless to recalc */
                mpz_mul(tmp2, tmp2, tmp3);
                mpz_add(tmp2, tmp2, inf);
            }

            t = mpz_get_si(tmp);
        }
        else
        {
            t = mpz_get_si(number);
        }

        //gmp_printf("i : %d,\tt : %d,\tnb : %Zd\n", start, t, number);
 //printf("start : %d \n", t);
        word[k] = charset[t];
        //printf("%d : %s\n", start, word);
        /* mpz_sub_ui(number, number, ((t+1)*(long)pow(base, start))); */
        mpz_ui_pow_ui(tmp3, base, start);
        mpz_mul_ui(tmp3, tmp3, t+1);
        mpz_sub(number, number, tmp3);
        start--;
        k++;
        mpz_set_si(inf, 0);

    }while(start >= 0);

    word[nb_char] = '\0';

    //gmp_printf("%s \n", word);
    mpz_clear(tmp);
    mpz_clear(tmp2);
    mpz_clear(tmp3);
    mpz_clear(inf);
    mpz_clear(number);

    return word;
}

void nextword(char* charset, int base, char* word)
{
    int i, j, len;
    char crt;

    len = strlen(word);

    for(i = len-1; i >= 0; i--)
    {
        crt = word[i];

        for(j=0; j<base; j++)
        {
            if(charset[j] == crt)
            {
                break;
            }
        }

        if(j<base-1)
        {
            word[i] = charset[j+1];
            break;
        }
        else
        {
            if(i!=0)
            {
                word[i] = charset[0];
            }
            else
            {
                for(j = 0; j <= len; j++)
                {
                    word[j] = charset[0];
                }
                word[len+1] = '\0';
            }
        }
    }
}
