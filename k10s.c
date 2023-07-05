#include <stdio.h>			/* printf(), fprintf() */
#include <sys/socket.h>		/* socket(), bind().connect() */
#include <arpa/inet.h>		/* sockaddr_ind inet_ntoa()  */
#include <stdlib.h>			/* atoi() */
#include <string.h>			/* menset() */
#include <unistd.h>			/* close() */
#include <pthread.h>			/* POSIXスレッドに必要 */

#define MAXPENDING 5			/* 未処理の接続要求の最大値 */
#define BUFFER_SIZE 255  // 受信用のバッファサイズ

void *ThreadMain(void *arg);	/* スレッドのメインプログラム */
int createTCPSvSock(unsigned short port);
int acceptTCPConn(int svSock);
void clHandler(int clSock);
void excep(char *errMsg);

/* クライアントスレッドに渡す引数の構造体 */
struct ThreadArgs
{
	int clntSock;				/* クライアントのソケットディスクリプタ */
};


int main(int argc, char *argv[])
{
	int servSock;						/* サーバのソケットディスクリプタ */
	int clntSock;						/* クライアントのソケットディスクリプタ */
	unsigned short echoServPort;		/* サーバのポート番号 */
	pthread_t threadID;				/* pthread_create() が返すスレッドID */
	struct ThreadArgs *threadArgs; 	/* スレッドの引数構造体へのポインタ */
	
	if (argc != 2)		/* 引数の数が正しいか確認 */
	{
		fprintf(stderr, "Usage: %s <SERVER PORT>\n", argv[0]);
		exit(1);
	}
	
	echoServPort = atoi(argv[1]); /* 1つ目の引数:ローカルポート */
	
	servSock = createTCPSvSock(echoServPort);
	
	for (;;) /* 処理を繰り返し実行 */
	{
		clntSock = acceptTCPConn(servSock);
		
		/* クライアント引数用にメモリを新しく確保 */
		if ((threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs))) == NULL)
			excep("malloc() failed");
		threadArgs -> clntSock = clntSock;
		
		/* クライアントスレッドを生成 */
		if (pthread_create(&threadID, NULL, ThreadMain, (void *) threadArgs) != 0)
			excep("pthread_create() failed");
		printf("with thread %ld\n", (long int) threadID);
	}
	/* この部分には到達しない */
}

void *ThreadMain(void *threadArgs)
{
	int clntSock; /* クライアント接続用ソケットディスクリプタ */
	
	/* 戻り、スレッドのリソースを割り当て解除 */
	pthread_detach(pthread_self());
	
	/* ソケットのファイルディスクリプタを引数から取り出す */
	clntSock = ((struct ThreadArgs *) threadArgs) -> clntSock;
	free(threadArgs);
	
	clHandler(clntSock);
	
	return (NULL);
}


int createTCPSvSock (unsigned short port)
{
	int sock;								/* 作成するソケット */
	struct sockaddr_in echoServAddr;	/* ローカルアドレス */

	/* 着信接続用のソケットを作成 */
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		excep("socket() failed");

	/* ローカルのアドレス構造体を作成 */
	memset(&echoServAddr, 0, sizeof(echoServAddr));	/* 構造体を0で埋める */
	echoServAddr.sin_family = AF_INET; 					/* インターネットアドレスファミリ */
	echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* ワイルドカードを使用 */
	echoServAddr.sin_port = htons (port);				/* ローカルポート */

	/* ローカルアドレスへバインド */
	if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
		excep("bind() failed");

	/* 「接続要求をリスン中」というマークをソケットに付ける */
	if (listen(sock, MAXPENDING) < 0)
		excep("listen() failed");

	return sock;
}


int acceptTCPConn(int servSock)
{
	int clntSock;							/* クライアントのソケットディスクリプタ */
	struct sockaddr_in echoClntAddr; 	/* クライアントのアドレス */
	unsigned int clntLen;					/* クライアントのアドレス構造体の長さ */
	
	/* 入出力パラメータのサイズをセット */
	clntLen = sizeof(echoClntAddr);
	
	/* クライアントからの接続要求を待つ */
	if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
		excep("accept() failed");
	/* clntSockはクライアントに接続済み */
	
	printf ("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

	return clntSock;
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


void excep(char *errMsg) {
    perror(errMsg);
    exit(1);
}
