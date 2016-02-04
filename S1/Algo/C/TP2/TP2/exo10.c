#include<stdio.h>

int main()
{
    int date=-1,jj,mois,an;
    printf("Donner une date : ");
    scanf("%d",&date);
    an=date%100;
    mois=(date/100)%100;
    jj=(date/100)/100;
    if(jj>=1 && jj<=31 && mois>=1 && mois<=12)
    {
    switch(mois)
     {
       case 1 : printf("%d Janvier 20%02d\n",jj,an);
                break;
       case 2 : printf("%d Fevrier 20%02d\n",jj,an);
                break;
       case 3 : printf("%d Mars 20%02d\n",jj,an);
                break;
       case 4 : printf("%d Avril 20%02d\n",jj,an);
                break;
       case 5 : printf("%d Mai 20%02d\n",jj,an);
                break;
       case 6 : printf("%d Juin 20%02d\n",jj,an);
                break;
       case 7 : printf("%d Juillet 20%02d\n",jj,an);
                break;
       case 8 : printf("%d Aout 20%02d\n",jj,an);
                break;
       case 9 : printf("%d Septembre 20%02d\n",jj,an);
                break;
       case 10 : printf("%d Octobre 20%02d\n",jj,an);
                break;
       case 11 : printf("%d Novembre 20%02d\n",jj,an);
                break;
       case 12 : printf("%d Decembre 20%02d\n",jj,an);
     }
    }
    else printf("erreur de saisie\n");
    return 0;
}
