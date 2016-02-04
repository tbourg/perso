#include<stdio.h>

int main()
{
    int annee=0;
    printf("\nDonner une annee entre 1512 et 3999 : ");
    scanf("%d",&annee);
    while(annee>=1512 && annee<=3999)
    {
    if((annee%4==0 && annee%100!=0) || annee%400==0) printf("%d est bissextile\n",annee);
    else printf("%d n est pas bissextile\n",annee);
    annee=0;
    printf("\nDonner une annee entre 1512 et 3999 : ");
    scanf("%d",&annee);
    }
    printf("fin\n");
    return 0;
}
