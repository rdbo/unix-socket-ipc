#include <sys/un.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int
main()
{
	const char *home_dir;
	char sockpath[PATH_MAX] = { 0 };
	char msg[10] = "Hello!\n";
	int sockfd;
	struct sockaddr_un sockaddr;
	int i;

	home_dir = getenv("HOME");
	snprintf(sockpath, sizeof(sockpath), "%s/%s", home_dir, ".proc2sock");
	printf("proc2 sockpath: %s\n", sockpath);

	sockaddr.sun_family = AF_UNIX;
	strncpy(sockaddr.sun_path, sockpath, strlen(sockpath));
	sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	connect(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
	printf("proc2 sockfd: %d\n", sockfd);

	for (i = 0; i < 10; ++i) {
		send(sockfd, msg, sizeof(msg) - 1, 0);
	}
	return 0;	
}
