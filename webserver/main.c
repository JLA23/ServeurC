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
   const char *message_bienvenue = "Bonjour, bienvenue sur mon serveur\n";
  int socket_client;
  int socket_serveur = creer_serveur(8080);
  char temp[2048];
  int pid;
  int req = 0;
  FILE * fichier;
  http_request requete;
  while((socket_client = accept(socket_serveur, NULL, NULL)) != -1){
    pid = fork();
    if(pid == 0){	
      fichier = fdopen(socket_client, "w+");
      fgets_or_exit(temp,sizeof(temp),fichier);
      req = parse_http_request(temp, &requete);
      skip_headers(fichier);
      if (!req){
	send_response(fichier, 400, "Bad Request", "400 : Bad Request");
      }
      else if(requete.method == HTTP_UNSUPPORTED){
	send_response(fichier, 405, "Method Not Allowed", "405 : Method Not Allowed");
      }
      else if(strcmp(requete.url, "/") == 0){
	send_response(fichier, 200, "OK", message_bienvenue);
      }
      else{
	send_response(fichier, 404, "Not Found", "404 : Not Found");
      }
      fclose(fichier);			
    }
    
    /* Utilisation methode accept et message de bienvenue */
    else {
      close(socket_client);
    }
  } 
  return 1;	
}
