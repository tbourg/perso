#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define li 5
#define co 5

void remplir(int l, int c, int mat[l][c])
/*
:entrée l, c : int
:entrée/sortie mat : tableau d'int à 2 dimensions
:pré-condition : mat contient l éléments sur la première dimension et c sur la seconde
:postcondition : remplit mat avec des valeurs générées aléatoirement comprises entre 0 et 9
*/
{
  srand(time(NULL));

  int i,j;
  for(i=0;i<l;i++)
    {
      for(j=0;j<c;j++) mat[i][j]=rand()%10;
    }
}


void afficher(int l, int c, int mat[l][c])
/*
:entrée l, c : int
:entrée mat : tableau d'int à 2 dimensions
:pré-condition : mat contient l éléments sur la première dimension et c sur la seconde
:postcondition : affiche à l'écran les éléments de mat
*/
{
  int i,j;
  for(i=0;i<l;i++)
    {
      for(j=0;j<c;j++) printf("%d ",mat[i][j]);
      printf("\n");
    }
}


void deuxmin(int l, int c, int mat[l][c], int mm[])
/*
:entrée l, c : int
:entrée mat : tableau d'int à 2 dimensions
:entrée/sortie mm : tableau d'int à 1 dimension
:pré-condition : mat contient l éléments sur la première dimension et c sur la seconde
:pré-condition : mm contient deux cases
:postcondition : mm contient les 2 plus valeurs non necessairement distinctes de mat
*/
{
  int i1=0,j1=0,i2,j2,i,j;
  for(i=0;i<l;i++)
    {
      for(j=0;j<c;j++)
       {
         if(mat[i][j]<mat[i1][j1])
           {
            	i1=i;
            	j1=j;
           }
       }
    }

  if(c>1)
  {
    if(i1==0 && j1==0)
    {
    	i2=0;
    	j2=1;
    }
    else
    {
    	i2=0;
    	j2=0;
    }
  }
  else
  {
    if(i1==0 && j1==0)
    {
    	i2=1;
    	j2=0;
    }
    else
    {
    	i2=0;
    	j2=0;
    }
  }

  for(i=0;i<l;i++)
    {
      for(j=0;j<c;j++)
       {
         if((i!=i1 || j!=j1) && mat[i][j]<mat[i2][j2])
           {
            	i2=i;
            	j2=j;
           }
       }
    }
  mm[0]=mat[i1][j1];
  mm[1]=mat[i2][j2];
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

         if(min != i)
         {
          var=tab[i];
          tab[i]=tab[min];
          tab[min]=var;
         }
       }
}

void tri_lignes(int l, int c, int mat[l][c])
/*
:entrée mat : tableau d'int à 2 dimensions
:entrée l : int
:pré-condition : mat contient l éléments sur les deux dimensions
:postcondition : tri les lignes de mat
*/
{
 int i;
 for(i=0;i<l;i++) tri(mat[i], c);
}


void tri_colonnes(int l, int c, int mat[l][c])
/*
:entrée mat : tableau d'int à 2 dimensions
:entrée l : int
:pré-condition : mat contient l éléments sur les deux dimensions
:postcondition : tri les colonnes de mat
*/
{
 int i,j,k,min,var;
 for(j=0;j<c;j++)
   {
     for(i=0;i<l-1;i++)
       {
         min=i;
         for(k=i+1;k<l;k++)
           {
             if(mat[k][j]<mat[min][j]) min=k;
           }

         if(min != i)
         {
          var=mat[i][j];
          mat[i][j]=mat[min][j];
          mat[min][j]=var;
         }
       }
   }
}


void afficher_char(int l, int c, char mat[l][c])
/*
:entrée l, c : int
:entrée mat : tableau de caractères à 2 dimensions
:pré-condition : mat contient l éléments sur la première dimension et c sur la seconde
:postcondition : affiche à l'écran les éléments de mat
*/
{
  int i,j;
  for(i=0;i<l;i++)
    {
      for(j=0;j<c;j++) printf("%c",mat[i][j]);
      printf("\n");
    }
}

void remplir_char(int l, int c, char mat[l][c])
/*
:entrée l, c : int
:entrée mat : tableau de caractères à 2 dimensions
:pré-condition : mat contient l éléments sur la première dimension et c sur la seconde
:postcondition : remplit mat avec des 'x' sur le bord et des espaces pour le reste
*/
{
  int i,j;
  for(i=0;i<l;i++) // Remplir premiere et derniere colonnes
    {
      mat[i][0] = 'x';
      mat[i][c-1] = 'x';
    }

  for(j=0;j<c;j++) // Remplir premiere et derniere lignes
    {
      mat[0][j] = 'x';
      mat[l-1][j] = 'x';
    }

   for(i=1;i<l-1;i++) // Remplir le reste de la mat
    {
      for(j=1;j<c-1;j++) mat[i][j]= ' ';
    }
}


void divise(int l, int mat[l][l], int A[])
/*
:entrée/sortie mat : tableau d'int à 2 dimensions
:entrée A : tableau d'int
:entrée l : int
:pré-condition : mat contient l éléments sur les deux dimensions
:pré-condition : A contient l éléments
:postcondition : si A[i] divise A[j] mat[i][j]=1, sinon mat[i][j]=0
*/
{
  int i,j;
  for(i=0;i<l;i++)
    {
      for(j=0;j<l;j++)
        {
          if(A[j]%A[i]==0) mat[i][j]=1;
          else mat[i][j]=0;
        }
    }
}


void parite(int l, int mat[l][l], int A[])
/*
:entrée mat : tableau d'int à 2 dimensions
:entrée A : tableau d'int
:entrée l : int
:pré-condition : mat contient l éléments sur les deux dimensions
:pré-condition : A contient l éléments
:postcondition : si A[i] et A[j] ont la même parité mat[i][j]=1, sinon mat[i][j]=0
*/
{
  int i,j;
  for(i=0;i<l;i++)
    {
      for(j=0;j<l;j++)
        {
          if(A[i]%2 == A[j]%2) mat[i][j]=1;
          else mat[i][j]=0;
        }
    }
}


int elleEstBooleenne(int l, int mat[l][l])
/*
:entrée mat : tableau d'int à 2 dimensions
:entrée l : int
:pré-condition : mat contient l éléments sur les deux dimensions
:postcondition : retourne 1 si mat ne contient pas de valeur autre que 0 et 1, 0 sinon
*/
{
  int i,j;
  for(i=0;i<l;i++)
    {
      for(j=0;j<l;j++)
        {
          if(mat[i][j]!=0 && mat[i][j]!=1) return 0;
        }
    }
  return 1;
}


int elleEstTriangulairesup(int l, int mat[l][l])
/*
:entrée mat : tableau d'int à 2 dimensions
:entrée l : int
:pré-condition : mat contient l éléments sur les deux dimensions
:postcondition : retourne 1 si mat est triangulaire supérieure, 0 sinon
*/
{
  int i,j;
  for(i=0;i<l;i++)
    {
      for(j=0;j<i;j++)
        {
          if(mat[i][j]!=0) return 0;
        }
    }
  return 1;
}


int elleEstTriangulaireinf(int l, int mat[l][l])
/*
:entrée mat : tableau d'int à 2 dimensions
:entrée l : int
:pré-condition : mat contient l éléments sur les deux dimensions
:postcondition : retourne 1 si mat est triangulaire inférieure, 0 sinon
*/
{
  int i,j;
  for(i=0;i<l;i++)
    {
      for(j=i+1;j<l;j++)
        {
          if(mat[i][j]!=0) return 0;
        }
    }
  return 1;
}


int elleEstTriangulaire(int l, int mat[l][l])
/*
:entrée mat : tableau d'int à 2 dimensions
:entrée l : int
:pré-condition : mat contient l éléments sur les deux dimensions
:postcondition : retourne 1 si mat est triangulaire, 0 sinon
*/
{
  if(elleEstTriangulaireinf(l, mat)==1 || elleEstTriangulairesup(l, mat)==1) return 1;
  return 0;
}






int main()
{
 int matrice[li][co];
 int minmin[2];
 char matrice2[li][co];
 int matrice3[10][10];
 int matrice4[10][10];
 int matrice5[3][3]={{1,0,0},{2,2,0},{3,3,3}};
 int A[]={1,2,3,4,5,6,7,8,9,10};

 remplir(li,co,matrice);
 afficher(li,co,matrice);
 if(elleEstBooleenne(li, matrice)==1) printf("la matrice est booleenne\n");
 else printf("la matrice n est pas booleenne\n");
 if(elleEstTriangulaire(li, matrice)==1) printf("la matrice est triangulaire\n");
 else printf("la matrice n est pas triangulaire\n");
 deuxmin(li,co,matrice,minmin);
 printf("deux min : %d %d\n",minmin[0],minmin[1]);
 printf("tri des lignes \n");
 tri_lignes(li,co,matrice);
 afficher(li,co,matrice);
 printf("tri des colonnes \n");
 tri_colonnes(li,co,matrice);
 afficher(li,co,matrice);
 printf("matrice de caracteres \n");
 remplir_char(li,co,matrice2);
 afficher_char(li,co,matrice2);
 printf("matrice de la relation divise \n"); 
 divise(10,matrice3,A);
 afficher(10,10,matrice3);
 if(elleEstBooleenne(10, matrice3)==1) printf("la matrice est booleenne\n");
 else printf("la matrice n est pas booleenne\n");
 if(elleEstTriangulaire(10, matrice3)==1) printf("la matrice est triangulaire\n");
 else printf("la matrice n est pas triangulaire\n");
 printf("matrice de la relation parite \n");
 parite(10,matrice4,A);
 afficher(10,10,matrice4);
 if(elleEstBooleenne(10, matrice4)==1) printf("la matrice est booleenne\n");
 else printf("la matrice n est pas booleenne\n");
 if(elleEstTriangulaire(10, matrice4)==1) printf("la matrice est triangulaire\n");
 else printf("la matrice n est pas triangulaire\n");
 afficher(3,3,matrice5);
 if(elleEstBooleenne(3, matrice5)==1) printf("la matrice est booleenne\n");
 else printf("la matrice n est pas booleenne\n");
 if(elleEstTriangulaire(3, matrice5)==1) printf("la matrice est triangulaire\n");
 else printf("la matrice n est pas triangulaire\n");


 return 0;
}
