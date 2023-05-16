#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#define DEFAULT_PORT 7
#define BUFFER_SIZE 128

void excep(const char *errMsg) {
    fprintf(stderr, "%s\n", errMsg);
    exit(1);
}

void clHandler(int clSock) {
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead = recv(clSock, buffer, BUFFER_SIZE, 0);
    if (bytesRead == -1){
		excep("Failed to receive data.");
	}
	
	printf("Received message from client: %.*s\n", (int)bytesRead, buffer);

	ssize_t bytesSent = send(clSock, buffer, bytesRead, 0);
	if (bytesSent == -1) {
		excep("Failed to send data.");
	}
}

int main(int argc, char *argv[]) {
	if (argc > 2) {
		excep("Usage: k06s [<echo port>]");
	}
	
	int echoPort = DEFAULT_PORT;

	if (argc == 2) {
		echoPort = atoi(argv[1]);
		if (echoPort <= 0) {
			excep("Invalid echo port number.");
		}
	}

	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;
	int serverSessionID;
	int clientSessionID;
	int clientLength;

	serverSessionID = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSessionID == -1) {
		excep("Failed to create a socket.");
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(echoPort);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(serverSessionID, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
		excep("Failed to bind the socket to the address.");
	}

	if (listen(serverSessionID, 5) == -1) {
		excep("Failed to listen for incoming connections.");
	}

	while (1) {
		clientLength = sizeof(clientAddr);
		clientSessionID = accept(serverSessionID, (struct sockaddr *)&clientAddr, (socklen_t *)&clientLength);
		if (clientSessionID == -1) {
			excep("Failed to accept a client connection.");
		}

		clHandler(clientSessionID);

		if (shutdown(clientSessionID, 2) == -1) {
			excep("Failed to shutdown the connection.");
		}

		close(clientSessionID);
	}

	return 0;
}
