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
    if (argc < 4) {
        printf("should be like this: ./client ip1 ip2 port\n");
        exit(1);
    }

    int sock;
    int str_len;
    char ip[INET_ADDRSTRLEN];
    socklen_t adr_sz;
    struct sockaddr_in svr_addr, from_addr;
    //char message[30] = {'h', 'e', 'l', 'l', 'o', '\0'};
	//char message[1024] = "{\"key\" : \"method_name\", \"lts_at\":1498702936, \"duration\":5, \"sys_ret\":0,\"bus_ret\":-1}";
	char message[1024] = {0};
	int i, stamp;

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket err");
        exit(1);
    }

    memset(&svr_addr, 0, sizeof(svr_addr));
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_port = htons(atoi(argv[3]));
    int ret = inet_pton(AF_INET, argv[1], &svr_addr.sin_addr);
    if (0 == ret) {
        perror("ip error");
        exit(1);
    } else if (-1 == ret) {
        perror("inet_pton errror");
        exit(1);
    }
	for (i=0; i<5; i++) {

		memset(message, 0, sizeof(message));
		stamp = 1498702900+rand()%100;

		sprintf(message, "{\"key\" : \"method_name\", \"lts_at\":%d, \"duration\":5, \"sys_ret\":0,\"bus_ret\":-1}", stamp);
		sendto(sock, message, strlen(message), 0, (struct sockaddr *)&svr_addr, sizeof(svr_addr));
	}
    if (NULL != inet_ntop(AF_INET, &svr_addr.sin_addr.s_addr, ip, sizeof(ip))) {
        printf("server %s %d\n", ip, svr_addr.sin_port);
    }
    adr_sz = sizeof(from_addr);
    str_len = recvfrom(sock, message, 30, 0, (struct sockaddr *)&from_addr, &adr_sz);
    message[str_len] = 0;
    if (NULL != inet_ntop(AF_INET, &from_addr.sin_addr.s_addr, ip, sizeof(ip))) {
        printf("server %s %d\n", ip, from_addr.sin_port);
    }
    printf("message from server: %s\n", message);


    sleep(5);


    memset(&svr_addr, 0, sizeof(svr_addr));
    memset(&from_addr, 0, sizeof(from_addr));
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_port = htons(atoi(argv[3]));
    ret = inet_pton(AF_INET, argv[2], &svr_addr.sin_addr);
    if (0 == ret) {
        perror("ip error");
        exit(1);
    } else if (-1 == ret) {
        perror("inet_pton errror");
        exit(1);
    }
    sendto(sock, message, strlen(message), 0, (struct sockaddr *)&svr_addr, sizeof(svr_addr));
    if (NULL != inet_ntop(AF_INET, &svr_addr.sin_addr.s_addr, ip, sizeof(ip))) {
        printf("server %s %d\n", ip, svr_addr.sin_port);
    }
    adr_sz = sizeof(from_addr);
    str_len = recvfrom(sock, message, 30, 0, (struct sockaddr *)&from_addr, &adr_sz);
    message[str_len] = 0;
    if (NULL != inet_ntop(AF_INET, &from_addr.sin_addr.s_addr, ip, sizeof(ip))) {
        printf("server %s %d\n", ip, from_addr.sin_port);
    }
    printf("message from server: %s\n", message);

    close(sock);
    return 0;
}
