#include<stdio.h>
#include<math.h>

int main()
{
    double a,b,c,delta;
    printf("Donner les coeff de l equation du second degre ax2+bx+c=0 : ");
    scanf("%lf %lf %lf",&a,&b,&c);
    if(a==0)
      {
          if(b==0)
            {
                  if(c==0) printf("Une infinite de solutions\n");
                  else printf("Pas de solutions\n");
            }
          else printf("Une solution : x1=x2=%lf\n",-c/b);
      }
    else
    {
        delta=b*b-4*a*c;
     if(delta<0) printf("Pas de solutions reelles\n");
     else if(delta==0) printf("Une solution : x1=x2=%lf\n",(-b/(2*a)));
     else printf("Deux solutions : x1=%lf et x2=%lf\n",(-b-sqrt(delta))/(2*a),(-b+sqrt(delta))/(2*a));
   }

    return 0;
}
