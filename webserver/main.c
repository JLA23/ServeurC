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
	
	while(fgets(temp, 256, fichier) != NULL){
	  fprintf(fichier, "%s %s", "<Pawnee>", temp);
	} 
	/*while((longueur = read(socket_client, temp, 256)) != 0)
         {
	  if(write(socket_client, temp, longueur) == -1)
           {
	    perror("Error Write Welcome 2");
	     traitement d’erreur 
	   }
         }*/
  /* Utilisation methode accept et message de bienvenue */
	fclose(fichier);
     }
    else {
      close(socket_client);
     } 
   }
 return 1;	
}
