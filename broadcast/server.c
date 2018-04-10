#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <errno.h>
#include <unistd.h>

#define server "255.255.255.255"

struct data
{
	unsigned int id;
	int flag;
	char str[128];
};

int pal(char s[])
{
    for (size_t i = 0; i < strlen(s) / 2; i++)
        if (s[i] != s[strlen(s) - i - 1])
        return 0;
    return 1;
}

int main(int argc, char *argv[])
{
	const int port = strtol(argv[1], NULL, 10);
	printf("%s:%d\n", server, port);

	int sockfd;
	if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
        perror("socket"); 
        exit(1);
    }

    int broadcast = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
    {
    	perror("SO_BROADCAST");
    	close(sockfd);
    	exit(1);
    }

    struct sockaddr_in sock_server;
    sock_server.sin_family = AF_INET;
    sock_server.sin_port = htons(port);
    sock_server.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    struct sockaddr_in sock_client;
    unsigned int socklen = sizeof(sock_client);
    bind(sockfd, (const struct sockaddr*) &sock_server, sizeof(sock_server));

    struct data my_data;
    while(1)
	{
        if (recvfrom(sockfd, &my_data, sizeof(my_data), 0, (struct sockaddr*) &sock_client, &socklen) < 0)
        {
            perror("recvfrom");
            close(sockfd);
            exit(1);
        }

        printf("%s\n", my_data.str);

        if (pal(my_data.str) == 1)
            printf("yes\n");
        else
            printf("no\n");

        my_data.flag = pal(my_data.str);        
        
		if (sendto(sockfd, &my_data, sizeof(my_data), 0, (struct sockaddr*) &sock_client, socklen) < 0)
        {
            perror("sendto");
            close(sockfd);
            exit(1);
        }
    }
    close(sockfd);
    return 0;
}
