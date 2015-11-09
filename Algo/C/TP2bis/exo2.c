#include<stdio.h>

int factorielle(int m)
/*
:entree m : int
:precondition : m>=0
:postcondition : retourne m!
*/
{
     int i,f=1;
     for(i=2;i<=m;i++) f*=i;
     return f;
}

int main()
{
    int n;
    printf("\nDonner un entier positif : ");
    scanf("%d",&n);
    while(n>=0)
    {
    printf("%d!=%d\n",n,factorielle(n));
    printf("\nDonner un entier positif : ");
    scanf("%d",&n);
    }
    printf("%d est un entier negatif\n",n);
    
    return 0;
}
