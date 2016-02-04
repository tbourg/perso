#include<stdio.h>

void echange(int *p, int *q)
/*
:entrée p : adresse d'un int
:entrée q : adresse d'un int
:postcondition : échange les valeurs de type int dont p et q sont les adresses 
*/
{
 int z;
 z=*p;
 *p=*q;
 *q=z;    
}


int main()
{
 int a=3, b=5;
 printf("avant : a=%d et b=%d\n",a,b);
 echange(&a,&b);
 printf("apres : a=%d et b=%d\n",a,b);
 return 0;   
}
