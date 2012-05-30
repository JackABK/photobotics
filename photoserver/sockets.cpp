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



int initialiseSocket()
{
	sockaddr_in	socketAddress = {0};
	int socketFileDescriptor = 0;


	socketFileDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketFileDescriptor == -1)
	{
		perror("Could not create a socket\n");
		return 1;
	}

	socketAddress.sin_family = AF_INET;
	socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	socketAddress.sin_port = htons(SERVER_SOCKET_PORT);

	if(!(bind(socketFileDescriptor, (struct sockaddr *)&socketAddress, sizeof(sockaddr_in))))
	{
		perror("Could not bind socket\n");
		close(socketFileDescriptor);
		return 1;
	}

	return 0;
}
