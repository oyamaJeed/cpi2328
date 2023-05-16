/*** TCP/IP server program ***/
/* Usage: ./k06s <echo port>t */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#define DEFAULT_PORT 7   // デフォルトのポート番号
#define BUFFER_SIZE 128  // 受信用のバッファサイズ

// エラーが発生した場合に呼び出される関数
void excep(const char *errMsg) {
    fprintf(stderr, "%s\n", errMsg);
    exit(1);
}

// クライアントからのデータ受信と送信を行う関数
void clHandler(int clSock) {
    char buffer[BUFFER_SIZE];  // 受信用のバッファ
    ssize_t bytesRead = recv(clSock, buffer, BUFFER_SIZE, 0);  // クライアントからのデータを受信する
    if (bytesRead == -1){
        excep("Failed to receive data.");  // エラーが発生した場合は終了する
    }
    
    printf("Received message from client: %.*s\n", (int)bytesRead, buffer);  // 受信したデータを表示する

    ssize_t bytesSent = send(clSock, buffer, bytesRead, 0);  // 受信したデータをクライアントに送信する
    if (bytesSent == -1) {
        excep("Failed to send data.");  // エラーが発生した場合は終了する
    }
}

int main(int argc, char *argv[]) {
    if (argc > 2) {
        excep("Usage: k06s [<echo port>]");  // 引数の数が不正な場合は終了する
    }
    
    int echoPort = DEFAULT_PORT;  // ポート番号の初期値を設定する

    if (argc == 2) {
        echoPort = atoi(argv[1]);  // 引数で指定されたポート番号を取得する
        if (echoPort <= 0) {
            excep("Invalid echo port number.");  // ポート番号が不正な場合は終了する
        }
    }

    struct sockaddr_in serverAddr;  // サーバーのアドレスを格納する構造体
    struct sockaddr_in clientAddr;  // クライアントのアドレスを格納する構造体
    int serverSessionID;  // サーバーソケットのID
    int clientSessionID;  // クライアントソケットのID
    int clientLength;  // クライアントのアドレスのサイズ

    serverSessionID = socket(AF_INET, SOCK_STREAM, 0);  // サーバーソケットを作成する
    if (serverSessionID == -1) {
        excep("Failed to create a socket.");  // エラーが発生した場合は終了する
    }

    serverAddr.sin_family = AF_INET;  // アドレスファミリーをIPv4に設定する
    
    serverAddr.sin_port = htons(echoPort); // ポート番号をネットワークバイトオーダーに変換して設定する
	serverAddr.sin_addr.s_addr = INADDR_ANY; // サーバーが利用するIPアドレスを設定する（任意のアドレス）
	
	if (bind(serverSessionID, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
		excep("Failed to bind the socket to the address.");  // ソケットをアドレスにバインドする際にエラーが発生した場合は終了する
	}

	if (listen(serverSessionID, 5) == -1) {
		excep("Failed to listen for incoming connections.");  // クライアントからの接続待ち状態の設定にエラーが発生した場合は終了する
	}

	while (1) {
		clientLength = sizeof(clientAddr);
		clientSessionID = accept(serverSessionID, (struct sockaddr *)&clientAddr, (socklen_t *)&clientLength);  // クライアントからの接続を受け入れる
		if (clientSessionID == -1) {
			excep("Failed to accept a client connection.");  // クライアントの接続受け入れにエラーが発生した場合は終了する
		}

		clHandler(clientSessionID);  // クライアントとの通信を処理する関数を呼び出す

		if (shutdown(clientSessionID, 2) == -1) {
			excep("Failed to shutdown the connection.");  // クライアントとの接続をシャットダウンする際にエラーが発生した場合は終了する
		}

		close(clientSessionID);  // クライアントソケットをクローズする
	}

	return 0;
}
