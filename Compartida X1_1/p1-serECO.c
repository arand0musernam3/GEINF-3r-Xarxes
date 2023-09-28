/**************************************************************************/
/*                                                                        */
/* P1 - L'aplicació ECO amb sockets TCP/IP                                */
/* Fitxer serECO.c que implementa el servidor ECO sobre la capa de        */
/* transport TCP (fent crides a la "nova" interfície de la capa de        */
/* transport o "nova" interfície de sockets).                             */
/*                                                                        */
/* Autors: Jordi Badia i Aniol Juanola                                    */
/* Data:                                                                  */
/*                                                                        */
/**************************************************************************/

/* Inclusió de llibreries, p.e. #include <stdio.h> o #include "meu.h"     */

#include "p1-tTCP.h"
#include "stdio.h"

/* Definició de constants, p.e.,                                          */

/* #define XYZ       1500                                                 */
#define TCP_PORT    3333

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */

/* int FuncioInterna(arg1, arg2...);                                      */

int main(int argc, char *argv[])
{
    /* Declaració de variables, p.e., int n;                                 */
    int socket_s;
    int port;
    char remIP[16];
    int remPort;
    char aux[30];

    /* Expressions, estructures de control, crides a funcions, etc.          */
    printf("Inicialitzant el servidor...\n");
    if (socket_s = TCP_CreaSockServidor("0.0.0.0", TCP_PORT) == -1) {
        printf("Error en crear el socket del servidor\n");
        return -1;
    }

    obtenirIpSock(socket_s, aux, 30);

    printf("Socket inicialitzat a %s\n", aux);

    socket_s = TCP_AcceptaConnexio(socket_s, remIP, &remPort);

}

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es troben */
/* a l'inici d'aquest fitxer.                                             */

/* Descripció de la funció, dels arguments, valors de retorn, etc.        */
/*int FuncioInterna(arg1, arg2...)
{

} */
