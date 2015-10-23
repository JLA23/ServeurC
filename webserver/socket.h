#ifndef __SOCKET_H__
#define __SOCKET_H__
/** Crée une socket serveur qui écoute sur toute les interfaces IPv4 de la machine sur le port passé en paramètre. La socket retournée doit pouvoir être utilisée directement par un appel à accept.

La fonction retourne -1 en cas d'erreur ou le descripteur de la socket créée. */

enum http_method{
  HTTP_GET,
  HTTP_UNSUPPORTED,
};

typedef struct{
  enum http_method method;
  int major_version;
  int minor_version;
  char *url;
} http_request;

int creer_serveur(int port);
void initialiser_signaux();
int parse_http_request(const char *request_line , http_request *request);
int check_and_open(const char *url, const char *document_root);
int get_file_size(int fd);
int copy(int in, int out);
char * fgets_or_exit(char *buffer , int size , FILE *stream);
char * rewrite_url(char *url);
void skip_headers(FILE *client);
void send_status(FILE *client , int code , const char *reason_phrase);
void send_response(FILE *client , int code , const  char *reason_phrase , const  char *message_body);
#endif
