#include<stdio.h>


int main()
{
    int n,su=0;
    float sv=0;
    for(n=0;n<=9;n++)
      {
        printf("u%d=%d \t v%d=%f\n",n,3*n+5,n,(3*n*n+5)/(n*n*n+1.));
        su+=3*n+5;
        sv+=(3*n*n+5)/(n*n*n+1.);
      }
    printf("su=%d  \t sv=%f\n",su,sv);
    return 0;
}
