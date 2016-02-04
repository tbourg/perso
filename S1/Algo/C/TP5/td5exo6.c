#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void remplir(int tab[], int taille)
/*
:entrée/sortie tab : tableau d'int
:entrée taille : int
:pré-condition : tab contient taille éléments
:postcondition : remplit tab avec des valeurs générées aléatoirement comprises entre 0 et 99
*/
{
     int i;
     printf("remplir : sizeof(tab) = %d\n",sizeof(tab));
     for(i=0;i<taille;i++) tab[i]=rand()%100;
}

void remplir2(int *p, int taille)
/*
:entrée p : adresse d'un int
:entrée taille : int
:pré-condition : p est l'adresse du premier élément d'un tableau qui contient taille éléments de type int
:postcondition : remplit le tableau dont l'adresse du premier élément est p avec des valeurs générées aléatoirement comprises entre 0 et 99
*/
{
     int i;
     printf("remplir2 : sizeof(p) = %d\n",sizeof(p));
     for(i=0;i<taille;i++) p[i]=rand()%100;
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
     for(i=0;i<taille;i++) printf("%d ",*(tab+i));
     printf("\n");
}

void afficher2(int *p, int taille)
/*
:entrée p : adresse d'un int
:entrée taille : int
:pré-condition : p est l'adresse du premier élément d'un tableau qui contient taille éléments de type int
:postcondition : affiche à l'écran les éléments du tableau dont p est l'adresse du premier élément
*/
{
     int i;
     for(i=0;i<taille;i++) printf("%d ",*(p+i));
     printf("\n");
}


int main()
{
 int tablo[10], *pt;
 srand(time(NULL));
 pt=tablo;
 printf("taille tablo = %d\ntaille pt = %d\n", sizeof(tablo), sizeof(pt));
 remplir(tablo,10);
 afficher(tablo, 10);
 remplir2(tablo,10);
 afficher2(tablo, 10);
 remplir(pt,10);
 afficher(pt, 10);
 remplir2(pt,10);
 afficher2(pt, 10);
 return 0;
}
