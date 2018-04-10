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

	unsigned int id = 0;
    while(1)
	{
		struct sockaddr_in sock_server;
		sock_server.sin_family = AF_INET;
		sock_server.sin_port = htons(port);
		sock_server.sin_addr.s_addr = htonl(INADDR_BROADCAST);

		inet_aton(server, &sock_server.sin_addr); 
		unsigned int socklen = sizeof(sock_server);

	    struct data my_data, my_data_2;
	   
	    while (1)
		{
			printf("=====> String\n");
			my_data.id = id++;
			fgets(my_data.str, 128, stdin);
			my_data.str[strlen(my_data.str) - 1] = '\0';

			if (sendto(sockfd, &my_data, sizeof(my_data), 0, (struct sockaddr* ) &sock_server, socklen) < 0)
			{
				perror("sendto");
    	  		close(sockfd);
       			exit(1);
			}
			if (recvfrom(sockfd, &my_data_2, sizeof(my_data_2), 0, (struct sockaddr* ) &sock_server, &socklen) < 0)
			{
				perror("recvfrom");
				close(sockfd);
				exit(1);
			}

			if (my_data.id == my_data_2.id)
				break;
			else
				printf("wrong\n");
		}
		if (my_data_2.flag == 1)
			printf("yes\n");
		else
			printf("no\n");
	}
	close(sockfd);
	return 0;
}
