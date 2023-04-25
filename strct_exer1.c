#include <stdio.h>

#include <string.h>

#ifndef USER_COM_ENA
	#define USER_COM_ENA
#endif

#include "exer_com.h"

int main(char argv, char *argc[])
{
	ST_EXER	st_val[2] = {
		{'1', '2', "Good morning!"},
		{'3', '4', "Good aftnoon!"}
	};
	ST_EXER *st_val_p[] = {&st_val[0],  &st_val[1]};

	printf("*** Init ****\n");
	
	for(int i=0;i<2;i++){
		printf("struct from=%c\n", st_val_p[i]->from);
		printf("struct to=%c\n", st_val_p[i]->to);
		printf("struct msg=%s\n", st_val_p[i]->msg);
	}
	
	st_val_p[1]->from = 'A';
	st_val_p[1]->to = 'B';
	strcpy(st_val_p[1]->msg, "This is a pen!");

	printf("*** Modify ****\n");

	for(int i=0;i<2;i++){
		printf("struct from=%c\n", st_val[i].from);
		printf("struct to=%c\n", st_val[i].to);
		printf("struct msg=%s\n", st_val[i].msg);
	}

	return 0;
}
