#include<stdio.h>
#include<stdlib.h>

void remplir(int tab[], int taille)
/*
:entrée/sortie tab : tableau d'int
:entrée taille : int
:pré-condition : tab contient taille éléments
:postcondition : remplit tab avec des valeurs générées aléatoirement comprises entre 0 et 99 
*/
{
     int i;
     for(i=0;i<taille;i++) tab[i]=rand()%100;
}


void afficher(int tab[], int taille)
/*
:entrée tab : tableau d'int
:entrée taille : int
:pré-condition : tab contient taille éléments
:postcondition : affiche à l'écran les éléments de tab
*/
{
     int i;
     for(i=0;i<taille;i++) printf("%d ",tab[i]);
     printf("\n");
}


void afficher_envers(int tab[], int taille)
/*
:entrée tab : tableau d'int
:entrée taille : int
:pré-condition : tab contient taille éléments
:postcondition : affiche à l'écran les éléments de tab dans l'ordre inverse (en partant du dernier)
*/
{
     int i;
     for(i=taille-1;i>=0;i--) printf("%d ",tab[i]);
     printf("\n");
}


void permuter(int tab[], int taille)
/*
:entrée/sortie tab : tableau d'int
:entrée taille : int
:pré-condition : tab contient taille éléments
:postcondition : inverse les éléments de tab en permutant le premier élément avec le dernier, le second avec l'avant dernier, et ainsi de suite 
*/
{
     int i=0,j=taille-1,var;
     while(i<j)
       {
         var=tab[i];
         tab[i]=tab[j];
         tab[j]=var;
         i++;
         j--;
       }
}

int maximum(int tab[], int taille)
/*
:entrée tab : tableau d'int
:entrée taille : int
:pré-condition : tab contient taille éléments
:postcondition : retourne la valeur maximum de tab 
*/
{
    int i,max=tab[0];
    for(i=1;i<taille;i++)
     {
       if(tab[i]>max) max=tab[i];
     }
    return max;
}

void maxmin_aff(int tab[], int taille)
/*
:entrée tab : tableau d'int
:entrée taille : int
:pré-condition : tab contient taille éléments
:postcondition : affiche à l'écran les valeurs maximum et minimum de tab 
*/
{
    int i,max=tab[0],min=tab[0];
    for(i=1;i<taille;i++)
     {
       if(tab[i]>max) max=tab[i];
       if(tab[i]<min) min=tab[i];
     }
    printf("max_aff = %d  et min_aff = %d\n",max,min);
}


void maxmin_trans(int tab[], int taille, int mm[])
/*
:entrée tab : tableau d'int
:entrée taille : int
:pré-condition : tab contient taille éléments
:entrée/sortie mm : tableau d'int
:pré-condition : mm contient 2 éléments
:postcondition : mm[0]= valeur maximum de tab et mm[1]= valeur minimum de tab 
*/
{
    int i,max=tab[0],min=tab[0];
    for(i=1;i<taille;i++)
     {
       if(tab[i]>max) max=tab[i];
       if(tab[i]<min) min=tab[i];
     }
    mm[0]=max;
    mm[1]=min;
}


void maxmax_trans(int tab[], int taille, int mm[])
/*
:entrée tab : tableau d'int
:entrée taille : int
:pré-condition : tab contient taille éléments
:entrée/sortie mm : tableau d'int
:pré-condition : mm contient 2 éléments
:postcondition : mm[0]= valeur du plus grand élément de tab et mm[1]= valeur du deuxieme plus grand élément de tab 
*/
{
    int i,max=0,max2;
    for(i=1;i<taille;i++)
     {
       if(tab[i]>tab[max]) max=i;
     }
    if(max==0) max2=1;
    else max2=0;
    for(i=1;i<taille;i++)
     {
       if(i!=max && tab[i]>tab[max2]) max2=i;
     }
    mm[0]=tab[max];
    mm[1]=tab[max2];
}


void tri(int tab[], int taille)
/*
:entrée/sortie tab : tableau d'int
:entrée taille : int
:pré-condition : tab contient taille éléments
:postcondition : trie tab par la méthode du tri selection
*/
{
     int i,j,min,var;
     for(i=0;i<taille-1;i++)
       {
         min=i;
         for(j=i+1;j<taille;j++)
           {
             if(tab[j]<tab[min]) min=j;
           }
         var=tab[i];
         tab[i]=tab[min];
         tab[min]=var;
       }
}

int main()
{
    int tableau[20];
    int minmax[2];
    int maxmax[2];
    int choix=1;

    while(choix!=0)
     {
       switch(choix)
        {
          case 1 : remplir(tableau, 20); break;
          case 2 : afficher(tableau, 20); break;
          case 3 : afficher_envers(tableau, 20); break;
          case 4 : permuter(tableau,20); afficher(tableau, 20); break;
          case 5 : printf("max = %d\n",maximum(tableau,20)); break;
          case 6 : maxmin_trans(tableau,20,minmax); printf("max_trans = %d et min_trans = %d\n",minmax[0],minmax[1]); break;
          case 7 : maxmax_trans(tableau,20,maxmax); printf("max_trans = %d et max2_trans = %d\n",maxmax[0],maxmax[1]); break;
    	  case 8 : tri(tableau,20); afficher(tableau, 20); break;
    	  case 0 : printf("fin\n"); break;
    	  default : printf("erreur de saisie\n");
    	}
       printf(" 1. Remplir \n 2. Afficher \n 3. Afficher a l envers\n 4. Permuter\n 5. Affciher le maximum\n 6. Afficher maximum et minimum\n 7. Afficher les deux valeurs maximales\n 8. Trier\n 0. Quitter\n Choix = ");
       scanf("%d",&choix);
     }

    return 0;
}
