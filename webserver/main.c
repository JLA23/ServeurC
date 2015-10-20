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
  char temp[2048];
  char *type, *slash, *protocol, *message;
  int pid;
  FILE * fichier;
  const char * error404 = "HTTP/1.1 400 Bad Request\nConnection: close \nContent-Length: 17 \n\n400 Bad Request";
  while((socket_client = accept(socket_serveur, NULL, NULL)) != -1)
    {
      pid = fork();
      if(pid == 0){
	//sleep(3);
        if(write(socket_client, message_bienvenue, strlen(message_bienvenue)) == -1) {
	  perror("Error Write Welcome");
	  /* traitement d’erreur */
	}
	fichier = fdopen(socket_client, "w+");
        fprintf(fichier, "test\n");
	if(fgets(temp, 2048, fichier) != NULL){
	  fprintf(stderr, "ligne %s", temp);
	  message = strdup(temp);
	  type = strtok(message, " ");
	  slash = strtok(NULL, " ");
	  protocol = strtok(NULL, " ");
	  if(strcmp(type, "GET") == 0 && (strncmp(protocol, "HTTP/1.1", 8) == 0 || strncmp(protocol, "HTTP/1.0", 8) == 0)){
	    printf("%s %s %s", type, slash, protocol);
	    while(fgets(temp, 2048, fichier) != NULL && (temp[0] != '\n' || temp[0] != '\r')){
	      // fprintf(stderr, "DEBUG [%s]" ,temp);
	      
	    }
	    // fprintf(stderr, "DEBBUG2[%s]", temp);
	    if(temp[0] == '\n' || temp[0] == '\r'){
	       // fprintf(stderr, "Ici \n");
	      fprintf(fichier, "%s", error404);
	    }
	  }
	  else{
	    fprintf(fichier, "test error\n");
	    printf("%s",error404);
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
