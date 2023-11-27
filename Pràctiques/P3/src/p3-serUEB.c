/**************************************************************************/
/*                                                                        */
/* P2 - L'aplicació UEB amb sockets TCP/IP - Part I                       */
/* Fitxer serUEB.c que implementa la interfície aplicació-administrador   */
/* d'un servidor de l'aplicació UEB, sobre la capa d'aplicació de         */
/* (la part servidora de) UEB (fent crides a la interfície de la part     */
/* servidora de la capa UEB).                                             */

/*                                                                        */
/* Autors:                                                                */
/* Data:                                                                  */
/*                                                                        */
/**************************************************************************/

#include "p3-aUEBs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "p3-aDNSc.h"

int AfegeixSck(int Sck, int *LlistaSck, int LongLlistaSck);
int TreuSck(int Sck, int *LlistaSck, int LongLlistaSck);
int desferURI(const char *uri, char *esq, char *nom_host, int *port, char *nom_fitx);

int read_config(char* path, int* port, int* maxCon) {

    FILE *fp;
    char linia[50];

    fp = fopen("p2-serUEB.cfg","r");
    if (fgets(linia,sizeof(linia),fp) == NULL) {
        printf("No s'ha pogut trobar el port a obrir");
        return -1;
    }
    *port = atoi(linia+9);

    if (fgets(linia,sizeof(linia),fp) == NULL) {
        printf("No s'ha pogut trobar l'arrel.");
        return -1;
    }
    strcpy(path, linia+7);

    if (fgets(linia,sizeof(linia),fp) == NULL) {
        printf("No s'ha trobat el limit de connexions simultànies.");
        return -1;
    }
    *maxCon = atoi(linia+11);

}

int fd;

void escriure(const char* str) {
    write(fd, str, strlen(str));
    printf("%s", str);
}

int main(int argc,char *argv[])
{
    //sempre hi haurà dos bucles infinits, el primer de serveix a dins, fins que es retorni -3 que llavors es tancarà la connexió. Fer un sleep al final tal i com diu l'enunciat.
    // L'altre bucle més gros és el d'acceptar connexions, també és infinit fins ctrl C.
    
    // Declaració de variables, p.e., int n;    
    int socket_s;
    int socket_con;
    int port_s;
    char text_res[200];
    char buffer[1000];

    int *llistaSck;
    int longLlistaSck;

    char locIP[16];
    int locPort;
    char remIP[16];
    int remPort;

    char path[300];
    int maxCon;

    // Fitxer log
    fd = open("serUEB.log", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    
    read_config(path, &port_s, &maxCon);

    sprintf(buffer, "Port d'escolta: %d\n\0", port_s);
    escriure(buffer);
    sprintf(buffer, "Arrel de src: %s\n\0", path);
    escriure(buffer);
    sprintf(buffer, "Limit de connexions simultanies: %d\n\0", maxCon);
    escriure(buffer);

    longLlistaSck = 1 + maxCon; // Socket d'escolta + maxim de connexions
    llistaSck = (int *) malloc(longLlistaSck * sizeof(int));
    for (int i = 0; i < longLlistaSck; i++)
        llistaSck[i] = -1;

    if (UEBs_IniciaServ(&socket_s, port_s, text_res) == -1) {
        escriure(text_res);
        return -1;
    }

    AfegeixSck(socket_s, llistaSck, longLlistaSck);
    escriure(text_res);

    while (UEBs_HaArribatAlgunaCosaPerLlegir(llistaSck, longLlistaSck, text_res) != -1) {

    }

    /*
    while (1) {
        socket_con = UEBs_AcceptaConnexio(socket_s, locIP, &locPort, remIP, &remPort, text_res);
        if (socket_con < 0) {
            escriure(text_res);
            continue;
        }

        escriure(text_res);

        while (1) {
            char tipus[4], nom_fitxer[10000];
            int res = UEBs_ServeixPeticio(socket_con, tipus, nom_fitxer, text_res, path);

            if (res == 0 || res == 1) {
                sprintf(buffer, "Petició rebuda: %s %s de %s:%d a %s:%d pel socket %d\n\0", tipus, nom_fitxer, remIP, remPort, locIP, locPort, socket_con);
                escriure(buffer);
            }

            escriure(text_res);

            if (res == -3) {
                UEBs_TancaConnexio(socket_con, text_res);
                escriure(text_res);
                break;
            }
        }
    }
    */
    
    return 0;
}

/* Donada la llista d'identificadors de sockets “LlistaSck” (de longitud  */
/* “LongLlistaSck” sockets), hi busca una posició "lliure" (una amb un    */
/* contingut igual a -1) i hi escriu l'identificador de socket "Sck".     */
/*                                                                        */
/* "LlistaSck" és un vector d'int d'una longitud d'almenys LongLlistaSck. */
/*                                                                        */
/* Retorna:                                                               */
/*  0 si tot va bé;                                                       */
/* -1 si hi ha error.                                                     */
int AfegeixSck(int Sck, int *LlistaSck, int LongLlistaSck)
{
    int i = 0;

    while (LlistaSck[i] != -1 && i < LongLlistaSck)
        i++;

    if (i == LongLlistaSck) {
        char buffer[1000];
        sprintf(buffer, "No s'ha pogut obrir el socket %d, no queda lloc a la llista\n\0", Sck);
        escriure(buffer);
        return -1;
    }

    LlistaSck[i] = Sck;

    return 0;
}

/* Donada la llista d'identificadors de sockets “LlistaSck” (de longitud  */
/* “LongLlistaSck” sockets), hi busca la posició on hi ha l'identificador */
/* de socket "Sck" i la marca com "lliure" (hi escriu un contingut igual  */
/* a -1).                                                                 */ 
/*                                                                        */
/* "LlistaSck" és un vector d'int d'una longitud d'almenys LongLlistaSck. */
/*                                                                        */
/* Retorna:                                                               */
/*  0 si tot va bé;                                                       */
/* -1 si hi ha error.                                                     */
int TreuSck(int Sck, int *LlistaSck, int LongLlistaSck)
{
    int i = 0;

    while (LlistaSck[i] != Sck && LongLlistaSck)
        i++;

    if (i == LongLlistaSck) {
        char buffer[1000];
        sprintf(buffer, "No s'ha pogut tancar el socket %d, no existeix a la llista\n\0", Sck);
        escriure(buffer);
        return -1;
    }

    LlistaSck[i] = -1;

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
int desferURI(const char *uri, char *esq, char *nom_host, int *port, char *nom_fitx)
{
	
}
