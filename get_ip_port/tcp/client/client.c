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
	struct sockaddr_in svr_addr;
	struct sockaddr_in cli_addr;

	if ((sock=socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket() err\n");
		exit(1);
	}

	memset(&svr_addr, 0, sizeof(svr_addr));
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_port = htons(atoi(argv[2]));
	int ret = inet_pton(AF_INET, argv[1], &svr_addr.sin_addr.s_addr);
	if (0 == ret) {
		perror("illegal addr\n");
		exit(1);
	} else if (-1 == ret) {
		perror("inet_pton() error\n");
		exit(1);
	}

	if (connect(sock, (struct sockaddr *)&svr_addr, sizeof(svr_addr)) < 0) {
		perror("connect() error\n");
		exit(1);
	}

	char cli_ip[INET_ADDRSTRLEN];
	int len = sizeof(cli_addr);
	memset(&cli_addr, 0, len);
	if (0 == getsockname(sock, (struct sockaddr *)&cli_addr, &len))
		if (NULL != inet_ntop(AF_INET, (struct sockaddr *)&cli_addr, cli_ip, sizeof(cli_ip))) {
			printf("client %s %d\n", cli_ip, ntohs(cli_addr.sin_port));
			printf("server %s %d\n", argv[1],atoi(argv[2]));
		}

    pause();

	close(sock);

	return 0;
}
