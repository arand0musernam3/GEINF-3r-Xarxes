#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int desferURIm1(const char *uri, char *esq, char *nom_host, int *port, char *nom_fitx);

int main(int argc,char *argv[])
{
 char uri[100];
 char esquema[100], nom_host[100], nom_fitx[100];
 int port;
 int n;
 
 /* Es demana un URI */
 printf("\n");
 printf("URI: ");
 scanf("%s",uri);
 
 /* Es desfà l'URI, mètode 1 */ 
 n = desferURIm1(uri,esquema,nom_host,&port,nom_fitx);
 
 printf("\n");
 printf("Nombre de valors assignats: %d\n", n);
 printf("Esquema: %s\n", esquema);
 printf("Nom DNS (o @IP): %s\n", nom_host);
 printf("#port: %d\n", port);
 printf("Nom fitxer: %s\n", nom_fitx);
 printf("\n");
  
 return 0;
}

/* Desfà l'URI "uri" en les seves parts: l'esquema (protocol) "esq", el   */
/* nom DNS (o l'@IP) "nom_host", el número de port "port" i el nom del    */
/* fitxer "nom_fitxer".                                                   */
/*                                                                        */
/* L'URI ha de tenir la forma "esq://nom_host:port/nom_fitxer" o bé       */
/* sense el número de port "esq://nom_host/nom_fitxer", i llavors port    */
/* s'emplena amb el valor 0 (la resta de casos no es contemplen).         */
/*                                                                        */
/* "uri", "esq", "nom_host" i "nom_fitxer" són "strings" de C (vector de  */
/* chars imprimibles acabat en '\0') d'una longitud suficient.            */
/*                                                                        */
/* Retorna:                                                               */
/*  el nombre de parts de l'URI que s'han assignat (4 si l'URI tenia      */
/*  número de port o 3 si no en tenia).                                   */
int desferURIm1(const char *uri, char *esq, char *nom_host, int *port, char *nom_fitx)
{
 int nassignats;
 char port_str[100];
 
 strcpy(esq,"");
 strcpy(nom_host,"");
 *port = 0;
 strcpy(nom_fitx,"");

 nassignats = sscanf(uri,"%[^:]://%[^:]:%[^/]%s",esq,nom_host,port_str,nom_fitx);
 
 /*
 printf("nassignats %d\n",nassignats);
 printf("esq %s\n", esq);
 printf("nom_host %s\n", nom_host);
 printf("port_str %s\n", port_str);
 printf("nom_fitx %s\n", nom_fitx);
 */
 
 /* URIs amb #port, p.e., esq://host:port/fitx, 4 valors assignats */
 if(nassignats==4)
 {
  *port = atoi(port_str);
  return nassignats;
 }  
  
 /* URIs sense #port, p.e., esq://host/fitx, 2 valors assignats; */
 /* llavors es fa port = 0 i una nova assignació                 */
 if(nassignats==2)
 {
  *port = 0;
   nassignats = sscanf(uri,"%[^:]://%[^/]%s",esq,nom_host,nom_fitx);
   return nassignats;
 }

 return nassignats;
}
