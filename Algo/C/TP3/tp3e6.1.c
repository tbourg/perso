#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void remplir(int tab[],int nbelt);

void affiche(int tab[],int nbelt);

int max(int tab[],int nbelt,int tbmax[]);

void main()
{
    int tab[20],m1,tbmax[2];
    remplir(tab,20);
    affiche(tab,20);
    printf("\n\n");
    max(tab,20,tbmax);
    affiche(tbmax,2);
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

int max(int tab[],int nbelt,int tbmax[])
/*
entree : tab un tableau et nbelt un int et tbmax un tableau vide
precond : tab a nbelt elements
postcond : stocke les maxs dans tbmax
*/
{
    int k=0,tabt[nbelt],j,l=0,m,p;
    for(j=0;j<nbelt;j++)
    {
       if(k<tab[j])
        {
            k=tab[j];
            p=j;
        }
    }
    for(m=0;m<nbelt;m++)
    {
        if(l<tab[m] && m!=p)
        {
            l=tab[m];
        }
    }
    tbmax[0]=k;
    tbmax[1]=l;
}



