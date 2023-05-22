#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 255

void excep(char *errMsg) {
    fprintf(stderr, "Error: %s\n", errMsg);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        excep("Usage: ./k07s <port>");
    }

    int port = atoi(argv[1]);

    // ソケットの作成
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        excep("Failed to create socket");
    }

    // サーバの情報設定
    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    // ソケットとポートのバインド
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        excep("Failed to bind socket");
    }

    while (1) {
        // クライアントからのメッセージを受信
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, sizeof(buffer));
        socklen_t client_len = sizeof(client_addr);
        int recv_len = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&client_addr, &client_len);
        if (recv_len < 0) {
            excep("Failed to receive message");
        }

        printf("HANDLING CLIENT: %s\n", inet_ntoa(client_addr.sin_addr));
        printf("RECEIVED MESSAGE: %s\n", buffer);

        // 受け取ったメッセージをクライアントに送信
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
            excep("Failed to send message");
        }
    }

    close(sockfd);
    return 0;
}
