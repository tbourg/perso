#include<stdio.h>

int main()
{
    int n=-1,i;
    printf("\nDonner un entier positif : ");
    scanf("%d",&n);
    while(n>=0)
    {
    for(i=1;i<=10;i++) printf("%d*%d=%d\n",i,n,i*n);
    n=-1;
    printf("\nDonner un entier positif : ");
    scanf("%d",&n);
    }
    printf("fin\n");
    return 0;
}
