#include<stdio.h>
#include<string.h>

void main()
{
    char chaineavec[200],chainesans[200];
    int nbEspace=0,i=0,decal,length;
    printf("entrez un texte\n");
    scanf("%[^\n]",chaineavec);
    length=strlen(chaineavec);

    /*do
    {
        if(chaineavec[i]==32)
        {
            nbEspace++;
            for(decal==i;decal<=length;decal++)
            {
                chaine[decal]=chaine[decal+1];
            }
            length--;

        }
        i++;
    }
    while(i<=length);
    printf("%d",nbEspace);
    printf("%s",chaine);
    */

    for (i = 0; i<=length; i++)
    {
        if(chaineavec[i]!=32)
        {
            chainesans[i-nbEspace]=chaineavec[i];
        }
        else
        {
            nbEspace++;
        }
    }
    printf("%s",chainesans);
}

