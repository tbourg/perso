#include<stdio.h>

int main()
{
 float L, l;
 printf("Saisir la longueur du rectangle : ");
 scanf("%f",&L);
 printf("Saisir la largeur du rectangle : ");
 scanf("%f",&l);
 printf("perimetre = %f et surface = %f\n", 2*(L+l), l*L); 
 return 0;
}
