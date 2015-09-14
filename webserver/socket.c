#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include "socket.h"

int creer_serveur(int port){

  int socket_serveur;
  int socket_client;
  const char *message_bienvenue = "Bonjour, bienvenue sur mon serveur\n";
  struct sockaddr_in saddr;
  saddr.sin_family = AF_INET; /* Socket ipv4 */
  saddr.sin_port = htons(port); /* Port d'écoute */
  saddr.sin_addr.s_addr = INADDR_ANY; /* écoute sur toutes les interfaces */

  socket_serveur = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_serveur == -1)
    {
      perror("socket_serveur");
      /* traitement de l'erreur */
    }
  /* Utilisation de la socket serveur */

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

  /* if (socket_client = accept(socket_serveur, NULL, NULL) != -1)*/
  socket_client = accept(socket_serveur, NULL, NULL);
  if(socket_client == -1)
    {
      perror("accept");
    }
      write(socket_client, message_bienvenue, strlen(message_bienvenue));
  /* Utilisation methode accept et message de bienvenue */

return 1;
}
