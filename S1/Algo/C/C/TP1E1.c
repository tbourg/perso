#include<stdio.h>

int main()
{
    int taille_int = sizeof(int);
    int taille_short = sizeof(short);
    int taille_long = sizeof(long);
    int taille_char = sizeof(char);
    int taille_float = sizeof(float);
    int taille_double = sizeof(double);
    printf("un int fait %d un long fait %d un char fait %d un float fait %d et un double fait %d octet",taille_int,taille_short,taille_long,taille_char,taille_float,taille_double);
}
