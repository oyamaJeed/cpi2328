/*** TCP/IP client program ***/
/* Usage: k06c <server IP> <echo word> [<echo port>] */




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
	// エラーメッセージを標準エラー出力に出力し、プログラムを終了する関数
    if (argc < 3 || argc > 4) {
        excep("Usage: k06c <server IP> <echo word> [<echo port>]");
    }

    const char *serverIP = argv[1];  // サーバーの IP アドレスをコマンドライン引数から取得
    const char *echoWord = argv[2];  // エコーバックする文字列をコマンドライン引数から取得
    int echoPort = DEFAULT_PORT;     // サーバーのポート番号のデフォルト値

    if (argc == 4) {
        echoPort = atoi(argv[3]);   // サーバーのポート番号をコマンドライン引数から取得
        if (echoPort <= 0) {
            excep("Invalid echo port number.");
        }
    }

    struct sockaddr_in serverAddr;  // サーバーのアドレスを表す構造体
    struct hostent *hostInfo;       // サーバーの IP アドレスを解決するための構造体

	// ソケットを生成する
    int sessionID = socket(AF_INET, SOCK_STREAM, 0); 
    if (sessionID == -1) {
        excep("Failed to create a socket.");
    }

    // サーバのIPアドレスを取得する
    hostInfo = gethostbyname(serverIP);
    if (hostInfo == NULL) {
        excep("Failed to find host.");
    }

	// サーバの情報を設定する
    serverAddr.sin_family = AF_INET;  // アドレスファミリーを IPv4 に設定
    serverAddr.sin_port = htons(echoPort);  // サーバーのポート番号を設定
    memcpy((char *)&serverAddr.sin_addr, (char *)hostInfo->h_addr_list[0], hostInfo->h_length);  // サーバーの IP アドレスを設定

    // サーバに接続する
    if (connect(sessionID, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        excep("Failed to connect.");
    }

    // データを送信する
    ssize_t bytesSent = send(sessionID, echoWord, strlen(echoWord), 0);
    if (bytesSent == -1) {
        excep("Failed to send data.");
    }

    // データを受信する
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead = recv(sessionID, buffer, BUFFER_SIZE, 0);
    if (bytesRead == -1) {
        excep("Failed to receive data.");
    }

    // 受信したデータを表示する
    printf("Received: %.*s\n", (int)bytesRead, buffer);

    // 接続を終了する
    if (shutdown(sessionID, 2) == -1) {
        excep("Failed to shutdown the connection.");
    }

    // ソケットを閉じる
    close(sessionID);

    return 0;
}
