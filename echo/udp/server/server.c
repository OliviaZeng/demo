#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("should be like this: ./server ip port\n");
        exit(1);
    }

    int sock;
    int str_len;
    char message[30];
    char ip[INET_ADDRSTRLEN];
    struct sockaddr_in svr_addr;

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket() error");
        exit(1);
    }

    memset(&svr_addr, 0, sizeof(svr_addr));
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_port   = htons(atoi(argv[2]));
    svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr *)&svr_addr, sizeof(svr_addr)) < 0) {
        perror("bind() error");
        exit(1);
    }

    while (1) {
        int svr_addr_len =  sizeof(svr_addr);
        str_len = recvfrom(sock, message, 30, 0, (struct sockaddr *)&svr_addr, &svr_addr_len);

        if (NULL != inet_ntop(AF_INET, &svr_addr.sin_addr.s_addr, ip, sizeof(ip))) {
            printf("client %s %d\n", ip, ntohs(svr_addr.sin_port));
        }
        sendto(sock, message, 30, 0, (struct sockaddr *)&svr_addr, svr_addr_len);
    }
    close(sock);
    return 0;

}
