#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 256

void excep(char *errMsg) {
    fprintf(stderr, "Error: %s\n", errMsg);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        excep("Usage: ./k07c <server IP> <echo word> [<echo port>]");
    }

    char *serverIP = argv[1];
    char *echoWord = argv[2];
    int echoPort = 7; // デフォルトのポート番号

    if (argc >= 4) {
        echoPort = atoi(argv[3]);
    }

    // ソケットの作成
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        excep("Failed to create socket");
   }
   
    // サーバの情報設定
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(serverIP);
    server_addr.sin_port = htons(echoPort);

    // エコーワードをサーバに送信
    if (sendto(sockfd, echoWord, strlen(echoWord), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        excep("Failed to send message");
    }

    // サーバからのメッセージを受信
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    socklen_t server_len = sizeof(server_addr);
    int recv_len = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&server_addr, &server_len);
    if (recv_len < 0) {
        excep("Failed to receive message");
    }

    // IPアドレスをチェック
    if (strcmp(inet_ntoa(server_addr.sin_addr), serverIP) != 0) {
        excep("Received message from an unexpected server");
    }

    printf("ECHO-BACK MESSAGE: %s\n", buffer);

    close(sockfd);
    return 0;
}
