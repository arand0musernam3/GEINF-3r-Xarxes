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

#include "p2-aUEBs.h"
#include <stdio.h>

int main(int argc,char *argv[])
{
    //sempre hi haurà dos bucles infinits, el primer de serveix a dins, fins que es retorni -3 que llavors es tancarà la connexió. Fer un sleep al final tal i com diu l'enunciat.
    // L'altre bucle més gros és el d'acceptar connexions, també és infinit fins ctrl C.
    
    // Declaració de variables, p.e., int n;    
    int socket_s;
    int socket_con;
    int port_s;
    char text_res[200];

    char locIP[16];
    int locPort;
    char remIP[16];
    int remPort;

    /* Expressions, estructures de control, crides a funcions, etc.          */
    FILE *fp;
    char linia[50];

    if ((fp = fopen("serUEB.cfg","r")) > 0 && fgets(linia,sizeof(linia),fp) == NULL) {
        printf("No s'ha pogut trobar el port a obrir")
        return -1;
    }

    port_s = atoi(linia);

    if ((socket_s = UEBs_IniciaServ(socket_con, port_s, text_res)) == -1) {
        printf("%s",text_res);
        return -1;
    }

    printf("Servidor inicialitzat\n");

    while (1) {
        if ((socket_con = UEBs_AcceptaConnexio(soccket_s, locIP, locPort, remIP, remPort, text_res)) < 0) {
            printf("%s", text_res);
        }

        while (1) {
            char tipus[4], char nom_fitxer[10000];
            int res = UEBs_ServeixPeticio(socket_con, tipus, nom_fitxer, text_res);
            printf("Servida petició: %s %s de %s:%d a %s:%d\n", tipus, nom_fitxer, remIP, remPort, locIP, remIP);
            if (res == 0) {
                printf("Fitxer servir\n");
            }
            else if (res == 1) {
                printf("Fitxer inexistent\n");
            }
            else {
                printf("%s", text_res);
            }
        }
    }
    
    return 0;
}
