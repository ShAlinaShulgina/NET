#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>

int pal(char s[])
{
	printf("%s%lu\n", s, strlen(s));
	for (size_t i = 0; i < (strlen(s) - 1) / 2; i++)
		if (s[i] != s[strlen(s) - i - 2])
			return 0;
	return 1;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Using ./client <port>");
		exit(1);
	}
	char data[128];
	const int port = strtol(argv[1], NULL, 10);
	struct sockaddr_in server = {AF_INET, htons(port)};
	int sock = 0;
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		perror("socket");
		exit(1);
	}
	inet_aton("255.255.255.255", &server.sin_addr);

    unsigned broadcast = 1;
    if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
		perror("setsockopt broadcast failed: ");
	struct sockaddr_in client;

	bind(sock, (const struct sockaddr*) &server, sizeof(server));
	while(1)
	{
		unsigned int srvrlen = sizeof(server);
		int ret = 0;
		if ((ret = recvfrom(sock, data, sizeof(data), 0, (struct sockaddr*) &client, &srvrlen)) < 0)
		{
			perror("recvfrom");
			exit(1);
		}
		int flag = 1;
		if ((ret = sendto(sock, &flag, sizeof(int), 0, (struct sockaddr*) &client, srvrlen)) < 0)
            perror("sendto() failed: ");
			
			
		int s_tcp = 0;
		//создание TCP сокета
		if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		{
			perror("socket");
			exit(1);
		}
		bzero(&server, sizeof(server));
		server.sin_family = AF_INET;
		server.sin_port = htons(port);
		bind(sock, (const struct sockaddr*) &server, sizeof(server));
		if(listen(sock, 5) < 0)
		{
			perror(NULL);
			exit(1);
		}
		while(1)
		{
			unsigned int clntlen = sizeof(client);
			if((s_tcp = accept(sock, (struct sockaddr *) &client, &clntlen)) < 0)
			{
				perror(NULL);
				exit(1);
			}
			int len = 0;
			if ((len = read(s_tcp, data, 128)) > 0)
			{
				flag = pal(data);
				if( (len = write(s_tcp, &flag, sizeof(int))) < 0)
				{
					perror("Can\'t write\n");
					exit(1);
				}
			}
			else
			{
				perror("read");
				exit(1);
			}
			memset(data, '\0', 128);
		}
		close(s_tcp);
	}
	close(sock);
	return 0;
}
