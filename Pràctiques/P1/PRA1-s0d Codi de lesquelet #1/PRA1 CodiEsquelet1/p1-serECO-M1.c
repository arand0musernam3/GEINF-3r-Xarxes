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

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */

/* int FuncioInterna(arg1, arg2...);                                      */

int main(int argc, char *argv[])
{
    /* Declaració de variables, p.e., int n;                                 */
    int socket_s;
    int socket_con;
    int port_s;
    char remIP[16];
    int remPort;
    char aux[30];

    /* Expressions, estructures de control, crides a funcions, etc.          */
    printf("Entra el port d'escolta del servidor: ");
    scanf("%d", &port_s);
    printf("Inicialitzant el servidor...\n");
    if ((socket_s = TCP_CreaSockServidor("0.0.0.0", port_s)) == -1) {
        printf("Error en crear el socket del servidor: %d\n", socket_s);
        return -1;
    }

    obtenirIpSock(socket_s, aux, 30);

    printf("Socket inicialitzat a %s\n", aux);
    char rebut[512];
    int RepRet;

    while(1) {
        socket_con = TCP_AcceptaConnexio(socket_s, remIP, &remPort);
        if (socket_con < 0) {
            printf("Error en TCP_AcceptaConnexio\n");
        }
        else {
            printf("Rebuda nova connexió\n");
            obtenirIpSock(socket_con, aux, 30);
            printf(" - SCON local: %s\n", aux);
            obtenirIpPeer(socket_con, aux, 30);
            printf(" - SCON remot: %s\n", aux);

            obtenirIpSock(socket_s, aux, 30);
            printf(" - SESC local: %s\n", aux);
            obtenirIpPeer(socket_s, aux, 30);
            printf(" - SESC remot: %s\n", aux);
        }

        while(1) {
            if ((RepRet = TCP_Rep(socket_con, rebut, 512)) < 0) {
                printf("%d", socket_con);
                printf("Error en la lectura de TCP_Rep\n");
                return -1;
            }
            else if (RepRet == 0) {
                printf("Connexio tancada amb el client (%s)\n\n", aux); //podem aprofitar valor anterior
                TCP_TancaSock(socket_con);
                break;
            }
            else {
                printf("Rebut: %s (%d bytes)\n", rebut, RepRet);
                if (TCP_Envia(socket_con, rebut, RepRet) < 0) {
                    printf("Error en l'enviament de l'ECO de %s", rebut);
                }
            }
        }
    }
}

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es troben */
/* a l'inici d'aquest fitxer.                                             */

/* Descripció de la funció, dels arguments, valors de retorn, etc.        */
/*int FuncioInterna(arg1, arg2...)
{

} */
