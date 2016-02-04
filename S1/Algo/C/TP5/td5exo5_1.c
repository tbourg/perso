#include<stdio.h>

void echangeint(int *p, int * q)
/*
:entrée p : adresse d'un int
:entrée q : adresse d'un int
:postcondition : échange les valeurs de type int dont p et q sont les adresses 
*/
{
 int aux;        
 aux=*p;
 *p=*q;
 *q=aux; 
}

void echangeshort(short *p, short * q)
/*
:entrée p : adresse d'un short
:entrée q : adresse d'un short
:postcondition : échange les valeurs de type short dont p et q sont les adresses 
*/
{
 short aux;        
 aux=*p;
 *p=*q;
 *q=aux; 
}

void echangechar(char *p, char * q)
/*
:entrée p : adresse d'un char
:entrée q : adresse d'un char
:postcondition : échange les valeurs de type char dont p et q sont les adresses 
*/
{
 char aux;        
 aux=*p;
 *p=*q;
 *q=aux; 
}

void echangefloat(float *p, float * q)
/*
:entrée p : adresse d'un float
:entrée q : adresse d'un float
:postcondition : échange les valeurs de type float dont p et q sont les adresses 
*/
{
 float aux;        
 aux=*p;
 *p=*q;
 *q=aux; 
}

void echangedouble(double *p, double * q)
/*
:entrée p : adresse d'un double
:entrée q : adresse d'un double
:postcondition : échange les valeurs de type double dont p et q sont les adresses 
*/
{
 double aux;        
 aux=*p;
 *p=*q;
 *q=aux; 
}


void echangeTout(void *p, void *q, int type)
/*
:entrée p : adresse
:entrée q : adresse
:entrée type : int
:pré-condition : si type vaut 1 p et q sont des adresses d'int, 2 adresses de short, 3 adresses de char, 4 adresses de float, 5 adresses de double
:postcondition : échange les valeurs se trouvant aux adresses p et q 
*/
 {
     switch(type) {
                   case 1 : echangeint(p,q);
                            break;       
                   case 2 : echangeshort(p,q);
                   	    break; 
                   case 3 : echangechar(p,q);
                            break;                        
                   case 4 : echangefloat(p,q);
                            break;                      
                   case 5 : echangedouble(p,q); 
     		  }
 }


int main()
{
 int type;
 printf("type des variables :\n 1. int \n 2. short \n 3. char \n 4. float \n 5. double \n");
 scanf("%d",&type);
 switch(type) {
                  case 1 :{  int a,b;
                             printf("\n a= ");
                             scanf("%d",&a);
                             printf("\n b= ");
                             scanf("%d",&b);
                             echangeTout(&a,&b,1);
                             printf("apres : a=%d et b=%d\n",a,b); 
                             break; 
                          }
                  case 2 :{  short a,b;
                             printf("\n a= ");
                             scanf("%hd",&a);
                             printf("\n b= ");
                             scanf("%hd",&b);
                             echangeTout(&a,&b,2);
                             printf("apres : a=%hd et b=%hd\n",a,b); 
                             break; 
                          }
                  case 3 :{  char a,b;
                             printf("\n a= ");
                             scanf(" %c",&a);
                             printf("\n b= ");
                             scanf(" %c",&b);
                             echangeTout(&a,&b,3);
                             printf("apres : a=%c et b=%c\n",a,b); 
                             break; 
                          }
                  case 4 :{  float a,b;
                             printf("\n a= ");
                             scanf("%f",&a);
                             printf("\n b= ");
                             scanf("%f",&b);
                             echangeTout(&a,&b,4);
                             printf("apres : a=%f et b=%f\n",a,b); 
                             break; 
                          }
                  case 5 :{  double a,b;
                             printf("\n a= ");
                             scanf("%lf",&a);
                             printf("\n b= ");
                             scanf("%lf",&b);
                             echangeTout(&a,&b,5);
                             printf("apres : a=%lf et b=%lf\n",a,b); 
                             break; 
                          }
                  }
 return 0;   
}
