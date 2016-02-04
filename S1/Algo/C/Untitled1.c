//#include<stdio.h>
//#include<string.h>

/*void retirer_chariot(char *chaine)
/*
:entree/sortie chaine : chaine de caracteres
:pre-cond : chaine contient le caractere '\n' avant le caractere '\0'
:post-cond : supprime '\n' de chaine en le remplaçant par '\0'

{
 int i=0;
 while(chaine[i]!='\n') i++;
 chaine[i]='\0';
}

void permuter(char *chaine, int i, int j)
/*
:entree/sortie chaine : chaine de caracteres
:entree i : entier
:entree j : entier
:pre-cond : i et j sont des indices de caracteres de chaine
:post-cond : inverse les caracteres d'indice i et j dans chaine

{
 char aux=chaine[i];
 chaine[i]=chaine[j];
 chaine[j]=aux;
}



void anagrammes(char *chaine, int i)
/*
:entree/sortie chaine : chaine de caracteres
:entree i : entier
:post-cond : affiche les anagrammes de chaine a partir du caractere d'indice i

{
 if(chaine[i]=='\0') printf("%s\n",chaine);
 else
  {
    int j=i;
    while(chaine[j]!='\0')
       {
        permuter(chaine,i,j);
        anagrammes(chaine,i+1);
        permuter(chaine,i,j);
        j++;
       }
   }
}
*/

int main()
{
    char chaine1[100];
    printf("Saisir une chaine de caracteres : ");
    //fgets(chaine1,100,stdin);
    /*retirer_chariot(chaine1);
    anagrammes(chaine1,0);*/
    return 0;
}

