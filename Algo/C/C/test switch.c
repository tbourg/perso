#include<stdio.h>

void main()
{
    int i;
    printf("entrez i");
    scanf("%d",&i);
    switch (i)
    {
        case 1:printf("c'est 1");
        case 2:printf("c'est 2");
        default :printf("autre");
    }
}
