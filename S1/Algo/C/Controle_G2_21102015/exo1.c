#include<stdio.h>

int main()
{
 int n, i, j;
 printf("Saisir un entier superieur ou egal a 2 : ");
 scanf("%d",&n);
 while(n >= 2)
  {
    for(i=0; i<n; i++)
        {
            for(j=0; j<n; j++)
                printf("A");
            printf("\n");
        }
    printf("Saisir un entier superieur ou egal a 2 pour continuer (inferieur a 2 pour sortir) : ");
    scanf("%d",&n);
  }
 return 0;
}
