#include<stdio.h>

void main()
{
int u,v,somme_u=0,somme_v=0,n;
for(n=1;n<=10;n++)
{
    u=3*n+5;
    v=(3*n*n+5)/(n*n*n+1);
    somme_u+=u;
    somme_v+=v;
    printf("u=%d et v=%d \n",u,v);
}
printf("somme des u=%d et somme des v=%d",somme_u,somme_v);
}
