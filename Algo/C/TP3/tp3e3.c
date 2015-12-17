#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void remplir(int tab[],int nbelt);

void affiche(int tab[],int nbelt);

void permute(int tab[],int nbelt);

void main()
{
    int tab[20];
    remplir(tab,20);
    affiche(tab,20);
    printf("\n\n");
    permute(tab,20);
    affiche(tab,20);
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

void permute(int tab[],int nbelt)
/*
entree : tab un tableau et nbelt un int
precond : tab a nbelt elements
postcond : inverse les valeurs
*/
{
    int k,tabt[nbelt],j;
    for(k=nbelt-1;k>=0;k--) tabt[nbelt-1-k]=tab[k];
    for(j=0;j<nbelt;j++) tab[j]=tabt[j];

}
