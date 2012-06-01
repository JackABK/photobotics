/*
 * sockets.cpp
 *
 *  Created on: 2012-05-29
 *      Author: steve
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>

#define SERVER_SOCKET_PORT 8888

int initialiseSocket() {
	struct sockaddr_in socketAddress;
	int socketFileDescriptor = 0;
	int returnStatus = 0;

	socketFileDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketFileDescriptor == -1) {
		perror("Could not create a socket\n");
		return 1;
	}

	socketAddress.sin_family = AF_INET;
	socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	socketAddress.sin_port = htons(SERVER_SOCKET_PORT);

	returnStatus = bind(socketFileDescriptor,
			(struct sockaddr *) &socketAddress, sizeof(socketAddress));
	if (returnStatus == 0) {
		fprintf(stderr, "Bind completed!\n");
	} else {
		fprintf(stderr, "Could not bind to address!\n");
		close(socketFileDescriptor);
		return 1;
	}

	returnStatus = listen(socketFileDescriptor, 2);
	if (returnStatus == 0) {
		fprintf(stderr, "listening on socket!\n");
	} else {
		close(socketFileDescriptor);
		return 1;
	}

	return socketFileDescriptor;
}
