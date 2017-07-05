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
        printf("should be like this: ./server port");
        exit(1);
    }


    int sock, sock_cli;
    struct sockaddr_in svr_addr, cli_addr;
    int str_len;
    char message[30] = {0};
    in_port_t port = atoi(argv[1]);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket err");
        exit(1);
    }

    memset(&svr_addr, 0, sizeof(struct sockaddr_in));
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_port = htons(port);
    svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr *)&svr_addr, sizeof(svr_addr)) < 0) {
        perror("bind err");
        exit(1);
    }

    if (listen(sock, 5) < 0) {
        perror("listen error");
        exit(1);
    }

    while (1) {
        socklen_t addr_size = sizeof(cli_addr);
        memset(&cli_addr, 0, sizeof(cli_addr));
        if ((sock_cli = accept(sock, (struct sockaddr *)&cli_addr, &addr_size)) < 0) {
            perror("accept error");
            exit(1);
        }

        char cli_name[200];
        if (NULL != inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, cli_name, sizeof(cli_name)))
            printf("client %s %d\n", cli_name, ntohs(cli_addr.sin_port));

        while ((str_len = read(sock_cli, message, 30)) != 0)
            write(sock_cli, message, str_len);
        close(sock_cli);
    }
    
    close(sock);
    return 0;
}
