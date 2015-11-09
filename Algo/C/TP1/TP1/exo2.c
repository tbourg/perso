#include<stdio.h>

int main()
{
 int x=2, y=3, z;
 printf("avant : x = %d et y = %d\n",x,y);
 z=x;
 x=y;
 y=z;
 printf("apres : x = %d et y = %d\n",x,y); 
 return 0;
}
