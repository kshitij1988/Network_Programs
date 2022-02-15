#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/select.h>

int main() {
	int sockfd;
	struct sockaddr_in srv_addr;
	char buf[20];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		perror("CLIENT:: Socket creation failed\n");
		return -1;
	}

	printf("CLIENT:: Socket creation successful!\n");

	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(4817);
	srv_addr.sin_addr.s_addr = inet_addr("192.168.1.12");

	if(0 != connect(sockfd, (struct sockaddr *)&srv_addr,sizeof(srv_addr))) {
		perror("CLIENT:: bind failed\n");
		close(sockfd);
		return -1;
	}

	printf("CLIENT:: Connection  successful!\n");
	getchar();

	strncpy(buf, "Hello from client", 18);
	write(sockfd, buf, sizeof(buf));
	printf("CLIENT:: Data written to the server\n");

	memset(buf, '\0', sizeof(buf));
	read(sockfd, buf, sizeof(buf));

	printf("CLIENT:: Data written read from server %s\n" ,buf);
	printf("CLIENT:: Communication completed, closing the connection\n");
	close(sockfd);

	return 0;
}
