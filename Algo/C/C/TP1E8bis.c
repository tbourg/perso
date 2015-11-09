#include<stdio.h>

void main()
{
    float x=0;
    float y=0;
    float nb1=0;
    float nb2=0;
    float nb3=0;
    float nb4=0;
    float nb5=0;
    scanf("%f",&x);
    scanf("%f",&y);
    nb1=9*x*x*x*x;
    nb2=y*y*y*y;
    nb3=2*y*y;
    nb4=3*x*x+y*y-1;
    nb5=3*x*x-y*y+1;
    float res1=nb1-nb2+nb3;
    float res2=nb4*nb5+1;
    printf("%f-%f-%f-%f-%f\n",nb1,nb2,nb3,nb4,nb5);
    printf("%f-%f",res1,res2);
}

