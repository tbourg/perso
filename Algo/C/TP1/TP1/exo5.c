#include<stdio.h>

int main()
{
 float R;
 printf("Saisir le rayon du cercle : ");
 scanf("%f",&R);
 printf("Pour le cercle de rayon = %f : diametre = %f, circonference = %f et aire = %f\n", R, 2*R, 2*R*3.14, R*R*3.14); 
 return 0;
}
