#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc ,char *argv[])
{
	if (argc < 2) {
		printf("should be like this: ./server port\n");
		exit(1);
	}

	int sock, cli_sock;
	struct sockaddr_in svr_addr, cli_addr, tmp_addr;
	

	if ((sock=socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket() error\n");
		exit(1);
	}
		
	memset(&svr_addr, 0, sizeof(svr_addr));
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_port = htons(atoi(argv[1]));
	svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sock, (struct sockaddr *)&svr_addr, sizeof(svr_addr)) < 0) {
		perror("bind() error\n");
		exit(1);
	}

	if (listen(sock, 5) < 0) {
		perror("listen() error\n");
		exit(1);
	}

	while(1) {
		int len = sizeof(cli_addr);
		char cli_ip[INET_ADDRSTRLEN] = {0};
		char svr_ip[INET_ADDRSTRLEN] = {0};
		memset(&cli_addr, 0, len);
		if (cli_sock = accept(sock, (struct sockaddr *)&cli_addr, &len) < 0) {
			perror("accept() error");
			exit(1);
		}

		if (NULL != inet_ntop(AF_INET, (struct sockaddr *)&cli_addr, cli_ip, INET_ADDRSTRLEN)) {
            printf("client %s %d\n", cli_ip, ntohs(cli_addr.sin_port));
        }

        len =  sizeof(tmp_addr);
		memset(&tmp_addr, 0, sizeof(tmp_addr));
        if (getsockname(sock, (struct sockaddr *)&tmp_addr, &len) < 0) {
            perror("getsockname() error");
            exit(1);
        }

        if (NULL != inet_ntop(AF_INET, (struct sockaddr *)&tmp_addr, svr_ip, INET_ADDRSTRLEN))
            printf("server %s %d\n", svr_ip, atoi(argv[1]));

        close(cli_sock);
            

	}
    close(sock);

    return 0;

}

