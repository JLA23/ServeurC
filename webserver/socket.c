#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <signal.h>
#include "socket.h"
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h>

int creer_serveur(int port){
  
  int socket_serveur;
  struct sockaddr_in saddr;
  saddr.sin_family = AF_INET; /* Socket ipv4 */
  saddr.sin_port = htons(port); /* Port d'écoute */
  saddr.sin_addr.s_addr = INADDR_ANY; /* écoute sur toutes les interfaces */
  int optval = 1;
  initialiser_signaux();
  
  socket_serveur = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_serveur == -1)
    {
      perror("socket_serveur");
      /* traitement de l'erreur */
    }
  /* Utilisation de la socket serveur */
  
  if(setsockopt(socket_serveur, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
    {
      perror("Can not set SO_REUSADDR option");
      /*traitement d'erreur*/
    }
  /*Activation de l'option SO_REUSEADDR*/  
  
  if (bind(socket_serveur, (struct sockaddr *)&saddr, sizeof(saddr)) == -1)
    {
      perror("bind socker_serveur");
      /* traitement d'erreur */
    }
  /* Utilisation de la methode bind */
  
  if (listen(socket_serveur, 10) == -1)
    {
      perror("listen socket_serveur");
      /* traitement d'erreur */
    }
  /* Utilisation de la methode */
  
  return socket_serveur;
}

int parse_http_request(const char *request_line , http_request *request){
  fprintf(stderr, "%s", request_line);
  char* str = strdup(request_line);
  char* met = strtok(str," ");
  char* url = strtok(NULL, " ");
  char* http_version = strtok(NULL, " ");
  if(url != NULL){
    request->url=rewrite_url(url);
  }
  if(met == NULL || url == NULL || http_version == NULL){
    return 0;
  }
  if(strcmp(met,"GET") == 0){
    request->method = HTTP_GET;
  }
  else{
    request->method = HTTP_UNSUPPORTED;
  }
  if((strncmp(http_version,"HTTP/1.0", 8)==0)){
    request->major_version=1;
    request->minor_version=0;
  }
  else if(strncmp(http_version,"HTTP/1.1", 8)==0){
    request->major_version=1;
    request->minor_version=1;
  }
  else{
    return 0;
  }
  return 1;
}

char * fgets_or_exit(char *buffer , int size , FILE *stream){
  char *buff;	
  if((buff = fgets(buffer, size, stream)) == NULL) {
    exit(1);
  }
  return buff;
}


void skip_headers(FILE *client){
  char buf[1024];
  while(buf[0] != '\r' && buf[1] != '\n'){
    fgets_or_exit(buf, sizeof(buf), client);
  }
}

char * rewrite_url(char *url){
  char * res;
  char * resultat;
  if((res = strchr(url, '?'))!=NULL){
    resultat = strtok(url, "?");
    fprintf(stderr,"%s", resultat);
    return resultat;
  }
  else{
     fprintf(stderr,"%s", url);
    return url;
      }
}

void send_status(FILE *client , int code , const char *reason_phrase){
	char status[256];
	sprintf(status, "HTTP/1.1 %d %s\r\n", code, reason_phrase);
	fprintf(client, "%s", status);
}

void send_response(FILE *client , int code , const  char *reason_phrase , const  char *message_body){
	send_status(client, code, reason_phrase);
	if(message_body != NULL) {
		char content_length[256];
		sprintf(content_length, "Content-Length: %zu\r\n", strlen(message_body));
		fprintf(client, "Content-Type: text/html\r\n");
		fprintf(client, "%s", content_length);
		fprintf(client, "\r\n");
		fprintf(client, "%s", message_body);
	}
	fprintf(client, "\r\n");
}

int check_and_open(const char *url, const char *document_root){
  char *address;
  char *file;
  char temp [1024];
  int f;
  address=strdup(url);
  file=strdup(document_root);
  strcat(temp,file);
  strcat(temp,address);
  f=open(temp,O_RDONLY);
  if(f==-1){
    fprintf(stderr,"Error open");
    return -1;
  }
  return f;
}

int get_file_size(int fd){ 
  struct stat s;
  if(fstat(fd,&s)==0){
    return s.st_size;
  }
  return 0;
  
}

int copy(int in, int out){
  char buff [2048];
  int lecture;
  lecture=read(in, buff, strlen(buff));
  if(lecture!=-1){
    write(out, buff, strlen(buff));
  }
  else{
    fprintf(stderr, "-1");
    return -1;
  }
  fprintf(stderr, "0");
  return 0;
}

void traitement_signal(int sig){
  printf("Signal %d recu\n", sig);
  waitpid(-1, &sig, WNOHANG);
}

void initialiser_signaux(){
  struct sigaction sa;
  sa.sa_handler = traitement_signal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if(sigaction(SIGCHLD, &sa, NULL) == -1){
    perror("sigaction(SIGCHLD)");
  }
}

