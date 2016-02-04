#include<stdio.h>

int main()
{
    char car;
    printf("Saisir un texte : ");
    scanf("%c",&car);
    while(car!='\n')
      {
       if('A'<=car && car<='Z') printf("%c",car-'A'+'a');
       else if('a'<=car && car<='z') printf("%c",car-'a'+'A');
       else printf("%c",car);
       scanf("%c",&car);
       }
    printf("\n");
    return 0;
}
