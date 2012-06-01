/*
 * photoserver.cpp
 *
 *  Created on: 2012-05-28
 *      Author: steve
 */
#include <stdint.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>

extern int initialisePWM(uint16_t, uint8_t);
extern int initialiseSocket(void);
//extern	int balls(char);
extern int cameraFunction(int);

int main(int argc, char* argv[]) {
	int socketFileDescriptor = 0;
	int n = 0;
	char buffer[256];
	initialisePWM(100, 50);
	socketFileDescriptor = initialiseSocket();

	cameraFunction('c');
	fprintf(stderr, "camera done");
	while (1) {
		struct sockaddr_in clientName = { 0 };
		int simpleChildSocket = 0;
		socklen_t clientNameLength = sizeof(clientName);
		/* wait here */

		simpleChildSocket = accept(socketFileDescriptor,
				(struct sockaddr *) &clientName, &clientNameLength);
		if (simpleChildSocket == -1) {
			fprintf(stderr, "Cannot accept connections!\n");
			close(socketFileDescriptor);
			exit(1);
		}
		fprintf(stderr, "Connection completed!\n");
		/* handle the new connection request
		 */
		/* write out our message to the client */
		bzero(buffer, 256);
		n = read(simpleChildSocket, buffer, 255);
		fprintf(stderr, "this is n %d", n);
		//if (n < 0) error("ERROR reading from socket");
		fprintf(stderr, "Here is the message: %-10.10s", &buffer[1]);
		close(simpleChildSocket);
	}
	close(socketFileDescriptor);

}

