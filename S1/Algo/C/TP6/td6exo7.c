#include<stdio.h>
#include<stdlib.h>

int robuste_entier()
/*
:post-cond : permet la saisie d'un entier en gerant les erreurs de saisie
*/
{
 int n;
 char chaine[100];
 char *pt=NULL;
 do
 {
   printf("saisir un entier : ");
   scanf("%s",chaine);
   n=strtol(chaine,&pt,10);
 }
 while(pt[0]!='\0');
 return n;
}

int main()
{

    printf("entier saisi : %d\n",robuste_entier());
    return 0;
}
