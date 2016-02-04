#include<stdio.h>

int bissextile(int annee)
/*
:entree annee : int
:precondition : 1512<=annee<=3999
:postcondition : retourne 1 si annee est bissextile ou 0 sinon
*/
{
     if((annee%4==0 && annee%100!=0) || annee%400==0) return 1;
     return 0;
}

int main()
{
    int annee;
    printf("\nDonner une annee entre 1512 et 3999 : ");
    scanf("%d",&annee);
    while(annee>=1512 && annee<=3999)
    {
    if(bissextile(annee)==1) printf("%d est bissextile\n",annee);
    else printf("%d n est pas bissextile\n",annee);
    
    printf("\nDonner une annee entre 1512 et 3999 : ");
    scanf("%d",&annee);
    }
    printf("%d n est pas compris entre 1512 et 3999\n",annee);
 
    return 0;
}
