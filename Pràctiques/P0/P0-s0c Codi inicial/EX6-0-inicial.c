#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Declaracions de funcions per així poder-les fer servir més avall  */
int PrimeraManera(int *enter, int *vectorenter, char *paraula);
int SegonaManera(int *enter, int *vectorenter, char *paraula);

int main()
{
 int enter1=1, enter2=1;
 int vectorenter1[]={1,2,3}, vectorenter2[]={1,2,3};
 char paraula1[]="inici", paraula2[]="inici";
 
 printf("\n");
 printf("Dins el main(), abans de cridar PrimeraManera():\n");
 printf("   enter = %d\n", enter1);
 printf("   vectoenter = %d %d %d\n", vectorenter1[0], vectorenter1[1], vectorenter1[2]); 
 printf("   paraula = %s\n", paraula1);
 if(PrimeraManera(&enter1, vectorenter1, paraula1) != 1)
 {
  printf("Error a PrimeraManera()\n");
  exit(-1);
 }
 printf("Dins el main(), despres de cridar PrimeraManera():\n");
 printf("   enter = %d\n", enter1);
 printf("   vectoenter = %d %d %d\n", vectorenter1[0], vectorenter1[1], vectorenter1[2]); 
 printf("   paraula = %s\n", paraula1);
 
 printf("\n");
 printf("Dins el main(), abans de cridar SegonaManera():\n");
 printf("   enter = %d\n", enter2);
 printf("   vectoenter = %d %d %d\n", vectorenter2[0], vectorenter2[1], vectorenter2[2]); 
 printf("   paraula = %s\n", paraula2);
 if(SegonaManera(&enter2, vectorenter2, paraula2) != 1)
 {
  printf("Error a SegonaManera()\n");
  exit(-1);
 }
 printf("Dins el main(), despres de cridar SegonaManera():\n");
 printf("   enter = %d\n", enter2);
 printf("   vectoenter = %d %d %d\n", vectorenter2[0], vectorenter2[1], vectorenter2[2]); 
 printf("   paraula = %s\n", paraula2);
 printf("\n");
 
 exit(0);
}

/* Definicions de les funcions                                       */

int PrimeraManera(int *enter, int *vectorenter, char *paraula)
{
 int nombre=2;
 int vectornombres[]={4,5,6};
 char acabament[]="acabament";
 
 printf("  Dins PrimeraManera(), abans de la copia:\n");
 printf("     enter = %d\n", *enter);
 printf("     vectoenter = %d %d %d\n", vectorenter[0], vectorenter[1], vectorenter[2]); 
 printf("     paraula = %s\n", paraula);

 /* Nombre enter (int) */
 enter = &nombre;

 /* Vector de nombres enters (int) */  
 vectorenter = vectornombres;

 /* String o vector de caràcters (char) ASCII imprimibles acabat en '\0')*/  
 paraula = acabament;

 printf("  Dins PrimeraManera(), despres de la copia:\n"); 
 printf("     enter = %d\n", *enter); 
 printf("     vectoenter = %d %d %d\n", vectorenter[0], vectorenter[1], vectorenter[2]);
 printf("     paraula = %s\n", paraula);
   
 return 1;	
}

int SegonaManera(int *enter, int *vectorenter, char *paraula)
{
 int nombre=2;
 int vectornombres[]={4,5,6};
 char acabament[]="acabament";
 int i;

 printf("  Dins SegonaManera(), abans de la copia:\n"); 
 printf("     enter = %d\n", *enter); 
 printf("     vectoenter = %d %d %d\n", vectorenter[0], vectorenter[1], vectorenter[2]);
 printf("     paraula = %s\n", paraula);
 
 /* Nombre enter (int) */
 *enter = nombre;
 
 /* Vector de nombres enters (int) */
 /* El següent codi és equivalent a memcpy(vectorenter,vectornombres,3) */
 for(i=0;i<=2;i++)
 {
  *(vectorenter+i) = *(vectornombres+i); /* o vectorenter[i]= vectornombres[i] */
 }

 /* String o vector de caràcters (char) ASCII imprimibles acabat en '\0')*/ 
 /* El següent codi és equivalent a strcpy(paraula,acabament) */
 i=0;
 while( *(acabament+i) != '\0' )
 {
  *(paraula+i) = *(acabament+i); /* o paraula[i]= acabament[i] */
  i++;
 }
 *(paraula+i) = '\0'; /* posa el '\0' al final */
 
 printf("  Dins SegonaManera(), despres de la copia:\n"); 
 printf("     enter = %d\n", *enter); 
 printf("     vectoenter = %d %d %d\n", vectorenter[0], vectorenter[1], vectorenter[2]);
 printf("     paraula = %s\n", paraula);
   
 return 1;
}
