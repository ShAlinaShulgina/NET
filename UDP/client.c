#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <errno.h>
#include <unistd.h>

char str[128];
#define port 4000
#define server "127.0.0.1"

int main()
{
	int sockfd;
	struct sockaddr_in sock_server;

	//domain указывающий семейство протоколов создаваемого сокета    AF_INET для сетевого протокола IPv4
	sock_server.sin_family = AF_INET;
	//Функция htons осуществляет перевод целого короткого числа из порядка байт, принятого на компьютере, в сетевой порядок байт.
	sock_server.sin_port = htons(port);
	//Функция htonl осуществляет перевод целого длинного числа из порядка байт, принятого на компьютере, в сетевой порядок байт.
	sock_server.sin_addr.s_addr = htonl(INADDR_ANY);

	if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
        perror("error"); 
        exit(1);
    }
	
	unsigned int socklen = sizeof(sock_server);
	inet_aton(server, &sock_server.sin_addr); 

	while(1)
	{
		printf("=====> String\n");
		memset(str, '\0', strlen(str));
		scanf("%s", str);
		printf("%d\n", strlen(str));
		if (sendto(sockfd, str, strlen(str), 0, (struct sockaddr* ) &sock_server, socklen) < 0)
		{
			perror("sendto");
    	    close(sockfd);
       		exit(1);
		}
		if (recvfrom(sockfd, str, strlen(str), 0, (struct sockaddr* ) &sock_server, &socklen) < 0)
		{
			perror("recvfrom");
			close(sockfd);
			exit(1);
		}

		if (str[0] == 1)
			printf("yes\n");
		else
			printf("no\n");
	}
	close(sockfd);
	return 0;
}