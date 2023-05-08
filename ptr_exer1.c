#include <stdio.h>
#include <string.h>

// 必要なincludeファイルがあれば記述する。
#define ARY_MAX 3 //文字列配列inp_ary1の要素数

// 関数のプロトタイプ宣言を記述

// 第一引数は文字列配列inp_ary1 の要素数,第二引数は文字列配列inp_ary1
char **get_modify_str_ary(int MAX, char hairetu[][8]) {
	char add_str[][2] = {"e", "e", "a"};
	// 関数の中身を実装
	for (int i=0; i<MAX; i++) {
		strcat(hairetu[i], add_str[i]);
	}
	return hairetu;
}

int main(int argc, char *argv[])
{
	char inp_ary1[][8] = {"Appl", "Orang", "Banan"};
	for (int i=0; i<ARY_MAX; i++) {
		printf("Before str ary[%d]=%s\n", i, inp_ary1[i]);
	}
	
	char **modified_ary = get_modify_str_ary(ARY_MAX ,inp_ary1);
	// ここで関数 get_modify_strary を呼ぶ。
	// 関数の中で App1 + e = Apple というように1文字付け足す。 
	// 1文字付け足した文字列配列を戻り値で戻す。
	
	printf("---------------------------------------\n");
	// main関数で、1文字付け足された文字列配列inp_aryをすべて表示する。
	for (int i=0; i<ARY_MAX; i++) {
		printf("After str ary[%d]=%s\n", i, modified_ary[i]);
	}
	
	return 0;
}


