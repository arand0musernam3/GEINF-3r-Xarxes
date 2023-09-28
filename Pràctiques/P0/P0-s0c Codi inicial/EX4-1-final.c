#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char linia[150];
 
	/* Lectura via teclat amb scanf().                                   */
	/* scanf() requereix un apuntador (a char) i linia ho es.            */
	/* scanf() ja hi posa el '\0' final i el converteix en string        */

	printf("Escriu una linia sense espais en blanc:\n");

	scanf("%s", linia); 
	/* l'scanf() fet així llegeix fins trobar un espai o canvi de linia  */
 
	/* Es mostra per pantalla el que s'ha entrat                         */
	printf("\nHas entrat:\n");
 	printf("%s\n", linia);

	/* Es printa la longitud de l'string */
	printf("\nLongitud de l'string:\n");
	printf("%ld caràcters\n",strlen(linia));
 
	/* Longitud del vector de chars */
	printf("Longitud del vector:\n");
	printf("%ld caràcters\n",sizeof(linia));
 
	/* Codi ASCII en forma d'enter del vector */
	printf("Input en codi ASCII:\n");
	
	int i = 0;
	
	while (linia[i] != '\0') {
		printf("%d ", linia[i]);
		i++;
	}
	
	printf("%d\n", linia[i]); //imprimim el \0 que queda després del while.
	
	exit(0);
}
