#include<stdio.h>

void main()
{
    int x=0;
    int y=0;
    int nb1=0;
    int nb2=0;
    int nb3=0;
    int nb4=0;
    int nb5=0;
    scanf("%i",&x);
    scanf("%i",&y);
    nb1=9*x*x*x*x;
    nb2=y*y*y*y;
    nb3=2*y*y;
    nb4=3*x*x+y*y-1;
    nb5=3*x*x-y*y+1;
    int res1=nb1-nb2+nb3;
    int res2=nb4*nb5+1;
    printf("%i-%i-%i-%i-%i\n",nb1,nb2,nb3,nb4,nb5);
    printf("%i-%i",res1,res2);
}
