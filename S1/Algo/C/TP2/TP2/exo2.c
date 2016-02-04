#include<stdio.h>


int main()
{
    int n=-1,i,f=1;
    printf("\nDonner un entier positif : ");
    scanf("%d",&n);
    if(n>=0)
    {
    for(i=2;i<=n;i++) f*=i;
    printf("%d!=%d\n",n,f);
    }
    else printf("erreur de saisie\n");
    return 0;
}
