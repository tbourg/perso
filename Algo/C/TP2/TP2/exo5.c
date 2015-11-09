#include<stdio.h>

int main()
{
    float note1=-1, note2=-1, note3=-1;
    printf("\nDonner trois notes : ");
    scanf("%f %f %f",&note1,&note2,&note3);
    if(note1>=0 && note1<=20 && note2>=0 && note2<=20 && note3>=0 && note3<=20)
    {
    if(note1<=note2 && note1<=note3) printf("moyenne = %f\n",(note2+note3)/2);
    else if(note2<=note1 && note2<=note3) printf("moyenne = %f\n",(note1+note3)/2);
    else printf("moyenne = %f\n",(note1+note2)/2);
    }
    else printf("erreur de saisie\n");
    return 0;
}
