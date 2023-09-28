#include <stdio.h>
#include <stdlib.h>

int main()
{
 char ElNom[100], ElDNI[10];
 int MevaEdatAnys, MevaEdatMesos;
 int nombrebytesllegits;
 
 /* Lectura via teclat del nom i cognoms (p.e. Pere Vila) amb read(). */
 /* read() requereix un apuntador i Nom ho és.                        */
 /* Un cop escrit el nom al teclat, despres de premer retorn, read()  */
 /* escriu els bytes (chars) entrats a ElNom i en retorna el nombre.  */
 /* ElNom te el char canvi de linia '\n' al final i no el necessitem. */
 /* ElNom es un vector de char pero per ser string necessita el char  */
 /* de final '\0', i per aixo li posem: sobreescrivim  '\n' pel '\0'. */
 printf("Nom i cognoms?\n");
 nombrebytesllegits = read(0,ElNom,100);
 if(nombrebytesllegits==-1)
 {
  printf("Error en read()\n");
  exit(-1);
 }
 ElNom[nombrebytesllegits-1]='\0';

 /* Lectura via teclat del DNI (p.e. 40563589D) amb scanf().          */
 /* scanf() requereix un apuntador (a char) i ElDNI ho es.            */
 /* scanf() ja hi posa el '\0' final.                                 */
 printf("DNI?\n");
 scanf("%s", ElDNI); 

 /* Lectura via teclat de l'edat amb scanf().                         */
 /* scanf() requereix un apuntador (a int) i com que MevaEdatAnys no  */
 /* ho es, cal fer servir l'operador &                                */
 /* scanf() ja hi posa el '\0' final.                                 */
 printf("Edat [anys]?\n");
 scanf("%d", &MevaEdatAnys); 
 
 /* Es comprova que l'edat en anys es correcta i es calcula en mesos  */
 if(MevaEdatAnys<=0)
 {
  printf("La teva edat en anys %d no es correcta\n", MevaEdatAnys);
  exit(0);
 }
 MevaEdatMesos=MevaEdatAnys*12;
 
 /* Es mostra per pantalla el resultat                                */
 printf("%s, amb DNI num. %s,\n", ElNom, ElDNI);
 printf("tens %d anys (%d mesos).\n", MevaEdatAnys, MevaEdatMesos);
 
 exit(0);
}
