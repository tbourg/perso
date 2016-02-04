#include<stdio.h>
#include<string.h>

void main()
{
    char chaine[200];
    int nbDeL=0,i;
    printf("entrez un texte\n");
    scanf("%s",chaine);
    for (i = 0; i<=strlen(chaine); i++)
    {
        if(chaine[i]=='l')
        {
            nbDeL++;
        }
        printf("%d \n",nbDeL);
    }
    printf("il y a %d lettre l",nbDeL);
}
