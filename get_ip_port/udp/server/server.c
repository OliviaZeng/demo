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
    if (argc < 2) {
        printf("should be like this: ./server port\n");
        exit(1);
    }

    int sock;
    struct sockaddr_in svr_addr, cli_addr, local_addr;
    char msg[30] = {0};
    char ip[INET_ADDRSTRLEN] = {0};
    char local_ip[INET_ADDRSTRLEN] = {0};

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket() error");
        exit(1);
    }
    

    memset(&svr_addr, 0, sizeof(svr_addr));
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_port =  htons(atoi(argv[1]));
    svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sock, (struct sockaddr *)&svr_addr, sizeof(svr_addr)) < 0) {
        perror("bind() error");
        exit(1);
    }

    socklen_t len = sizeof(cli_addr);
    memset(&cli_addr, 0, len);
    recvfrom(sock, msg, sizeof(msg), 0, (struct sockaddr *)&cli_addr, &len);

    
    socklen_t local_len = sizeof(local_addr);
    memset(&local_addr, 0, local_len);
    if (getsockname(sock, (struct sockaddr *)&local_addr, &local_len) < 0) {
        perror("getsockname() error");
        exit(1);
    }


    if (NULL != inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, ip, sizeof(ip))) {
        printf("client %s %d\n", ip, ntohs(cli_addr.sin_port));
    }



    if (NULL != inet_ntop(AF_INET, &local_addr.sin_addr.s_addr, local_ip, sizeof(local_ip))) {
        printf("local client %s %d\n", local_ip, ntohs(local_addr.sin_port));
    }

    close(sock);
    return 0;
}

