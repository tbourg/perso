#include<stdio.h>
#include<math.h>


int nb_solutions (double a, double b, double c)
/*
:entres a,b,c : double
:postcondition : retourne le nombre de solutions reelles de l'equation du second degre ax2+bx+c=0
		 	  ou -1 si le nombre de solutions est infini 
*/
{
    double delta;
    if(a==0)
      {
          if(b==0)
            {
                  if(c==0) return -1; // infinite de solutions
                  else return 0;
            }
          else return 1;
      }
    else
    {
        delta=b*b-4*a*c;
     if(delta<0) return 0;
     else if(delta==0) return 1;
     else return 2;
   }
}   

int main()
{
    double a,b,c;
    int nb;
    printf("Donner les coeff de l equation du second degre ax2+bx+c=0 : ");
    scanf("%lf %lf %lf",&a,&b,&c);
    nb=nb_solutions(a,b,c);
    if(nb==-1) printf("Une infinite de solutions\n");
    else printf("%d solution(s)\n",nb);
    return 0;
}