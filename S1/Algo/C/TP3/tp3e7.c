#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void remplir(int tab[],int nbelt);

void affiche(int tab[],int nbelt);

void tri(int tab[],int nbelt,int tbrange[]);

void main()
{
    int tab[20],m1,tbrange[20]={0};
    remplir(tab,20);
    affiche(tab,20);
    printf("\n\n");
    tri(tab,20,tbrange);
    affiche(tbrange,20);
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

void tri(int tab[],int nbelt,int tbrange[])
/*
entree : tab un tableau et nbelt un int et tbrange un tableau vide
precond : tab a nbelt elements
postcond : range les elts
*/
{

    int j,k=0,l,m=99,p,c,n,i=0;//,tab2[nbelt];
    //for(n=0;n<nbelt;n++) tab2[n]=tab[n];

    /*
    for(j=0;j<nbelt;j++)
    {
       if(k<tab[j])
        {
            k=tab[j];
            c=j;
        }
    }
    /*
    for(m=0;m<nbelt;m++)
    {
        for(n=0;n<nbelt;n++)
        {
            for(l=0;l<nbelt;l++)
            {
                if(tab[l]<tab[c] && tab[l]>tbrange[m-1])
                {
                  c=l;
                }
            }
        }

        tbrange[j]=tab[c];
        tab[c]=100;
    }
    */
    for(i=0;i<nbelt;i++)
    {
        m=99;
        for(l=0;l<nbelt;l++)
        {
            if(m>tab[l])
            {
                m=tab[l];
                c=l;
                printf("%d",m);
            }
        }
        printf("etape%d : %d \n",i,m);
        tbrange[i]=tab[c];
        tab[c]=101;
        affiche(tab,20);
        printf("\n\n");
        affiche(tbrange,20);
        printf("\n\n");
    }
}




