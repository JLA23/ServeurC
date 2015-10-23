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
  char temp[2048];
  int pid;
  int req, fd;
  FILE * fichier;
  http_request requete;
  while((socket_client = accept(socket_serveur, NULL, NULL)) != -1){
    pid = fork();
    if(pid == 0){	
      fichier = fdopen(socket_client, "w+");
      fgets_or_exit(temp,sizeof(temp),fichier);
      skip_headers(fichier);
      req = parse_http_request(temp, &requete);
      fd = check_and_open(requete.url, "/home/eric/C/S4/pawnee/ServeurC/webserver/");
      fprintf(stderr, "%d\n", fd);
      if (!req){
	send_response(fichier, 400, "Bad Request", "400 : Bad Request");
      }
      else if(requete.method == HTTP_UNSUPPORTED){
	send_response(fichier, 405, "Method Not Allowed", "405 : Method Not Allowed");
      }
      else if(fd != -1){
	fprintf(stderr, "entrer");
	fprintf(fichier,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %du\r\n\r\n",get_file_size(fd));
	copy(fd,socket_client);
      }
      else{
	fprintf(stderr, "error");
	send_response(fichier, 404, "Not Found", "404 : Not Found");
	}
      fclose(fichier);			
    }
    
    else {
      close(socket_client);
    }
  } 
  return 1;	
}
