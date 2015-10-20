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
  int longeur = strlen(message_bienvenue);
  char temp[2048];
  char *type, *slash, *protocol, *message;
  int pid;
  FILE * fichier;
  const char * error400 = "HTTP/1.1 400 Bad Request\nConnection: close \nContent-Length: 17 \n\n400 Bad Request\n";
  const char * ok200 = "HTTP/1.1 200 OK\nConnection: close\nContent-length: %d\n\n";
  const char * error404 = "HTTP/1.1 404 Bad Request\nConnection: close \nContent-Length: 17 \n\n404 Bad Request\n";
  while((socket_client = accept(socket_serveur, NULL, NULL)) != -1)
    {
      pid = fork();
      if(pid == 0){	
	fichier = fdopen(socket_client, "w+");
	if(fgets(temp, 2048, fichier) != NULL){
	  message = strdup(temp);
	  type = strtok(message, " ");
	  slash = strtok(NULL, " ");
	  protocol = strtok(NULL, " ");
	  if(strcmp(type, "GET") == 0 && (strncmp(protocol, "HTTP/1.1", 8) == 0 || strncmp(protocol, "HTTP/1.0", 8) == 0)){
	    printf("%s %s %s\n", type, slash, protocol);
	    while(fgets(temp, 2048, fichier) != NULL ){
	      if(temp[0] == '\n' || temp[strlen(temp)-1] != '\n'){
		break;
	      }
	      else if(temp[0] == '\r'){
		break;
	      }
	      else{
	      fprintf(stderr, "%s" ,temp);
	      }
	    }
 	    if(temp[strlen(temp)-1] != '\n'){
	      fprintf(fichier, "%s", error400);
	    }
	    else if(strcmp(slash, "/") == 0){
	      fprintf(fichier, ok200, longeur);
	      fprintf(fichier, message_bienvenue);
	    }
	    else if(strcmp(slash, "/") != 0){
	      fprintf(fichier, error404);
	    }
	  }
	  else{
	    fprintf(fichier, error400);
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
