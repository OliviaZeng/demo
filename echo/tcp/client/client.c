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
    if (argc < 3) {
        printf("should be like this: ./client ip port\n");
        exit(1);
    }

    int sock;
    struct sockaddr_in serv_addr;
    char *ip = argv[1];
    in_port_t port = atoi(argv[2]);
    char *msg = "hello world";
    char message[30] = {0};
    int str_len, rev_len, rev_cnt;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("sock failed\n");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port   = htons(port);
    int ret = inet_pton(AF_INET, ip, &serv_addr.sin_addr);
    if (0 == ret) {
        fputs("Illegal address", stderr);
        fputs("\n", stderr);
        exit(1);

    } else if (-1 == ret) {
        perror("error");
        exit(1);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect err");
        exit(1);
    }

    while(1) {
        str_len = write(sock, msg, strlen(msg));
        rev_len = 0;
        while (rev_len < str_len) {
            rev_cnt = read(sock, &message[rev_len], sizeof(message)-1);
            if (-1 == rev_cnt) {
                perror("read error");
                exit(1);
            }
            rev_len += rev_cnt;
        }
        message[rev_len] = 0;
        printf("%s\n", message);
        sleep(1);
    }
    close(sock);
    return 0;
}
