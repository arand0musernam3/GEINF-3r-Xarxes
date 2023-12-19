/**************************************************************************/
/*                                                                        */
/* P2 - L'aplicació UEB amb sockets TCP/IP - Part I                       */
/* Fitxer cliUEB.c que implementa la interfície aplicació-usuari          */
/* d'un client de l'aplicació UEB, sobre la capa d'aplicació de           */
/* (la part client de) UEB (fent crides a la interfície de la part        */
/* client de la capa UEB).                                                */
/*                                                                        */
/* Autors:                                                                */
/* Data:                                                                  */
/*                                                                        */
/**************************************************************************/

/* Inclusió de llibreries, p.e. #include <stdio.h> o #include "meu.h"     */

#include "p3-aUEBc.h"
#include "p3-aDNSc.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
/* Definició de constants, p.e.,                                          */

/* #define XYZ       1500                                                 */

#define LLARGADA_BUFFER_IP 16
#define DEFAULT_PORT 7999

const char STOP_SEQUENCE[] = "STOP\0";

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */

/* int FuncioInterna(arg1, arg2...);                                      */

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
    int nassignats;
    char port_str[100];

    strcpy(esq, "");
    strcpy(nom_host, "");
    *port = 0;
    strcpy(nom_fitx, "");

    nassignats = sscanf(uri, "%[^:]://%[^:]:%[^/]%s", esq, nom_host, port_str, nom_fitx);

    /*
    printf("nassignats %d\n",nassignats);
    printf("esq %s\n", esq);
    printf("nom_host %s\n", nom_host);
    printf("port_str %s\n", port_str);
    printf("nom_fitx %s\n", nom_fitx);
    */

    /* URIs amb #port, p.e., esq://host:port/fitx, 4 valors assignats */
    if (nassignats == 4)
    {
        *port = atoi(port_str);
        return nassignats;
    }

    /* URIs sense #port, p.e., esq://host/fitx, 2 valors assignats; */
    /* llavors es fa port = 0 i una nova assignació                 */
    if (nassignats == 2)
    {
        *port = 0;
        nassignats = sscanf(uri, "%[^:]://%[^/]%s", esq, nom_host, nom_fitx);
        return nassignats;
    }

    return nassignats;
}

int main(int argc, char *argv[])
{
    // Declaració de variables, p.e., int n;
    int socket_c;
    char text_res[200];

    /* Expressions, estructures de control, crides a funcions, etc.          */
    char ip_ser[20];
    char uri[512];
    char esq[64];
    int port_ser;
    char ip_cli[20];
    char nom_dns[512];
    int port_cli;
    char nom_fitxer[10000];

    char fitxer[9999];
    int long_fitxer;
    char opt[5];
    int obtingutCorrectament;

    do
    {
        printf("Entra l'URI [STOP per aturar el client]: ");
        scanf("%s", uri);

        if (strcmp(uri, STOP_SEQUENCE) == 0) {
            return 0;
        }

        if (desferURI(uri, esq, nom_dns, &port_ser, nom_fitxer) == 3)
        {
            port_ser = DEFAULT_PORT;
        }

        if (strcmp(esq, "pueb\0") != 0) {
            printf("Error: l'esquema entrat no correspon a pueb.\n");
            continue;
        }

        if (DNSc_ResolDNSaIP(nom_dns, ip_ser, text_res) == -1)
        {
            printf("Error: el nom DNS entrat no correspon a cap servidor.\n", text_res);
            continue;
        }

        socket_c = UEBc_DemanaConnexio(ip_ser, port_ser, ip_cli, &port_cli, text_res);

        if (socket_c == -1)
        {
            continue;
        }

        struct timeval start, end;
        gettimeofday(&start, 0);
        obtingutCorrectament = UEBc_ObteFitxer(socket_c, nom_fitxer, fitxer, &long_fitxer, text_res);
        if (obtingutCorrectament != 0)
        {
            printf("%s", text_res);
        }
        else
        {
            gettimeofday(&end, 0);
            printf("Servida petició d'obtenir %s de %s:%d a %s:%d\n", nom_fitxer, ip_cli, port_cli, ip_ser,
                   port_ser);

            printf("%s\n", fitxer);

            printf("Temps de resposta: %f s\n", (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1e-6);

            int fd = open(nom_fitxer + 1, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); // el +1 és per esquivar la / del principi NO TREURE
            write(fd, fitxer, long_fitxer);
        }

        if (UEBc_TancaConnexio(socket_c, text_res) == -1)
        {
            printf("%s", text_res);
        }

        printf("Vols realitzar una altra petició? [y/n]");
        scanf("%s", opt);
        if (opt[0] == 'n' || opt[0] == 'N')
            break;

    } while (1);

    sleep(5);

    return 0;
}
