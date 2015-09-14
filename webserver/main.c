#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include "socket.h"

int main(){
  int socket_client;
  int socket_serveur = creer_serveur(8080);
  const char *message_bienvenue = "Bonjour, bienvenue sur mon serveur\n";
  int pid;
  while((socket_client = accept(socket_serveur, NULL, NULL)) != -1)
    {
      pid = fork();
      if(pid == 0){
	sleep(1);
	write(socket_client, message_bienvenue, strlen(message_bienvenue));
      }
  /* Utilisation methode accept et message de bienvenue */
    }
 return 1;	
}
