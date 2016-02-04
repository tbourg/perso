#include<stdio.h>

void echange(float *p, float *q)
/*
:entrée p : adresse d'un float
:entrée q : adresse d'un float
:postcondition : échange les valeurs de type float dont p et q sont les adresses 
*/
{
 float z;
 z=*p;
 *p=*q;
 *q=z;
}


int main()
{
 float a=3.5, b=4.6;
 printf("avant : a=%f et b=%f\n",a,b);
 echange(&a,&b);
 printf("apres : a=%f et b=%f\n",a,b);
 return 0;
}
