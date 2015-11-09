#include<stdio.h>

void jma(int date)
/*
:entree date : int
:precondition : date est compose de 6 chiffres et represente une date sous la forme JJMMAA
:postcondition : affiche date sous la forme JJ mois 20AA (le mois etant en toutes lettres)
		 ou erreur si date ne represente pas une date valide
*/
{
    int jj,mois,an;
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
                break;
     }
    }
    else printf("erreur : %d ne represente pas une date valide\n",date);
}

int main()
{
    int d;
    printf("Donner une date : ");
    scanf("%d",&d);
    jma(d);
    return 0;
}
