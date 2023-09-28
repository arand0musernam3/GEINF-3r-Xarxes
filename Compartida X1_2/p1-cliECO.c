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

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */

/* int FuncioInterna(arg1, arg2...);                                      */

int main(int argc, char *argv[])
{
    /* Declaració de variables, p.e., int n;                                 */
    char ip[LLARGADA_BUFFER_IP];
    int port;
    int socket_c;

    /* Expressions, estructures de control, crides a funcions, etc.          */

    // OBERTURA SOCKET PORT ALEATORI
    
    if (socket_c = TCP_CreaSockClient("0.0.0.0", 0) == -1)
    {
        printf("Error a TCP_CreaSockClient");
        return -1;
    }

    char aux[30];
    obtenirIpSock(socket_c, aux, 30);
    printf("Prova: %s\n", aux);

    // LLEGIR INPUT USUARI PER A CONNECTAR-SE:
    
    printf("Introdueix la direcció IP a la que et vols connectar:");
    scanf("%s", ip);
    printf("Introdueix el port al que et vols connectar:");
    scanf("%d", &port);
    printf("IP: %s, port: %d\n", ip, port);

    printf("%d\n", TCP_DemanaConnexio(socket_c, ip, port));
    obtenirIpPeer(socket_c, aux, 30);
    printf("Prova 2: %s\n", aux);
    
}

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es troben */
/* a l'inici d'aquest fitxer.                                             */

/* Descripció de la funció, dels arguments, valors de retorn, etc.        */
/*int FuncioInterna(arg1, arg2...)
{

} */
