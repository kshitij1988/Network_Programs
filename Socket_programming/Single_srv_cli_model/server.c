#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include <sys/un.h>

#define MY_SOCK_PATH "./LocalSock"
#define LISTEN_BACKLOG 50

int main(){

	int sockfd, newfd;
	struct sockaddr_un my_addr, peer_addr;
	socklen_t peer_addr_size;
	char buf[10];
	const char *wbuf = "Bye";

	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("SEVER::: Socket creation failed\n");
		return -1;
	}

	printf("SERVER::: Socket creation successful\n");

	memset(&my_addr, 0, sizeof(struct sockaddr_un));
	my_addr.sun_family = AF_UNIX;
	strncpy(my_addr.sun_path, MY_SOCK_PATH,
                   sizeof(my_addr.sun_path) - 1);

	if (bind(sockfd, (struct sockaddr *) &my_addr,
                   sizeof(struct sockaddr_un)) == -1)
        {
		printf("SEVER::: Socket bind failed\n");
		close(sockfd);
		return -1;
	}

	printf("SERVER::: Binding successful\n");

	if (listen(sockfd, LISTEN_BACKLOG) == -1) {
		printf("SEVER::: Listening failed\n");
		close(sockfd);
		return -1;
	}

	printf("SERVER::: Listening successful\n");

	peer_addr_size = sizeof(struct sockaddr_un);
        newfd = accept(sockfd, (struct sockaddr *) &peer_addr,
                        &peer_addr_size);
        if (newfd == -1) {
		printf("SEVER::: Accepting failed\n");
		close(sockfd);
		return -1;
	}

	printf("SERVER::: Accepting successful\n");

	read(newfd, buf, sizeof(buf));
	printf("SERVER::: Received %s from client\n" ,buf);

	write(newfd, wbuf, sizeof(wbuf));
	printf("SERVER::: sent message to client\n");

	close(sockfd);
	unlink(MY_SOCK_PATH);

	return 0;
}
