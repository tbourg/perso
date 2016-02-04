#include<stdio.h>

void main()
{
    float lon=0;
    float lar=0;
    printf("Entrez la longueur et la largeur:");
    scanf("%f",&lon);
    scanf("%f",&lar);
    float per = 2*lon+2*lar;
    float aire = lon*lar;
    printf("l'aire est %f cm² et le perimetre est %f cm",aire,per);
}
