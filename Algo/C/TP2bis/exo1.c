#include<stdio.h>


void tab_multip(int m)
/*
:entree m : int
:precondition : m>=0
:postcondition : affiche la table de multiplication de m
*/
{
     int i;
     for(i=1;i<=10;i++) printf("%d*%d=%d\n",i,m,i*m);
}

int main()
{
    int n=-1;
    printf("\nDonner un entier positif : ");
    scanf("%d",&n);
    while(n>=0)
    {
    tab_multip(n);
    printf("\nDonner un entier positif : ");
    n=-1;
    scanf("%d",&n);
    }
    printf("%d est un entier negatif\n",n);
    
    return 0;
}
