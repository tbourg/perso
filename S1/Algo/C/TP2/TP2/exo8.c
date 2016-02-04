#include<stdio.h>


int main()
{
    char chiffre;
    do
    {
    printf("saisir un chiffre :");
    scanf(" %c",&chiffre);
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
                  break;
       case '#' : printf("fin\n");
                  break;
       default : printf("erreur saisie\n");
     }
    }
    while(chiffre!='#');
    return 0;
}
