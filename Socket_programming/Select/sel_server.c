#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define MAX_CLIENTS 50

int main() {
	int sockfd, sessfd, maxfd, ret, cli_size, cnt;
	struct sockaddr_in srv_addr, cli_addr;
	fd_set rfds,itr_rdset;
	char buff[20];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		perror("SERVER:: Socket creation failed\n");
		return -1;
	}

	printf("SERVER:: Socket creation successful!\n");

	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(4817);
	srv_addr.sin_addr.s_addr = inet_addr("192.168.1.12");

	if(0 != bind(sockfd, (struct sockaddr *)&srv_addr,sizeof(srv_addr))) {
		perror("SERVER:: bind failed\n");
		close(sockfd);
		return -1;
	}

	printf("SERVER:: Socket bind successful!\n");

	if(0 != listen(sockfd, MAX_CLIENTS)) {
		perror("SERVER:: listening failed!");
		close(sockfd);
		return -1;
	}

	printf("SERVER:: Listening for clients!\n");
	printf("---------------------------------\n");

	FD_ZERO(&rfds);
	FD_SET(sockfd, &rfds);

	maxfd = sockfd;
	cli_size = sizeof(cli_addr);
	while(1) {
		itr_rdset = rfds;
		
		ret = select(maxfd+1, &itr_rdset, NULL, NULL, NULL);
		if(ret < 0) {
			perror("SERVER:: Select failed\n");
			continue;
		} else if(ret > 0) {
			if(1 == FD_ISSET(sockfd, &itr_rdset)) {
				printf("SERVER:: New connection request\n");

				sessfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_size);
				if(sessfd == -1) {
					perror("SERVER::: Accepting failed\n");
					close(sockfd);
					return -1;
				}

				printf("SERVER:: SELECT:: Client request accepted from %s:%d\n"
						,inet_ntoa(cli_addr.sin_addr)
						,ntohs(cli_addr.sin_port));

				FD_SET(sessfd, &rfds);
				printf("SERVER:: SELECT:: sessfd added to rfds\n");
				FD_CLR(sockfd, &itr_rdset);
				printf("SERVER:: SELECT:: sockfd removed from itr_rdset\n");
				if(maxfd < sessfd)
					maxfd = sessfd;
			} else {
				for(cnt=0; cnt<maxfd+1; ++cnt){
					if(1 == FD_ISSET(cnt, &itr_rdset)){
						ret = read(cnt, buff, sizeof(buff));
						if (ret < 0){
							perror("Server read failure\n");
							continue;
						} else if (ret == 0){
							printf("SERVER:: SELECT:: client %d closing the connection!\n", cnt);
						        close(cnt);
					       		FD_CLR(cnt, &rfds);
					 		printf("SERVER:: SELECT:: client %d removed from rfds\n", cnt);		
						} else {
							printf("SERVER:: SELECT:: %dth fd ready with data\n", cnt); 
							printf("SERVER:: READ:: Data read from %dth fd is %s\n", cnt, buff);
							write(cnt, "Hello from Server!", 19);
						       	printf("SERVER:: WRITE:: Data written to client\n");
						}
					}
				}

			}

		} else if(ret == 0) {
			perror("SERVER:: Timeout Expired\n");
			continue;
		}		
	}
	return 0;
}
