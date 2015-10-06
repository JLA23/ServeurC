#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <stdlib.h>
#include "socket.h"

int main(){
  int socket_client;
  int socket_serveur = creer_serveur(8080);
  const char *message_bienvenue = "Bonjour, bienvenue sur mon serveur\n";
  /*int longueur = 0;*/
  char temp[256];
  char *type, *slash, *protocol, *message;
  int pid;
  while((socket_client = accept(socket_serveur, NULL, NULL)) != -1)
    {
      pid = fork();
      if(pid == 0){
	FILE *fichier;
	fichier = fdopen(socket_client, "w+");
	sleep(3);
        if(write(socket_client, message_bienvenue, strlen(message_bienvenue)) == -1)
         {
	  perror("Error Write Welcome");
		/* traitement d’erreur */
	 }
	if(fgets(temp, 256, fichier) != NULL){
	  message = strdup(temp);
	  type = strtok(message, " ");
	  slash = strtok(NULL, " ");
	  protocol = strtok(NULL, " ");
	  if(strcmp(type, "GET") == 0 && strcmp(slash, "/") == 0 && (strncmp(protocol, "HTTP/1.1", 8) == 0 || strncmp(protocol, "HTTP/1.0", 8) == 0)){
	    while(fgets(temp, 256, fichier) != NULL && (temp[0] != '\n' || temp[0] != '\r')){
		printf("%s %s", "<Pawnee>", temp);
	      }
	    }
	}
  /* Utilisation methode accept et message de bienvenue */
	fclose(fichier);
      }   
    else {
      close(socket_client);
     }
    } 
 return 1;	
}
