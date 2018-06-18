#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define NRA 550
#define NCA 550
#define NCB 550

int main(int argc, char *argv[]) 
{
    clock_t begin, end;
    int i, j, k;
    double a[NRA][NCA], b[NCA][NCB], c[NRA][NCB];
    
    for (i = 0; i < NRA; i++) 
    {
        for (j = 0; j< NCA; j++)
            a[i][j] = i + j;
    }

    for (i = 0; i < NCA; i++)
    {
        for (j = 0; j < NCB; j++)
            b[i][j] = i * j;
           // b[i][j] = i + j;
    }
    begin = clock();
    for (k = 0; k < NCB; k++)
        for (i = 0; i < NRA; i++)
        {
            c[i][k] = 0.0;
            for ( j = 0; j < NCA; j++)
                c[i][k] = c[i][k] + a[i][j] * b[j][k];
        }
    end = clock();
    printf("[4][5] %6.2f   \n", c[4][5]);

    for (i = 0; i < NRA; i++)
    {
        printf("\n");
        for (j = 0; j < NCB; j++)
            printf("%6.2f   \n ", c[i][j]);
    }
    printf("%fs",(double)(end-begin)/CLOCKS_PER_SEC);
    printf("\n");
 
    return 0;

}

