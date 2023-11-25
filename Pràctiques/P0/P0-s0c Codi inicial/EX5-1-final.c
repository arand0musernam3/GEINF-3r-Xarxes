#include <stdio.h>
#include <stdlib.h>

int main()
{
	char linia[150];
	int i, nllegits;
 
	for(i=0;i<=1;i++)
	{
		/* Lectura via teclat fent servir un read(), que ho escriu a linia. */
		/* linia és un apuntador a char: apunta a la primera posició d'un   */
		/* espai de memòria de 150 chars (bytes).                           */
		/* Quan es prem el canvi de línia (char '\n'), tot el que s'ha      */
		/* entrat via teclat està disponible per ser llegit; read() llegeix */
		/* "tot" el que s'ha entrat (de fet fins a 150 bytes), incloent el  */
		/* '\n' (és el darrer char), i ho escriu a linia; retorna el nombre */
		/* de bytes (chars) llegits.                                        */
		printf("\nEscriu la línia #%d:\n", i);
		nllegits = read(0,linia,150);
  
		/* Es mostra per pantalla el que s'ha entrat fent servir un write().*/
		printf("\nLa línia #%d conté (via write):\n", i);
		write(1,linia,nllegits);
 
		/* Es mostra per pantalla el que s'ha entrat fent servir un         */
		/* "printf d'string": printf("%s...", linia); com que per fer-ho    */ 
		/* linia ha de ser un string, es posa el char nul '\0' al final     */
		/* per convertir-lo en string; a més s'elimina el char '\n' (es     */
		/* substitueix un per l'altre)                                      */
		printf("\nLa línia #%d conté (via printf d'string):\n", i);
		printf("%s\n", linia); 	
	}	

exit(0);
}
