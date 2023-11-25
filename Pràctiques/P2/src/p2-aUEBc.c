/**************************************************************************/
/*                                                                        */
/* P2 - L'aplicació UEB amb sockets TCP/IP - Part I                       */
/* Fitxer aUEBc.c que implementa la capa d'aplicació de UEB, sobre la     */
/* cap de transport TCP (fent crides a la "nova" interfície de la         */
/* capa TCP o "nova" interfície de sockets TCP), en la part client.       */
/*                                                                        */
/* Autors:                                                                */
/* Data:                                                                  */
/*                                                                        */
/**************************************************************************/

/* Inclusió de llibreries, p.e. #include <sys/types.h> o #include "meu.h" */
/*  (si les funcions externes es cridessin entre elles, faria falta fer   */
/*   un #include del propi fitxer capçalera)                              */


#include "p2-tTCP.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH 10006

static char send_buf[MAX_LENGTH];
static char receive_buf[MAX_LENGTH];

/* int FuncioInterna(arg1, arg2...);                                      */

int ConstiEnvMis(int SckCon, const char *tipus, const char *info1, int long1);
int RepiDesconstMis(int SckCon, char *tipus, char *info1, int *long1);

/* Definició de funcions EXTERNES, és a dir, d'aquelles que es cridaran   */
/* des d'altres fitxers, p.e., int UEBc_FuncioExterna(arg1, arg2...) { }  */
/* En termes de capes de l'aplicació, aquest conjunt de funcions externes */
/* formen la interfície de la capa UEB, en la part client.                */

/* Crea un socket TCP "client" en una @IP i #port TCP local qualsevol, a  */
/* través del qual demana una connexió a un S UEB que escolta peticions   */
/* al socket TCP "servidor" d'@IP "IPser" i #portTCP "portTCPser".        */
/* Escriu l'@IP i el #port TCP del socket "client" creat a "IPcli" i      */
/* "portTCPcli", respectivament.                                          */
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
int UEBc_DemanaConnexio(const char *IPser, int portTCPser, char *IPcli, int *portTCPcli, char *TextRes)
{
    int sck = TCP_CreaSockClient("0.0.0.0", 0);
    if (TCP_DemanaConnexio(sck, IPser, portTCPser) == -1) {
        strcpy(TextRes, "Connexió no acceptada\n\0");
        return -1;
    }
    strcpy(TextRes, "Connexió acceptada\n\0");
    TCP_TrobaAdrSockLoc(sck, IPcli, portTCPcli);

    return sck;
}

/* Obté el fitxer de nom "NomFitx" del S UEB a través de la connexió TCP  */
/* d'identificador "SckCon". Escriu els bytes del fitxer a "Fitx" i la    */
/* longitud del fitxer en bytes a "LongFitx".                             */
/* Escriu un text que descriu el resultat de la funció a "TextRes".       */
/*                                                                        */
/* "NomFitx" és un "string" de C (vector de chars imprimibles acabat en   */
/* '\0') d'una longitud <= 10000 chars (incloent '\0').                   */
/* "Fitx" és un vector de chars (bytes) qualsevol (recordeu que en C,     */
/* un char és un enter de 8 bits) d'una longitud <= 9999 bytes.           */
/* "TextRes" és un "string" de C (vector de chars imprimibles acabat en   */
/* '\0') d'una longitud màxima de 200 chars (incloent '\0').              */
/*                                                                        */
/* Retorna:                                                               */
/*  0 si el fitxer existeix al servidor;                                  */
/*  1 la petició és ERRònia (p.e., el fitxer no existeix);                */
/* -1 si hi ha un error a la interfície de sockets;                       */
/* -2 si protocol és incorrecte (longitud camps, tipus de peticio, etc.); */
/* -3 si l'altra part tanca la connexió.                                  */
int UEBc_ObteFitxer(int SckCon, const char *NomFitx, char *Fitx, int *LongFitx, char *TextRes)
{
    int aux;
    aux = ConstiEnvMis(SckCon, "OBT", NomFitx, strlen(NomFitx));
    if (aux < 0)
        return aux;

    char tipus[4];
    aux = RepiDesconstMis(SckCon, tipus, Fitx, LongFitx);
    if (aux < 0)
        return aux;

    if (!strcmp(tipus, "ERR")) {
        strncpy(TextRes, Fitx, *LongFitx > 200 ? 200 : *LongFitx);
        return 1;
    }

    //ha arribat el fitxer correctament
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
int UEBc_TancaConnexio(int SckCon, char *TextRes)
{
    int aux = TCP_TancaSock(SckCon);
    if (aux == 0)
        strcpy(TextRes, "Tot ha anat bé.\0");
    else {
        strcpy(TextRes, strerror(aux));
    }
    return aux;
}

/* Si ho creieu convenient, feu altres funcions EXTERNES                  */

/* Descripció de la funció, dels arguments, valors de retorn, etc.        */
/* int UEBc_FuncioExterna(arg1, arg2...)
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

	if (!strcmp(tipus, "OBT") || !strcmp(tipus, "COR") || !strcmp(tipus, "ERR"))  {
        memcpy(send_buf, tipus, 3);
        sprintf(send_buf + 3, "%.4d", long1);
        memcpy(send_buf + 7, info1, long1);

        if (TCP_Envia(SckCon, send_buf, long1 + 7) < 0) {
            return -1;
        }
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
/*  0 si tot va bé;                                                       */
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
    if (strncmp(receive_buf, "OBT", 3) && strncmp(receive_buf, "COR", 3) && strncmp(receive_buf, "ERR", 3))
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
    return 0;
}
