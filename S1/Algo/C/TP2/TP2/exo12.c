#include<stdio.h>
#include<math.h>

int main()
{
    int nb=0,i=0;
    char car;
    printf("Saisir un texte : ");
    scanf("%c",&car);
    while(car!='\n')
      {
       i++;
       if(car=='L') nb++;
       if(car!=' ') printf("%c",car);
       scanf("%c",&car);
       }
    printf("\n");
    printf("nb de L : %d\n",nb);
    printf("i = %d\n",i);
    return 0;
}
