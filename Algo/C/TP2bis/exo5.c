#include<stdio.h>

void lettres(char chiffre)
/*
:entree chiffre : char
:precondition : '0'<=chiffre<='9'
:postcondition : affiche l'ecriture de chiffre avec des lettres
*/
{
     switch(chiffre)
     {
       case '1' : printf("un\n");
                  break;
       case '2' : printf("deux\n");
                  break;
       case '3' : printf("trois\n");
                  break;
       case '4' : printf("quatre\n");
                  break;
       case '5' : printf("cinq\n");
                  break;
       case '6' : printf("six\n");
                  break;
       case '7' : printf("sept\n");
                  break;
       case '8' : printf("huit\n");
                  break;
       case '9' : printf("neuf\n");
     }
}


int main()
{
    char chiffre;
    do
    {
    printf("Donner un chiffre (ou # pour terminer) :");
    scanf(" %c",&chiffre);
    if(chiffre>='0' && chiffre<='9') lettres(chiffre);
    }
    while(chiffre!='#');

    return 0;
}
