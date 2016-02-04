#include<stdio.h>
#include<string.h>

void retirer_chariot(char *chaine)
/*
:entree/sortie chaine : chaine de caracteres
:pre-cond : chaine contient le caractere '\n' avant le caractere '\0'
:post-cond : supprime '\n' de chaine en le remplaçant par '\0'
*/
{
 int i=0;
 while(chaine[i]!='\n') i++;
 chaine[i]='\0';
}

int longueur(char *chaine)
/*
:entree chaine : chaine de caracteres
:post-cond : retourne le nombre de caracteres de chaine (sans compter '\0')
*/
{
 int i=0;
 while(chaine[i]!='\0') i++;
 return i;
}


char* copie(char *chaine1, char *chaine2)
/*
:entree/sortie chaine1 : chaine de caracteres
:entree chaine2 : chaine de caracteres
:pre-cond : le tableau cree pour contenir chaine1 peut contenir chaine2
:post-cond : copie chaine2 dans chaine1 et retourne chaine1
*/
{
 int i=0;
 while(chaine2[i]!='\0')
   {
     chaine1[i]=chaine2[i];
     i++;
   }
 chaine1[i]='\0';
 return chaine1;
}

char* concatenation(char *chaine1, char *chaine2)
/*
:entree/sortie chaine1 : chaine de caracteres
:entree chaine2 : chaine de caracteres
:pre-cond : le tableau cree pour contenir chaine1 peut contenir en plus chaine2
:post-cond : concatene chaine1 et chaine2 dans chaine1 et retourne chaine1
*/
{
 int i=0,j=0;
 while(chaine1[i]!='\0') i++;
 while(chaine2[j]!='\0')
   {
     chaine1[i]=chaine2[j];
     i++; j++;
   }
 chaine1[i]='\0';
 return chaine1;
}



char* retourne(char *chaine1)
/*
:entree/sortie chaine1 : chaine de caracteres
:post-cond : inverse les caracteres de chaine1 et retourne chaine1
*/
{
 int i=0,j;
 char aux;
 j=longueur(chaine1)-1;
 while(i<j)
 {
   aux=chaine1[i];
   chaine1[i]=chaine1[j];
   chaine1[j]=aux;
   i++;
   j--;
 }
 return chaine1;
}


int main()
{
    char chaine1[200];
    char chaine2[100];
    printf("Saisir une chaine de caracteres : ");
    fgets(chaine1,100,stdin);
    retirer_chariot(chaine1);
    printf("longeur de la chaine1 : %d\n",longueur(chaine1));
    printf("Saisir une chaine de caracteres : ");
    fgets(chaine2,100,stdin);
    retirer_chariot(chaine2);
    printf("longeur de la chaine2 : %d\n",longueur(chaine2));
    printf("copie dans chaine1 : %s\n",copie(chaine1,chaine2));
    printf("concatenation dans chaine1 : %s\n",concatenation(chaine1,chaine2));
    printf("retourne chaine1 : %s\n",retourne(chaine1));
    return 0;
}
