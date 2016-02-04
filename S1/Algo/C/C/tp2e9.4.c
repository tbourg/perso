#include<stdio.h>
#include<math.h>

void main()
{
    float annee=2000;
    do
    {
        printf("\nentrez une année entre 1512 et 3999\n");
        scanf("%f",&annee);
        if (((annee/4==floor(annee/4)) && (annee/100!=floor(annee/100))) || (annee/400==floor(annee/400)))
        {
            printf("l'année est bissextile");
        }
        else
        {
            printf("l'année n'est pas bissextile");
        }
    }
    while(annee>=1512 && annee<=3999);
}

