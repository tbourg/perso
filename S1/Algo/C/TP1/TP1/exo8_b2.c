#include<stdio.h>

int main()
{
 double x, y, nb1, nb2, nb3, resultat1, nb4, nb5, resultat2;
 x=10864;
 y=18817;
 nb1=9*x*x*x*x;
 printf("nb1 = %lf\n",nb1);
 nb2=y*y*y*y;
 printf("nb2 = %lf\n",nb2);
 nb3=2*y*y;
 printf("nb3 = %lf\n",nb3);
 resultat1= nb1-nb2+nb3; 
 printf("resultat1 = %lf\n",resultat1);
 nb4=3*x*x+y*y-1;
 printf("nb4 = %lf\n",nb4);
 nb5=3*x*x-y*y+1;
 printf("nb5 = %lf\n",nb5);
 resultat2=nb4*nb5+1;
 printf("resultat2 = %lf\n",resultat2);
 return 0;
}
