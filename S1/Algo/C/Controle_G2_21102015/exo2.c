#include<stdio.h>

int main()
{
 int a,b;
 char op;
 printf("Saisir une operation arithmetique : ");
 scanf("%d %c %d",&a,&op,&b);
 if(op == '+')
    printf("%d %c %d = %d\n",a,op,b,a+b);
 else if(op == '-')
    printf("%d %c %d = %d\n",a,op,b,a-b);
 else if(op == '*')
    printf("%d %c %d = %d\n",a,op,b,a*b);
 else if(op == '/')
    printf("%d %c %d = %d\n",a,op,b,a/b);
 else if(op == '%')
    printf("%d %c %d = %d\n",a,op,b,a%b);
 return 0;
}
