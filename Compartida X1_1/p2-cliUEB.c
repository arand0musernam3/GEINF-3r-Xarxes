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

#include "p2-aUEBc.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
/* Definició de constants, p.e.,                                          */

/* #define XYZ       1500                                                 */

#define LLARGADA_BUFFER_IP 16

const char STOP_SEQUENCE[] = "STOP\0";

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */

/* int FuncioInterna(arg1, arg2...);                                      */

int main(int argc, char *argv[]) {
    // Declaració de variables, p.e., int n;
    int socket_c;
    char text_res[200];

    /* Expressions, estructures de control, crides a funcions, etc.          */
    char peticio[8];
    char ip_ser[LLARGADA_BUFFER_IP];
    int port_ser;
    char *ip_cli;
    int port_cli;
    char nom_fitxer[10000];

    do {
        printf("IP servidor: ");
        scanf("%s", ip_ser);
        printf("Port servidor: ");
        scanf("%d", &port_ser);
        port_ser = port_ser == 0 ? 3000 : port_ser;
    } while ((socket_c = UEBc_DemanaConnexio(ip_ser, port_ser, ip_cli, &port_cli, text_res)) == -1);

    char opt[5];
    do {
        printf("Petició: ");
        scanf("%s", peticio);
        printf("Nom del fitxer: ");
        scanf("%s", nom_fitxer);

        if (!strcmp(peticio, "obtenir")) {
            char fitxer[9999];
            int long_fitxer;

            if (UEBc_ObteFitxer(socket_c, nom_fitxer, fitxer, &long_fitxer, text_res) != 0) {
                printf(text_res);
            } else {
                printf("Servida petició: %s %s de %s:%d a %s:%d\n", peticio, nom_fitxer, ip_cli, port_cli, ip_ser,
                       port_ser);

                printf("%s", fitxer);

                int fd = open(nom_fitxer, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
                write(fd, fitxer, long_fitxer);
            }

        }
        printf("Vols realitzar una altra petició? [y/n]");
        scanf("%s", opt);
    } while (opt[0] == 'y' || opt[0] == 'Y');

    sleep(5);

    if (UEBc_TancaConnexio(socket_c, text_res) == -1) {
        printf("%s", text_res);
    }

    return 0;
}
