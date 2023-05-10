/*** TCP/IP server program ***/
/* Usage: ./server HostName */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>


#define PORT 50001 // ポート番号


char buf[]="Sample message.\n"; // クライアントに送信するメッセージ


int main(int argc, char *argv[])
{
	struct sockaddr_in ServerAddr; // サーバーの情報を格納する構造体
	struct sockaddr_in ClientAddr; // クライアントの情報を格納する構造体
	struct hostent *HostInfo; // ホスト情報を格納する構造体
	int ServerSessionID; // サーバーソケットのID
	int ClientSessionID; // クライアントソケットのID
	int clientlength; // クライアント情報構造体の長さ

	// 引数の数が適切でない場合
	if (argc!=2){
		fprintf(stderr,"Usage: ./server HostName!\n");
		exit(1);
	}

	// サーバーソケットを作成する
	if ((ServerSessionID=socket(AF_INET,SOCK_STREAM,0))==-1){
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
	
	// ソケットに名前を付ける
	if (bind(ServerSessionID,(struct sockaddr *)&ServerAddr, sizeof(ServerAddr))==-1){
		fprintf(stderr,"Failed to assign a socketname.\n");
		exit(1);
	}

	// クライアントからの接続要求を待機する
	if (listen(ServerSessionID,5)==-1){
		fprintf(stderr,"Failed to listen.\n");
		exit(1);
	}

	while(1){
		clientlength=sizeof(ClientAddr);
		// クライアントからの接続要求を受け入れる
		if ((ClientSessionID=accept(ServerSessionID, (struct sockaddr *)&ClientAddr, (socklen_t *)&clientlength))==-1){
			fprintf(stderr,"Failed to accept.\n");
			exit(1);
		}

		// クライアントにメッセージを送信する
		if (send(ClientSessionID,buf,sizeof(buf),0)==-1){
			fprintf(stderr,"Failed to send a message.\n");
			exit(1);
		}
		
		// クライアントとの接続を終了する
		if (shutdown(ClientSessionID,2)==-1){
			fprintf(stderr,"Failed to shutdown.\n");
			exit(1);
		}
	close(ClientSessionID);
	}
}
