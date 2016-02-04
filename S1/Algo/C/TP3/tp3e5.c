#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void remplir(int tab[],int nbelt);

void affiche(int tab[],int nbelt);

int ext(int tab[],int nbelt);

void main()
{
    int tab[20];
    remplir(tab,20);
    affiche(tab,20);
    printf("\n\n");
    ext(tab,20);
}

void remplir(int tab[],int nbelt)
/*
entree : tab un tableau
    taille un int
precond : tab un tableau de taille elements
postcond : remplit avec des nbres entre 0 et 99
*/
{
    int i;
    srand(time(NULL));
    for(i=0;i<nbelt;i++) tab[i]=rand()%100;
}

void affiche(int tab[],int nbelt)
/*
entree : tab un tableau et nbelt un int
precond : tab a nbelt elements
postcond : affiche nbelt int
*/
{
    int j;
    for(j=0;j<nbelt;j++) printf("%d ",tab[j]);
}

int ext(int tab[],int nbelt)
/*
entree : tab un tableau et nbelt un int
precond : tab a nbelt elements
postcond : ecrit les extremums
*/
{
    int k=0,tabt[nbelt],j;
    for(j=0;j<nbelt;j++) if(k<tab[j])k=tab[j];
    printf("%d ",k);
    int l=99,jj;
    for(jj=0;jj<nbelt;jj++) if(l>tab[jj])l=tab[jj];
    printf("%d",l);
}
