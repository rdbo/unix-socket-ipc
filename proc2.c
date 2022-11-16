#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <signal.h>

static int sigcaught = 0;

void
sighandler(int sig)
{
	printf("\nSignal caught: %d\n", sig);
	sigcaught = !sigcaught;
}

int
main()
{
	int sockfd;
	char sockpath[PATH_MAX] = { 0 };
	const char *home_dir;
	struct stat statbuf;
	struct sockaddr_un sockaddr;
	char msgbuf[10] = { 0 };

	signal(SIGINT, sighandler);

	home_dir = getenv("HOME");
	printf("Home Dir: %s\n", home_dir);
	snprintf(sockpath, sizeof(sockpath), "%s/%s", home_dir, ".proc2sock");
	if (stat(sockpath, &statbuf) && !remove(sockpath)) {
		printf("'%s' already exists and cannot be removed", sockpath);
		return 1;
	}

	sockaddr.sun_family = AF_UNIX;
	strncpy(sockaddr.sun_path, sockpath, strlen(sockpath));
	sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	printf("sockfd: %d\n", sockfd);
	bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
	while (!sigcaught) {
		if (recv(sockfd, msgbuf, sizeof(msgbuf) - 1, MSG_DONTWAIT) > 0) {
			printf("Got Message: %s\n", msgbuf);
		}
	}
	close(sockfd);
	remove(sockpath);
	return 0;
}
