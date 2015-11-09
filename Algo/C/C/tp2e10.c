#include<stdio.h>

void main()
{
    int date,jj,mois,an;
    printf("entrez une annee sous la forrme JJMMYY\n");
    scanf("%d",&date);
    an=date%100;
    date/=100;
    mois=date%100;
    jj=date/100;
    printf("%02d ",jj);
    switch(mois)
    {
        case 1:printf("janvier");
        break;
        case 2:printf("fevrier");
        break;
        case 3:printf("mars");
        break;
        case 4:printf("avril");
        break;
        case 5:printf("mai");
        break;
        case 6:printf("juin");
        break;
        case 7:printf("juillet");
        break;
        case 8:printf("aout");
        break;
        case 9:printf("septembre");
        break;
        case 10:printf("octobre");
        break;
        case 11:printf("novembre");
        break;
        case 12:printf("decembre");
        break;

    }
    printf(" 20%02d",an);

    }

