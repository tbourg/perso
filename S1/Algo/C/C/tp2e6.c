#include<stdio.h>

void main()
{
    int time,h,m,s;
    printf("entrezz un moment sous la forme HHMMSS\n");
    scanf("%d",&time);
    s=time%100;
    time/=100;
    m=time%100;
    h=time/100;
    s=s+1;
    if(s==60)
    {
        s=0;
        m=m+1;
    }
    if(m==60)
    {
        m=0;
        h=h+1;
    }
    printf("ensuite vous avez %02d secondes %02d minutes et %02d heures",s,m,h);
}
