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

int pal(char s[])
{
	for (size_t i = 0; i < strlen(s) / 2; i++)
		if (s[i] != s[strlen(s) - i - 1])
			return 0;
	return 1;
}

int main()
{
	
	int sockfd;

	if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		perror("error"); 
		exit(1);
	}

    struct sockaddr_in sock_server;

	//domain указывающий семейство протоколов создаваемого сокета    AF_INET для сетевого протокола IPv4
	sock_server.sin_family = AF_INET;
	//Функция htons осуществляет перевод целого короткого числа из порядка байт, принятого на компьютере, в сетевой порядок байт.
	sock_server.sin_port = htons(port);
	//Функция htonl осуществляет перевод целого длинного числа из порядка байт, принятого на компьютере, в сетевой порядок байт.
	sock_server.sin_addr.s_addr = htonl(INADDR_ANY);

	struct sockaddr_in sock_client;
	unsigned int socklen = sizeof(sock_client);
	if (bind(sockfd, (const struct sockaddr*) &sock_server, sizeof(sock_server)) < 0)
	{
		perror("bind");
		close(sockfd);
		exit(1);
	}

	while(1)
	{
		if (recvfrom(sockfd, str, sizeof(str), 0, (struct sockaddr*) &sock_client, &socklen) < 0)
		{
			perror("recvfrom");
			close(sockfd);
			exit(1);
		}

		printf("%s\n", str);
		if (pal(str) == 1)
			printf("yes\n");
		else
			printf("no\n");
		str[0] = pal(str);
		printf("%d\n", (int)str[0]);		
		
		if (sendto(sockfd, str, sizeof(str), 0, (struct sockaddr*) &sock_client, socklen) < 0)
		{
			perror("sendto");
			close(sockfd);
			exit(1);
		}
	}
	close(sockfd);
	return 0;
}
