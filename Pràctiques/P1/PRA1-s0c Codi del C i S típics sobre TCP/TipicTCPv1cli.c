/**************************************************************************/
/*                                                                        */
/* Client TCP típic (v1: socket/no fa bind/connect/read,write/close)      */
/* L'@socket del S (@IP,#portTCP) està fixada en el codi                  */
/* El C es connecta al S (s'estableix una connexió TCP)                   */
/* L'usuari entra una línia via teclat, i el C l'envia al S               */
/* El client tanca el socket, i per tant la connexió TCP, i s'acaba       */
/*                                                                        */
/* Codi extret de "La interfície de sockets de C a UNIX", al Moodle       */
/*                                                                        */
/**************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
 int scon, i;
 int bytes_llegits, bytes_escrits;
 char buffer[200];
 struct sockaddr_in adrrem;
 char iprem[16];
 int portrem;

 /* 1) Crida socket()                                                                    */
 /* Es crea el socket TCP scon del client (el socket "local"), que de moment no té       */
 /* adreça (@IP i #port TCP) assignada.                                                  */
 if((scon=socket(AF_INET,SOCK_STREAM,0))==-1)
 {
  perror("Error en socket");
  exit(-1);
 }

 /* 2) Crida bind(), opcional en el client                                               */
 /* Per assignar adreça a scon (@IP i #port TCP) el programador ho pot fer de 2 maneres: */
 /* i) explícitament, és a dir, assignant uns valors concrets en el codi, fent bind()    */
 /* amb aquests valors; ii) implícitament, és a dir, deixant que els assigni el S.O.,    */
 /* que escull una @IP qualsevol de la màquina i un #port TCP que estigui lliure. Una    */
 /* assignació implícita es pot fer de 2 maneres: i) fent bind() amb @IP 0.0.0.0 (que    */
 /* vol dir qualsevol, és a dir, totes les @IP de la màquina) i #port TCP 0 (llavors,    */
 /* després del bind(), el S.O. haurà assignat un #port TCP, i després del connect() el  */
 /* S.O. haurà assignat una @IP, la de la interfície de sortida); ii) no fent bind()     */
 /* (llavors, després del connect(), el S.O. haurà assignat un #port TCP i una @IP, la   */
 /* de la interfície de sortida).                                                        */
 /* Aquí es fa una assignació implícita: o no fer bind() o fer bind(0.0.0.0,0)           */
 
 /* 3) Crida connect()                                                                   */
 /* Es connecta scon al socket del servidor (el socket "remot"). Primer s’omple adrrem   */
 /* amb l’adreça del socket remot (@IP i #port TCP) i després es fa connect(). A més,    */
 /* com que abans no s'ha fet bind(), ara connect() farà que s’assigni una adreça a scon */
 /* (@IP i #port TCP) de manera implícita (l'@IP de la interfície de xarxa per on surti  */
 /* i un #port TCP qualsevol lliure).                                                    */
 /* L'adreca del socket remot és @IP 10.0.0.23 i #port TCP 3000.                         */
 strcpy(iprem,"10.0.0.23");
 portrem = 3000;

 adrrem.sin_family=AF_INET;
 adrrem.sin_port=htons(portrem);
 adrrem.sin_addr.s_addr= inet_addr(iprem);
 for(i=0;i<8;i++){adrrem.sin_zero[i]='\0';}

 if((connect(scon,(struct sockaddr*)&adrrem,sizeof(adrrem)))==-1)
 {
  perror("Error en connect");
  close(scon);
  exit(-1);
 }

 /* Un cop fet connect() es diu que el socket scon està "connectat" al socket remot.     */
 /* Com que és un socket TCP això també vol dir que s'ha establert una connexió TCP.     */

 /* 4) Crida write()                                                                     */ 
 /* S'envia pel socket connectat scon el que es llegeix del teclat                       */
 if((bytes_llegits=read(0,buffer,sizeof(buffer)))==-1)
 {
  perror("Error en read");
  close(scon);
  exit(-1);
 }
 if((bytes_escrits=write(scon,buffer,bytes_llegits))==-1)
 {
  perror("Error en write");
  close(scon);
  exit(-1);
 }

 /* 5) Crida close()                                                                     */ 
 /* Es tanca el socket scon, que com que és un socket TCP, també vol dir que es tanca la */
 /* connexió TCP establerta.                                                             */
 if(close(scon)==-1)
 {
  perror("Error en close");
  exit(-1);
 }

 exit(0);
}
