#include<stdio.h>
#include<string.h>

void main()
{
    char chaine[200]="";
    int nbDeL=0,i;
    printf("entrez un texte\n");
    scanf("%[^\n]",chaine);
    for (i = 0; i<=strlen(chaine); i++)
    {
        if(chaine[i]<=90 && chaine[i]>=65)
        {
            chaine[i]+=32;
        }
        else if(chaine[i]>=97 && chaine[i]<=122)
        {
            chaine[i]-=32;
        }
        printf("%s \n",chaine);
    }

}

