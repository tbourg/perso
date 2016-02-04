#include<stdio.h>

int main()
{
 int date,j,m,a;
 printf("Donner une date : ");
 scanf("%d",&date);
 a=date%100;
 m=(date/100)%100;
 j=(date/100)/100;
 printf("%02d-%02d-20%02d\n",j,m,a);
 return 0;
}
