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
#include <string.h>
#include <stdlib.h>

int read_config(char* path, int* port) {

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

    char locIP[16];
    int locPort;
    char remIP[16];
    int remPort;

    char path[300];

    // Fitxer log
    int fd = open("serUEB.log", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    write(fd, fitxer, long_fitxer);
    
    read_config(path, &port_s);

    char buffer[1000];
    int l = sprintf(buffer, "Port d'escolta: %d\n", port_s);
    write(fd, buffer, l);
    l = sprintf(buffer, "Arrel de src: %s\n", path);
    write(fd, buffer, l);

    printf("Port d'escolta: %d\n", port_s);

    if (UEBs_IniciaServ(&socket_s, port_s, text_res) == -1) {
        write(fd, text_res, strlen(text_res));
        printf("%s",text_res);
        return -1;
    }


    write(fd, text_res, strlen(text_res));
    printf("%s", text_res);

    while (1) {
        if ((socket_con = UEBs_AcceptaConnexio(socket_s, locIP, &locPort, remIP, &remPort, text_res)) < 0) {
            write(fd, text_res, strlen(text_res));
            printf("%s", text_res);
            continue;
        }

        write(fd, text_res, strlen(text_res));
        printf("%s", text_res);

        while (1) {
            char tipus[4], nom_fitxer[10000];
            int res = UEBs_ServeixPeticio(socket_con, tipus, nom_fitxer, text_res, path);

            if (res == 0 || res == 1) {
                l = sprintf(buffer, "Petició rebuda: %s %s de %s:%d a %s:%d pel socket %d\n", tipus, nom_fitxer, remIP, remPort, locIP, locPort, socket_con);
                write(fd, buffer, l);
                printf("Petició rebuda: %s %s de %s:%d a %s:%d pel socket %d\n", tipus, nom_fitxer, remIP, remPort, locIP, locPort, socket_con);

                if (res == 0) {
                    write(fd, text_res, strlen(text_res));
                    printf("%s", text_res);
                }
                else {
                    write (fd, text_res, strlen(text_res));
                    printf("%s", text_res);
                }
            }
            else {
                write(fd, text_res, strlen(text_res));
                printf("%s", text_res);
            }
        }
    }
    
    return 0;
}
