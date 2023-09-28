#include <stdio.h>
#include <stdlib.h>

# include "EX7-3-funcions.h"

int main()
{
 char ElNom[100], ElDNI[10];
 int MevaEdatAnys, MevaEdatMesos;

 if(ObteInfo(ElNom,100,ElDNI,&MevaEdatAnys) == -1)
 {
  printf("Error a ObteInfo()\n");
  exit(-1);
 }

 if(TrobaEdatEnMesos(MevaEdatAnys,&MevaEdatMesos) != 0)
 {
  printf("Error a TrobaEdatEnMesos()\n");
  exit(-1);
 }

 if(MostraResultat(ElNom,ElDNI,MevaEdatAnys,MevaEdatMesos) != 0)
 {
  printf("Error a MostraResultat()\n");
  exit(-1);
 }

 exit(0);
}


