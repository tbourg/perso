#include<stdio.h>


int main()
{
    int n=-1,h,m,s;
    printf("Donner une heure :");
    scanf("%d",&n);
    s=n%100;
    m=(n/100)%100;
    h=n/10000;
    if(s>=0 && s<60 && m>=0 && m<60 && h>=0 && h<24)
     {
            s++;
            if(s==60)
             {
                     s=0;
                     m++;
                     }
            if(m==60)
             {
                     m=0;
                     h++;
                     }
            if(h==24) h=0;

            printf("%dh-%dmn-%ds\n",h,m,s);
    }
    else printf("erreur saisie\n");
    return 0;
}
