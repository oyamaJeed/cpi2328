#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <signal.h>
#include <errno.h>

#define ECHOMAX 255

void excep(char *errMsg);
void UseIdleTime();
void SIGIOHandler(int signalType);

int sock;

int main(int argc, char *argv[])
{
    struct sockaddr_in echoServerAddr;
    unsigned short echoServPort;
    struct sigaction handler;

    // コマンドライン引数のチェック
    if (argc != 2) {
        excep("Usage: ./server port");
    }

    // サーバーポート番号の取得
    echoServPort = atoi(argv[1]);

    // ソケットの作成
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        excep("Failed to create a socket!");
    }

    // サーバーアドレスの設定
    memset(&echoServerAddr, 0, sizeof(echoServerAddr));
    echoServerAddr.sin_family = AF_INET;
    echoServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    echoServerAddr.sin_port = htons(echoServPort);

    // ソケットとサーバーアドレスのバインド
    if (bind(sock, (struct sockaddr *)&echoServerAddr, sizeof(echoServerAddr)) < 0) {
        excep("bind failed");
    }

    // SIGIOシグナルのハンドラの設定
    handler.sa_handler = SIGIOHandler;

    if (sigfillset(&handler.sa_mask) < 0) {
        excep("sigfillset() failed");
    }

    handler.sa_flags = 0;

    if (sigaction(SIGIO, &handler, 0) < 0) {
        excep("sigaction() failed for SIGIO");
    }

    // ソケットの非同期モード設定
    if (fcntl(sock, F_SETOWN, getpid()) < 0) {
        excep("Unable to set process owner to us");
    }

    if (fcntl(sock, F_SETFL, O_NONBLOCK | FASYNC) < 0) {
        excep("Unable to put client sock into nonblocking/async mode");
    }

    // メインループ
    for (;;) {
        UseIdleTime();
    }
}

// アイドル時間の利用
void UseIdleTime()
{
    printf(".\n");
    sleep(3);
}

// SIGIOシグナルハンドラ
void SIGIOHandler(int signalType)
{
    struct sockaddr_in echoClntAddr;
    unsigned int clntLen;
    int recvMsgSize;
    char echoBuffer[ECHOMAX];

    do {
        clntLen = sizeof(echoClntAddr);

        // クライアントからメッセージの受信
        if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr *)&echoClntAddr, &clntLen)) < 0) {
            if (errno != EWOULDBLOCK) {
                excep("recvfrom() failed");
            }
		}else{
			printf("Handling client %s \n",inet_ntoa(echoClntAddr.sin_addr));
			
			// クライアントにエコーバックするメッセージの送信
			/*if(sendto(sock,echoBuffer,recvMsgSize,0,(struct sockaddr *)&echoClntAddr,sizeof(echoClntAddr))!=recvMsgSize){
				excep("sendto() failed");
			}*/
		}
	}while(recvMsgSize >= 0);
}

// 例外処理関数
void excep(char *errMsg){
	fprintf(stderr,"%s\n",errMsg);
	exit(1);
}
