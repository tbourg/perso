#include<stdio.h>
#include<stdlib.h>

double robuste_flottant()
/*
:post-cond : permet la saisie d'un flottant en gerant les erreurs de saisie
*/
{
 double n;
 char chaine[100];
 char *pt;
 do
 {
   printf("saisir un flottant : ");
   scanf("%s",chaine);
   n=strtod(chaine,&pt);
 }
 while(pt[0]!='\0');
 return n;
}

int main()
{

    printf("flottant saisi : %lf\n",robuste_flottant());
    return 0;
}