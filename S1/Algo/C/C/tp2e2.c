#include<stdio.h>

void main()
{
    int i,x=1,fact=1;
    printf("entrezz un nombre\n");
    scanf("%d",&i);
    for(x=1;x<=i;x++) fact*=x;
    printf("%d",fact);
}
