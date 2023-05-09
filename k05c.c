/*** TCP/IP client program ***/
/* Usage: ./k05c HostName */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#define PORT 50001 // ポート番号
#define BUFSIZE 256

int main(int argc, char *argv[])
{
	struct sockaddr_in ServerAddr; // サーバーの情報を格納する構造体
	struct hostent *HostInfo; // ホスト情報を格納する構造体
	int SessionID; // サーバーソケットのID
	char buf[BUFSIZE]; // 受信用バッファ
	int recv_size; // 受信したデータのサイズ

	// 引数の数が適切でない場合
	if (argc!=2){
		fprintf(stderr,"Usage: ./k05c HostName!\n");
		exit(1);
	}

	// サーバーソケットを作成する
	if ((SessionID=socket(AF_INET,SOCK_STREAM,0))==-1){
		fprintf(stderr, "Failed to make a socket!\n");
		exit(1);
	}

	// ホスト名からIPアドレスを検索する
	if ((HostInfo=gethostbyname(argv[1]))==NULL){
		fprintf(stderr,"Failed to find host.\n");
		exit(1);
	}

	// サーバー情報構造体を初期化する
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_port=htons(PORT); // ポート番号を設定する
	memcpy((char *)&ServerAddr.sin_addr, (char *)HostInfo->h_addr_list[0], HostInfo->h_length);

	// サーバーに接続する
	if (connect(SessionID, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr))==-1){
		fprintf(stderr,"Failed to connect.\n");
		exit(1);
	}

	// サーバーからのメッセージを受信する
	if ((recv_size=recv(SessionID,buf,sizeof(buf),0))==-1){
		fprintf(stderr,"Failed to receive a message.\n");
		exit(1);
	}
	
	// 受信したメッセージを表示する
	printf("%.*s", recv_size, buf);

	// サーバーとの接続を終了する
	if (shutdown(SessionID,2)==-1){
		fprintf(stderr,"Failed to shutdown.\n");
		exit(1);
	}

	close(SessionID);
	
	return 0;
}
