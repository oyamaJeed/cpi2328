#include <stdio.h>		/* printf(). fprintf() */
#include <sys/socket.h>	/* socket(). connect(). sendto(). recvfrom() */
#include <arpa/inet.h>	/* sockaddr_in. inet_addr() */
#include <stdlib.h>		/* atoi() */
#include <string.h>		/* memset() */
#include <unistd.h>		/* close() */
#include <errno.h>		/* errno. EINTR */
#include <signal.h>		/* sigaction() */

#define ECHOMAX		255 	/*エコー文字列の最大長 */
#define TIMEOUT_SECS	3		/*再送信までの数*/
#define MAXTRIES		3		/* 最大試行回数 */

int tries=0; /* 送信回数のカウンタ(シグナルハンドラからのアクセス用、 グローバル */

void excep(char *errMsg);			/* エラー処理関数 */
void SIGALRMHandler(int sigType);		/* SIGALRM のハンドラ */

int main(int argc, char *argv[])
{
	int sock;								/* ソケットディスクリプタ */
	struct sockaddr_in echoServAddr;	/* エコーサーバーのアドレス */
	struct sockaddr_in fromAddr;			/* エコー送信元のアドレス */
	unsigned short echoServPort;			/* エコーサーバのポート番号 */
	unsigned int fromSize;				/* recvfrom() のアドレスの入出力サイズ */
	struct sigaction myAction;			/* シグナルハンドラの設定用 */
	char *servIP;							/* サーバのIPアドレス */
	char *echoString;						/* エコーサーバへ送信する文字列 */
	char echoBuffer[ECHOMAX+1];			/* エコー文字列の受信バッファ */
	int echoStringLen;					/* エコー文字列の長さ */
	int respStringLen;					/* 受信データグラムの長さ */
	
	if ((argc < 3) || (argc > 4))			/* 引数の数が正しいか確認 */
	{
		fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n", argv[0]);
		exit(1);
	}
	
	servIP = argv[1];		/* 1つ目の引数:サーバのIPアドレス(ドット 10進表記) */
	echoString = argv[2]; /* 2つ目の引数:エコー文字列 */
	
	if ((echoStringLen = strlen(echoString)) > ECHOMAX)
		excep("Echo word too long");
	
	if (argc == 4)
		echoServPort = atoi(argv[3]); /* 指定のポート番号があれば使用 */
	else
		echoServPort = 7; /* エコーサービスのwell-knownポート番号 */
	
	/* UDP によるベストエフォート型のデータグラムソケットを作成 */
	if ((sock = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		excep("socket() failed");
	
	/* アラームシグナル用シグナルハンドラのセット */
	myAction.sa_handler = SIGALRMHandler;
	if (sigfillset(&myAction.sa_mask) < 0) /* ハンドラ内ではすべてをブロック */
		excep("sigfillset() failed"); 
	myAction.sa_flags = 0;
	
	if (sigaction(SIGALRM, &myAction, 0) < 0)
		excep("sigaction() failed for SIGALRM");

	/* サーバのアドレス構造体を作成 */
	memset(&echoServAddr, 0, sizeof(echoServAddr));		/* 構造体を埋める */
	echoServAddr.sin_family = AF_INET;
	echoServAddr.sin_addr.s_addr = inet_addr (servIP); 	/* サーバのIPアドレス */
	echoServAddr.sin_port = htons (echoServPort);			/* サーバのポート番号 */
	
	/* 文字列をサーバに送信 */
	if (sendto(sock, echoString, echoStringLen, 0, (struct sockaddr *) &echoServAddr, sizeof (echoServAddr)) != echoStringLen)
		excep("sendto() sent a different number of bytes than expected");
	
	/* 応答を受信 */
	
	fromSize = sizeof(fromAddr);
	alarm (TIMEOUT_SECS);		/* タイムアウト時間を設定 */ 
	while ((respStringLen = recvfrom (sock, echoBuffer, ECHOMAX, 0, (struct sockaddr *)&fromAddr, &fromSize)) < 0)
		if (errno == EINTR)	/* アラームの終了 */
		{
			if (tries < MAXTRIES) /* triesはシグナルハンドラ内でインクリメント */
			{
				printf("timed out, %d more tries...\n", MAXTRIES - tries);
				if (sendto(sock, echoString, echoStringLen, 0, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) != echoStringLen)
					excep("sendto() failed");
				alarm(TIMEOUT_SECS);
			}
		else
			excep("No Response");
	}
	else
		excep("recvfrom() failed");
	
	/* recvfrom()が何かを受信したら、タイムアウトをキャンセル */	
	alarm(0);
	
	/* 受信データをNULL文字で終端させる */
	echoBuffer[respStringLen] = '\0';
	printf("Received: %s\n", echoBuffer);	/* 受信データを表示 */
	
	close(sock);
	exit(0);
}

void excep(char *errMsg) {
    perror(errMsg);
    exit(1);
}

void SIGALRMHandler(int sigType)	/* SIGALAMハンドラ */
{
	tries += 1;
}
