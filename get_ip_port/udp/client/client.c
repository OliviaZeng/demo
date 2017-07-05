#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
   if (argc < 3)  {
       printf("should be like this: ./client ip port\n");
       exit(1);
       return 0;
   }

   int sock;
   char *msg = "hello";
   struct sockaddr_in svr_addr, tmp_addr;

   if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
       perror("socket() error");
       exit(1);
   }

   memset(&svr_addr, 0, sizeof(svr_addr));
   svr_addr.sin_family = AF_INET;
   svr_addr.sin_port   = htons(atoi(argv[2]));
   int ret = inet_pton(AF_INET, argv[1], &svr_addr.sin_addr);
   if (0 == ret) {
       perror("ip error");
       exit(1);
   } else if (-1 == ret) {
       perror("inet_pton() error");
       exit(1);
   }

   sendto(sock, msg, strlen(msg), 0, (struct sockaddr *)&svr_addr, sizeof(svr_addr));

   socklen_t len = sizeof(tmp_addr);
   memset(&tmp_addr, 0, len);
   if (getsockname(sock, (struct sockaddr *)&tmp_addr, &len) < 0) {
       perror("getsockname() errro");
       exit(1);
   }

   char cli_ip[INET_ADDRSTRLEN] = {0};
   if (NULL != inet_ntop(AF_INET, (struct sockaddr *)&tmp_addr, cli_ip, sizeof(cli_ip)))
       printf("client %s %d\n", cli_ip, ntohs(tmp_addr.sin_port));

   pause();

   close(sock);
   return 0;
   

}

