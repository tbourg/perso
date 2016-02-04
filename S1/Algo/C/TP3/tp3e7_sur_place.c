#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void remplir(int tab[],int nbelt);

void affiche(int tab[],int nbelt);

void tri(int tab[],int nbelt);

void main()
{
    int tab[20],m1,tbrange[20];
    remplir(tab,20);
    affiche(tab,20);
    printf("\n\n");
    tri(tab,20);
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
    //srand(time(NULL));
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

void tri(int tab[],int nbelt)
/*
entree : tab un tableau et nbelt un int
precond : tab a nbelt elements
postcond : range les elts
*/
{
    int t,i,k;
    for(t = 1; t < nbelt; t++)
    {
        for(i=0; i < nbelt - 1; i++)
        {
            if(tab[i] > tab[i+1])
            {
                k= tab[i] - tab[i+1];
                tab[i] -= k;
                tab[i+1] += k;
            }
        }
    }

}





