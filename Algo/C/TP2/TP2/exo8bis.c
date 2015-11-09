#include<stdio.h>


int main()
{
    char chiffre;
    do
    {
    printf("saisir un chiffre :");
    scanf(" %c",&chiffre);
    if(chiffre == '0')
    	printf("zero\n");
    else if(chiffre == '1')
    	printf("un\n");
    else if(chiffre == '2')
    	printf("deux\n");
    else if(chiffre == '3')
    	printf("trois\n");
    else if(chiffre == '4')
    	printf("quatre\n");
    else if(chiffre == '5')
    	printf("cinq\n");
    else if(chiffre == '6')
    	printf("six\n");
    else if(chiffre == '7')
    	printf("sept\n");
    else if(chiffre == '8')
    	printf("huit\n");
    else if(chiffre == '9')
    	printf("neuf\n");
    else if(chiffre == '#')
    	printf("fin\n");
    else
    	printf("erreur saisie\n");
    }
    while(chiffre!='#');
    return 0;
}
