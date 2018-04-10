#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <math.h>

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
	
	while(1)
	{
		//sprintf(data, "%s", "broadcast+");
		printf("=====> String\n");
		fgets(data, 128, stdin);
		data[strlen(data) - 1] = '\0';
		unsigned int srvrlen = sizeof(server);
		int flag = 0; //ответ сревера, готов ли принять соединение 1 да, 0 нет
		int ret = 0;
        if ((ret = sendto(sock, data, sizeof(data), 0, (struct sockaddr*) &server, srvrlen)) < 0)
            perror("sendto() failed: ");

        if ((ret = recvfrom(sock, &flag, sizeof(int), 0, (struct sockaddr*) &server, &srvrlen)) < 0)
			perror("recvfrom() failed: ");
		printf("%d\n", flag);
		while (flag)
		{
			int s_tcp = 0;
			//создание TCP сокета
			if ((s_tcp = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
			{
				perror("socket");
				exit(1);
			}
			bzero(&server, sizeof(server));
			server.sin_family = AF_INET;
			server.sin_port = htons(port);
			if(inet_aton("127.0.0.1", &server.sin_addr) == 0)
			{
				printf("Invalid IP address\n");
				exit(1);
			}
			if(connect(s_tcp, (struct sockaddr *) &server, sizeof(server)) < 0)
			{
				perror(NULL);
				exit(1);
			}
			int len = 0, value = 0;
			printf("String => ");
			fflush(stdin);
			fgets(data, 128, stdin);
			if( (len = write(s_tcp, data, strlen(data))) < 0)
			{
				perror("Can\'t write\n");
				exit(1);
			}
			printf("%d\n", len);
			if ((len = read(s_tcp, &value, sizeof(int))) < 0)
			{
				perror("Can\'t read\n");
				exit(1);
			}
			if (value)
				printf("YES\n");
			else
				printf("NO\n");
			close(s_tcp);		
		}
	}
	close(sock);
	return 0;
}
