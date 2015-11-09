#include<stdio.h>

void main()
{
    int i,nb=0;
     do
    {
        printf("entrez un nombre\n");
        scanf("%d",&nb);


        for (i=1;i<=10;i++)
        {
            printf("%d x %d = %d\n",i,nb,i*nb);
        }
    }
    while(nb>=0);
}

