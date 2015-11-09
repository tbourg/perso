#include<stdio.h>

void main()
{
    float note1,note2,note3,max1,max2;
    printf("entrezzz trois notes\n");
    scanf("%f %f %f",&note1,&note2,&note3);
    if (note1>=note3 && note2>=note3)
    {
        max1=note1;
        max2=note2;
    }
    else if (note2>=note1 && note3>=note2)
    {
        max1=note2;
        max2=note3;
    }
    else
    {
        max1=note1;
        max2=note3;
    }
    float moyenne=(max1+max2)/2;
    printf("moyenne=%f",moyenne);
}
