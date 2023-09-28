#include <stdio.h>
#include <stdlib.h>

/* Declaracions de funcions per així poder-les fer servir més avall  */
int ObteInfo(char *nom, int longnom, char *dni, int *edatanys);
int TrobaEdatEnMesos(int anys, int *mesos);
int MostraResultat(const char *nom, const char *dni, int edatanys, int edatmesos);

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

/* Definicions de les funcions                                       */

int ObteInfo(char *nom, int longnom, char *dni, int *edatanys)
{
	//Lectura del NOM
	printf("Nom i cognoms?\n");
	int nombrebytesllegits = read(0,nom,longnom);
	if(nombrebytesllegits==-1)
	{
		printf("Error en read()\n");
		return(-1);
	}
	nom[nombrebytesllegits-1]='\0'; //substitució del \n per \0
	
	//Lectura del DNI
	printf("DNI?\n");
	scanf("%s", dni);
	
	//Lectura de l'edat
	printf("Edat [anys]?\n");
	scanf("%d", edatanys);
	
	if (edatanys <=0) {
		printf("Error, l'edat entrada és incorrecta.\n");
		return -1;
	}
	
	return 0;
}

int TrobaEdatEnMesos(int edatanys, int *edatmesos)
{
	*edatmesos=edatanys*12;
	return 0;
}


int MostraResultat(const char *nom, const char *dni, int edatanys, int edatmesos)
{
	printf("%s, amb DNI num. %s,\n", nom, dni);
	printf("tens %d anys (%d mesos).\n", edatanys, edatmesos);
	return 0;
}

