/**************************************************************************/
/*                                                                        */
/* P2 - L'aplicació UEB amb sockets TCP/IP - Part I                       */
/* Fitxer aUEB.c que implementa la capa d'aplicació de UEB, sobre la      */
/* cap de transport TCP (fent crides a la "nova" interfície de la         */
/* capa TCP o "nova" interfície de sockets TCP), en la part servidora.    */
/*                                                                        */
/* Autors:                                                                */
/* Data:                                                                  */
/*                                                                        */
/**************************************************************************/

/* Inclusió de llibreries, p.e. #include <sys/types.h> o #include "meu.h" */
/*  (si les funcions externes es cridessin entre elles, faria falta fer   */
/*   un #include del propi fitxer capçalera)                              */

#include "p3-tTCP.h"
#include <stdlib.h>
#include <sys/types.h> //Specified in man 2 open
#include <sys/stat.h>
#include <errno.h> //Allows use of error numbers
#include <fcntl.h> //Specified in man 2 open
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

/* Definició de constants, p.e.,                                          */

/* #define XYZ       1500                                                 */
#define COR "COR\0"
#define ERR "ERR\0"
#define OBT "OBT\0"

#define MAX_LENGTH 10006

static char send_buf[MAX_LENGTH];
static char receive_buf[MAX_LENGTH];

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */

/* int FuncioInterna(arg1, arg2...);                                      */

int ConstiEnvMis(int SckCon, const char *tipus, const char *info1, int long1);
int RepiDesconstMis(int SckCon, char *tipus, char *info1, int *long1);

/* Definició de funcions EXTERNES, és a dir, d'aquelles que es cridaran   */
/* des d'altres fitxers, p.e., int UEBs_FuncioExterna(arg1, arg2...) { }  */
/* En termes de capes de l'aplicació, aquest conjunt de funcions externes */
/* formen la interfície de la capa UEB, en la part servidora.             */

/* Inicia el S UEB: crea un nou socket TCP "servidor" a una @IP local     */
/* qualsevol i al #port TCP “portTCPser”. Escriu l'identificador del      */
/* socket creat a "SckEsc".                                               */
/* Escriu un text que descriu el resultat de la funció a "TextRes".       */
/*                                                                        */
/* "TextRes" és un "string" de C (vector de chars imprimibles acabat en   */
/* '\0') d'una longitud màxima de 200 chars (incloent '\0').              */
/*                                                                        */
/* Retorna:                                                               */
/*  0 si tot va bé;                                                       */
/* -1 si hi ha un error en la interfície de sockets.                      */
int UEBs_IniciaServ(int *SckEsc, int portTCPser, char *TextRes)
{
    int aux = TCP_CreaSockServidor("0.0.0.0", portTCPser);
    if (aux <= 0) {
        strcpy(TextRes, "Error en inicialitzar el servidor\n\0");
        return -1;
    }
    *SckEsc = aux;
    sprintf(TextRes, "El servidor ha estat iniciat correctament (Socket d'escolta: %d)\n\0", *SckEsc);
    return 0;
}

/* Accepta una connexió d'un C UEB que arriba a través del socket TCP     */
/* "servidor" d'identificador "SckEsc". Escriu l'@IP i el #port TCP del   */
/* socket TCP "client" a "IPcli" i "portTCPcli", respectivament, i l'@IP  */
/* i el #port TCP del socket TCP "servidor" a "IPser" i "portTCPser",     */
/* respectivament.                                                        */
/* Escriu un text que descriu el resultat de la funció a "TextRes".       */
/*                                                                        */
/* "IPser" i "IPcli" són "strings" de C (vector de chars imprimibles      */
/* acabats en '\0') d'una longitud màxima de 16 chars (incloent '\0').    */
/* "TextRes" és un "string" de C (vector de chars imprimibles acabat en   */
/* '\0') d'una longitud màxima de 200 chars (incloent '\0').              */
/*                                                                        */
/* Retorna:                                                               */
/*  l'identificador del socket TCP connectat si tot va bé;                */
/* -1 si hi ha un error a la interfície de sockets.                       */
int UEBs_AcceptaConnexio(int SckEsc, char *IPser, int *portTCPser, char *IPcli, int *portTCPcli, char *TextRes)
{
    int aux = TCP_AcceptaConnexio(SckEsc, IPcli, portTCPcli);
    if (aux <= 0) {
        strcpy(TextRes, strerror(errno));
        return -1;
    }
    if (TCP_TrobaAdrSockLoc(aux, IPser, portTCPser) < 0) {
        strcpy(TextRes, strerror(errno));
        return -1;
    }
    sprintf(TextRes, "Nova connexió acceptada. Socket: %d\n\0", aux);
    return aux;

}

/* Serveix una petició UEB d'un C a través de la connexió TCP             */
/* d'identificador "SckCon". A "TipusPeticio" hi escriu el tipus de       */
/* petició (p.e., OBT) i a "NomFitx" el nom del fitxer de la petició.     */
/* Escriu un text que descriu el resultat de la funció a "TextRes".       */
/*                                                                        */
/* "TipusPeticio" és un "string" de C (vector de chars imprimibles acabat */
/* en '\0') d'una longitud de 4 chars (incloent '\0').                    */
/* "NomFitx" és un "string" de C (vector de chars imprimibles acabat en   */
/* '\0') d'una longitud <= 10000 chars (incloent '\0').                   */
/* "TextRes" és un "string" de C (vector de chars imprimibles acabat en   */
/* '\0') d'una longitud màxima de 200 chars (incloent '\0').              */
/*                                                                        */
/* Retorna:                                                               */
/*  0 si el fitxer existeix al servidor;                                  */
/*  1 la petició és ERRònia (p.e., el fitxer no existeix);                */
/* -1 si hi ha un error a la interfície de sockets;                       */
/* -2 si protocol és incorrecte (longitud camps, tipus de peticio, etc.); */
/* -3 si l'altra part tanca la connexió;                                  */
/* -4 si hi ha problemes amb el fitxer de la petició (p.e., nomfitxer no  */
/*  comença per /, fitxer no es pot llegir, fitxer massa gran, etc.).     */
int UEBs_ServeixPeticio(int SckCon, char *TipusPeticio, char *NomFitx, char *TextRes, const char* path)
{
    int longNomFitx, descFitx, aux;
    aux = RepiDesconstMis(SckCon, TipusPeticio, NomFitx, &longNomFitx);
    if (aux < 0) {
        switch (aux) {
            case -1:
                strcpy(TextRes, "Hi ha hagut un error a la interfície de sockets\n\0");
                break;
            case -2:
                strcpy(TextRes, "El protocol és incorrecte\n\0");
                break;
            case -3:
                strcpy(TextRes, "El client ha tancat la connexió\n\0");
                break;
        }
        return aux;
    }

    if (NomFitx[0] != '/') {
        strcpy(TextRes, "El protocol és correcte, però hi ha hagut problemes amb el fitxer de la petició\n\0");
        ConstiEnvMis(SckCon, ERR, "El format del fitxer és incorrecte. T'has oblidat la /?\n\0", 57);
        return -4;
    }

    char auxStr[512] = "";
    strcat(auxStr, path);
    strcat(auxStr, NomFitx);
    strcpy(NomFitx, auxStr);

    descFitx = open(NomFitx, O_RDONLY);
    if (descFitx < 0) {
        ConstiEnvMis(SckCon, ERR, "No s'ha trobat el fitxer.\0", 26);
        strcpy(TextRes, "El fitxer no s'ha trobat\n\0");
        return 1;
    }
    char fitxer[10000];
    int bytes_fitxer = read(descFitx, fitxer, 10000);

    ConstiEnvMis(SckCon, COR, fitxer, bytes_fitxer);

    strcpy(TextRes, "El fitxer ha estat enviat\n\0");

    return 0;
}

/* Tanca la connexió TCP d'identificador "SckCon".                        */
/* Escriu un text que descriu el resultat de la funció a "TextRes".       */
/*                                                                        */
/* "TextRes" és un "string" de C (vector de chars imprimibles acabat en   */
/* '\0') d'una longitud màxima de 200 chars (incloent '\0').              */
/*                                                                        */
/* Retorna:                                                               */
/*   0 si tot va bé;                                                      */
/*  -1 si hi ha un error a la interfície de sockets.                      */
int UEBs_TancaConnexio(int SckCon, char *TextRes)
{
	int aux = TCP_TancaSock(SckCon);
    if (aux == 0)
        strcpy(TextRes, "La connexió s'ha tancat correctament\n\0");
    else {
        strcpy(TextRes, "No s'ha pogut tancar la connexió correctament.\n\0");
    }
    return aux;
}

/* Si ho creieu convenient, feu altres funcions EXTERNES                  */

/* Descripció de la funció, dels arguments, valors de retorn, etc.        */
/* int UEBs_FuncioExterna(arg1, arg2...)
{
	
} */

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es        */
/* troben a l'inici d'aquest fitxer.                                      */

/* Descripció de la funció, dels arguments, valors de retorn, etc.        */
/* int FuncioInterna(arg1, arg2...)
{
	
} */

/* "Construeix" un missatge de PUEB a partir dels seus camps tipus,       */
/* long1 i info1, escrits, respectivament a "tipus", "long1" i "info1"    */
/* (que té una longitud de "long1" bytes), i l'envia a través del         */
/* socket TCP “connectat” d’identificador “SckCon”.                       */
/*                                                                        */
/* "tipus" és un "string" de C (vector de chars imprimibles acabat en     */
/* '\0') d'una longitud de 4 chars (incloent '\0').                       */
/* "info1" és un vector de chars (bytes) qualsevol (recordeu que en C,    */
/* un char és un enter de 8 bits) d'una longitud <= 9999 bytes.           */
/*                                                                        */
/* Retorna:                                                               */
/*  0 si tot va bé;                                                       */
/* -1 si hi ha un error a la interfície de sockets;                       */
/* -2 si protocol és incorrecte (longitud camps, tipus de peticio).       */
int ConstiEnvMis(int SckCon, const char *tipus, const char *info1, int long1)
{
    if (long1 > 9999 || long1 <= 0)
        return -2;

    if (!strcmp(tipus, OBT) || !strcmp(tipus, COR) || !strcmp(tipus, ERR)) {
        memcpy(send_buf, tipus, 3);
        sprintf(send_buf + 3, "%.4d", long1);
        memcpy(send_buf + 7, info1, long1);

        struct timeval start, end;
        gettimeofday(&start, 0);
        if (TCP_Envia(SckCon, send_buf, long1 + 7) < 0) {
            return -1;
        }
        gettimeofday(&end, 0);

        double te = (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)*1e-3;
        printf("Temps d'enviament: %f ms\n", te);
        printf("Velocitat efectiva: %f b/ms\n", long1 / te);

        return 0;
    } else {
        return -2;
    }
}

/* Rep a través del socket TCP “connectat” d’identificador “SckCon” un    */
/* missatge de PUEB i el "desconstrueix", és a dir, obté els seus camps   */
/* tipus, long1 i info1, que escriu, respectivament a "tipus", "long1"    */
/* i "info1" (que té una longitud de "long1" bytes).                      */
/*                                                                        */
/* "tipus" és un "string" de C (vector de chars imprimibles acabat en     */
/* '\0') d'una longitud de 4 chars (incloent '\0').                       */
/* "info1" és un vector de chars (bytes) qualsevol (recordeu que en C,    */
/* un char és un enter de 8 bits) d'una longitud <= 9999 bytes.           */
/*                                                                        */
/* Retorna:                                                               */
/*  0 si tot va bé,                                                       */
/* -1 si hi ha un error a la interfície de sockets;                       */
/* -2 si protocol és incorrecte (longitud camps, tipus de peticio);       */
/* -3 si l'altra part tanca la connexió.                                  */
int RepiDesconstMis(int SckCon, char *tipus, char *info1, int *long1)
{
    int aux = TCP_Rep(SckCon, receive_buf, MAX_LENGTH);
    if (aux == 0)
        return -3;
    if (aux < 0)
        return -1;
    if (strncmp(receive_buf, OBT, 3) && strncmp(receive_buf, COR, 3) && strncmp(receive_buf, ERR, 3))
        return -2;

    //ara sabem que el tipus és correcte
    memcpy(tipus, receive_buf, 3);
    tipus[3] = '\0';

    char auxVect[5];
    memcpy(auxVect, receive_buf+3, 4);
    auxVect[4] = '\0';
    if (!(*long1 = atoi(auxVect)))
        return -2;

    memcpy(info1, receive_buf+7, *long1);

    info1[*long1] = '\0';

    return 0;
}

/* Examina simultàniament i sense límit de temps (una espera indefinida)  */
/* els sockets (poden ser TCP, UDP i  teclat -stdin-) amb identificadors  */
/* en la llista “LlistaSck” (de longitud “LongLlistaSck” sockets) per     */
/* saber si hi ha arribat alguna cosa per ser llegida, excepte aquells    */
/* que tinguin identificadors igual a -1.                                 */
/* Escriu un text que descriu el resultat de la funció a "TextRes".       */
/*                                                                        */
/* "LlistaSck" és un vector d'int d'una longitud d'almenys LongLlistaSck. */
/* "TextRes" és un "string" de C (vector de chars imprimibles acabat en   */
/* '\0') d'una longitud màxima de 200 chars (incloent '\0').              */
/*                                                                        */
/* Retorna:                                                               */
/*  l'identificador del socket a través del qual ha arribat alguna cosa;  */
/*  -1 si hi ha error.                                                    */
int UEBs_HaArribatAlgunaCosaPerLlegir(const int *LlistaSck, int LongLlistaSck, char *TextRes)
{
	int sock = T_HaArribatAlgunaCosaPerLlegir(LlistaSck, LongLlistaSck, -1);
    switch (sock) {
        case -1:
            text_res = "Hi ha hagut un error al select\n\0";
            break;
        case -2:
            text_res = "Timeout al select \n\0";
            break;
        default:
            sprintf(text_res, "Ha arribat alguna cosa per llegir al socket %d\n\0", sock)
    }
    return sock;
}