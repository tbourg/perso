#include<stdio.h>

void main()
{
    int i,nb;
    printf("entrez un nombre\n");
    scanf("%d",&nb);

    if(nb<<0) printf("vous faites erreur");
    for (i=1;i<=10;i++)
    {
        printf("%d x %d = %d\n",i,nb,i*nb);
    }
}
