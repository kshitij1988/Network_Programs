#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define MY_SOCK_PATH "./LocalSock"

int main(){

	int sockfd;
	char rbuf[10];
	struct sockaddr_un peer_addr;

	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("CLIENT::: Socket creation failed\n");
		return -1;
	}

	printf("CLIENT::: Socket creation successful\n");

	memset(&peer_addr, 0, sizeof(struct sockaddr_un));
	peer_addr.sun_family = AF_UNIX;
	strncpy(peer_addr.sun_path, MY_SOCK_PATH,
                   sizeof(peer_addr.sun_path) - 1);

	if (connect(sockfd, (struct sockaddr *) &peer_addr,
                   sizeof(struct sockaddr_un)) == -1)
        {
		printf("CLIENT::: Socket connect failed\n");
		close(sockfd);
		return -1;
	}

	printf("CLIENT::: Connection successful\n");

	write(sockfd, "Hi", 3);
	printf("CLIENT::: Written a message to server\n");

	read(sockfd, rbuf, sizeof(rbuf));
	printf("CLIENT::: Server says %s\n", rbuf);

	close(sockfd);
	printf("CLIENT::: Connection terminated\n");

	return 0;
}
