/**************************************************************************/
/*                                                                        */
/* P1 - L'aplicació ECO amb sockets TCP/IP                                */
/* Fitxer cliECO.c que implementa el client ECO sobre la capa de          */
/* transport TCP (fent crides a la "nova" interfície de la capa de        */
/* transport o "nova" interfície de sockets).                             */
/*                                                                        */
/* Autors: Jordi Badia i Aniol Juanola                                    */
/* Data:                                                                  */
/*                                                                        */
/**************************************************************************/

/* Inclusió de llibreries, p.e. #include <stdio.h> o #include "meu.h"     */

#include "p1-tTCP.h"

#include <stdio.h>

/* Definició de constants, p.e.,                                          */

/* #define XYZ       1500                                                 */

#define LLARGADA_BUFFER_IP 16

const char STOP_SEQUENCE[] = "STOP\0";

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */

/* int FuncioInterna(arg1, arg2...);                                      */

int main(int argc, char *argv[])
{
    //variables a utilitzar
    char ip[LLARGADA_BUFFER_IP];
    int port;
    int socket_c;
    char aux[30];
    char msg[512];
    int readBytesMsg;

    char opt[5];
    do {
        if ((socket_c = TCP_CreaSockClient("0.0.0.0", 0)) == -1)
        {
            printf("Error a TCP_CreaSockClient");
            return -1;
        }

        obtenirIpSock(socket_c, aux, 30);

        printf("IP servidor: %s\n", aux);

        // LLEGIR INPUT USUARI PER A CONNECTAR-SE:

        printf("Introdueix la direcció IP a la que et vols connectar:");
        scanf("%s", ip);
        printf("Introdueix el port al que et vols connectar:");
        scanf("%d", &port);

        if (TCP_DemanaConnexio(socket_c, ip, port) < 0) {
            return -1;
        }

        obtenirIpPeer(socket_c, aux, 30);
        printf("IP servidor: %s\n", aux);

        printf("Entra \"STOP\" per a parar la transmissio de missatges\n");


        for(;;) {
            readBytesMsg = read(0, msg, 512);
            msg[readBytesMsg-1] = '\0';
            if (!strcmp(msg, STOP_SEQUENCE)) {
                break;
            }
            printf("Enviats %d bytes\n", TCP_Envia(socket_c, msg, readBytesMsg));
            readBytesMsg = TCP_Rep(socket_c, msg, 512);
            printf("ECO: %s\n\n", msg);
        }

        TCP_TancaSock(socket_c);

        //ara toca demanar si es vol establir una altra connexió.
        printf("Vols establir una altra connexió? [y/n]");
        scanf("%s", opt);
    } while (opt[0] == 'y' || opt[0] == 'Y');

    return 0;
}

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es troben */
/* a l'inici d'aquest fitxer.                                             */

/* Descripció de la funció, dels arguments, valors de retorn, etc.        */
/*int FuncioInterna(arg1, arg2...)
{

} */
