#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int desferURIm2(const char *uri, char *esq, char *nom_host, int *port, char *nom_fitx);

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
 
 /* Es desfà l'URI, mètode 2 */ 
 n = desferURIm2(uri,esquema,nom_host,&port,nom_fitx);
 
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
int desferURIm2(const char *uri, char *esq, char *nom_host, int *port, char *nom_fitx)
{
 char *urilocal, *hostportlocal;
 char *token;
 int nassignats;
 char port_str[100];
 
 urilocal = (char *) malloc(sizeof(char) * (strlen(uri) + 1));
 strcpy(urilocal,uri);
 
 strcpy(esq,"");
 strcpy(nom_host,"");
 *port = 0;
 strcpy(nom_fitx,"");
 nassignats=0;
 
 /* La part "esq": fins trobar ":" */
 token = strtok(urilocal,":");
 if(token==NULL)
 {
  free(urilocal);
  return nassignats;
 }
 strcpy(esq,token);
 nassignats++;
 
 /* La part "nom_host:port": fins trobar "/" */
 /* Es guarda temporalment a hostportlocal   */
 token = strtok(NULL,"/");
 if(token==NULL)
 {
  free(urilocal);
  return nassignats;
 }
 hostportlocal = (char *) malloc(sizeof(char) * (strlen(token) + 1));
 strcpy(hostportlocal,token);
 nassignats++;
  
 /* La part "nom_fitxer": fins al final, p.e., trobar "" */
 /* Cal afegir-li al davant "/"                          */
 token = strtok(NULL,"");
 if(token==NULL)
 {
  free(urilocal);
  free(hostportlocal);
  return nassignats;
 }
 nom_fitx[0]='/';
 strcpy(nom_fitx+1,token);
 nassignats++;
 
 /* La part de "nom_host:port" o potser "nom_host" sense "port"  */
 /* Cal processar hostportlocal per trobar ":"                   */
 /*  Si no hi és, vol dir que és sense "port", i es fa port = 0  */
 /*  Si hi és, vol dir que és amb "port" i es separa             */
 token = strtok(hostportlocal,":");
 if(token==NULL)
 {
  strcpy(nom_host,hostportlocal);
  *port=0;
  free(urilocal);
  free(hostportlocal);
  return nassignats;
 }
 strcpy(nom_host,token);
 
 token = strtok(NULL,"");
 if(token==NULL)
 {
  free(urilocal);
  free(hostportlocal);
  return nassignats;
 }
 strcpy(port_str,token);
 *port = atoi(port_str);
 nassignats++;
 
 /*
 printf("--nassignats %d\n",nassignats);
 printf("--esq %s\n", esq);
 printf("--nom_host %s\n", nom_host);
 printf("--port_str %s\n", port_str);
 printf("--nom_fitx %s\n", nom_fitx);
 */
  
 free(urilocal);
 free(hostportlocal);

 return nassignats;
}
