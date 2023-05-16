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

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        excep("Usage: k06c <server IP> <echo word> [<echo port>]");
    }

    const char *serverIP = argv[1];
    const char *echoWord = argv[2];
    int echoPort = DEFAULT_PORT;

    if (argc == 4) {
        echoPort = atoi(argv[3]);
        if (echoPort <= 0) {
            excep("Invalid echo port number.");
        }
    }

    struct sockaddr_in serverAddr;
    struct hostent *hostInfo;

    int sessionID = socket(AF_INET, SOCK_STREAM, 0);
    if (sessionID == -1) {
        excep("Failed to create a socket.");
    }

    hostInfo = gethostbyname(serverIP);
    if (hostInfo == NULL) {
        excep("Failed to find host.");
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(echoPort);
    memcpy((char *)&serverAddr.sin_addr, (char *)hostInfo->h_addr_list[0], hostInfo->h_length);

    if (connect(sessionID, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        excep("Failed to connect.");
    }

    ssize_t bytesSent = send(sessionID, echoWord, strlen(echoWord), 0);
    if (bytesSent == -1) {
        excep("Failed to send data.");
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead = recv(sessionID, buffer, BUFFER_SIZE, 0);
    if (bytesRead == -1) {
        excep("Failed to receive data.");
    }

    printf("Received: %.*s\n", (int)bytesRead, buffer);

    if (shutdown(sessionID, 2) == -1) {
        excep("Failed to shutdown the connection.");
    }

    close(sessionID);

    return 0;
}
