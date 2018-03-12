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

    struct sockaddr_in sock_client;
    unsigned int socklen = sizeof(sock_client);
   	bind(sockfd, (const struct sockaddr*) &sock_server, sizeof(sock_server));

   	while(1)
   	{
   		recvfrom(sockfd, str, sizeof(str), 0, (struct sockaddr*) &sock_client, &socklen);
   		char flag = 1;

   		printf("%s\n", str);
   		if (pal(str) == 1)
   			printf("yes\n");
   		else
   			printf("no\n");
   		memset(str, '\0', strlen(str));
   		sendto(sockfd, str, sizeof(str), 0, (struct sockaddr*) &sock_client, socklen);
   	}
	
    close(sockfd);
	return 0;
}