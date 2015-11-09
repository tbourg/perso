#include<stdio.h>

void main()
{
    int duree=0;
    scanf("%i",&duree);
    int a=duree%100;
    duree=duree/100;
    int m=duree%100;
    duree=duree/100;
    int j=duree;
    printf("%i-%i-20%i",j,m,a);
}
