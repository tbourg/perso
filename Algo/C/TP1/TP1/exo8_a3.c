#include<stdio.h>

int main()
{
 long long x, y, nb1, nb2, nb3, resultat1, nb4, nb5, resultat2;
 x=408855776;
 y=708158977;
 nb1=9*x*x*x*x;
 printf("nb1 = %lld\n",nb1);
 nb2=y*y*y*y;
 printf("nb2 = %lld\n",nb2);
 nb3=2*y*y;
 printf("nb3 = %lld\n",nb3);
 resultat1= nb1-nb2+nb3;
 printf("resultat1 = %lld\n",resultat1);
 nb4=3*x*x+y*y-1;
 printf("nb4 = %lld\n",nb4);
 nb5=3*x*x-y*y+1;
 printf("nb5 = %lld\n",nb5);
 resultat2=nb4*nb5+1;
 printf("resultat2 = %lld\n",resultat2);
 return 0;
}
