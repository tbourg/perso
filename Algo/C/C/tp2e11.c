#include<stdio.h>
#include<string.h>
#include<math.h>

void main()
{
    float a,b,c,discriminant,racine,racine1,racine2;
    printf("Vous avez une equation du type ax²+bx+c=0 \n\nVeuillez entrez la valeur de a:");
    scanf("%f",&a);
    printf("Celle de b:");
    scanf("%f",&b);
    printf("Celle de c:");
    scanf("%f",&c);
    if(a==0)
    {
        printf("a ne peut etre nul");
    }
    else
    {
        discriminant=b*b-4*a*c;
        if(discriminant<0)
        {
            printf("Il n'y a pas de solutions");
        }
        else if(discriminant==0)
        {
            racine=(-b)/2*a;
            printf("Une solution:%f",racine);
        }
        else
        {
            racine1=(-sqrtf(discriminant)-b)/2*a;
            racine2=(-sqrtf(discriminant)+b)/2*a;
            printf("Deux solutions:%f et %f",racine1,racine2);
        }
    }
}
