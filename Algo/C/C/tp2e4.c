#include<stdio.h>
#include<math.h>

void main()
{
    float annee;
    printf("entrez une ann�e entre 1512 et 3999\n");
    scanf("%f",&annee);
    if (((annee/4==floor(annee/4)) && (annee/100!=floor(annee/100))) || (annee/400==floor(annee/400)))
    {
        printf("l'ann�e est bissextile");
    }
    else
    {
        printf("l'ann�e n'est pas bissextile");
    }
}
