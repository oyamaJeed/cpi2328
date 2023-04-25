#include <stdio.h>

#include <string.h>

#include "exer_com.h"

int main(int argv, char *argc[])
{
	UNI_EXER	uni_val = {
        .exer = {
            .from = 'C',
            .to = 'D',
            .msg = "Good evening!"
        }
    };
    
	UNI_EXER	*uni_val_p = &uni_val;

	printf("*** Init ****\n");

	printf("uni st from=%c\n", uni_val_p->exer.from);
	printf("uni st to=%c\n", uni_val_p->exer.to);
	printf("uni st msg=%s\n", uni_val_p->exer.msg);
	
	uni_val_p->exer.from = 'E';
	uni_val_p->exer.to = 'F';
	strcpy(uni_val_p->exer.msg, "This is an apple!");
	
	printf("*** Modify ****\n");

	printf("uni st from=%c\n", uni_val.exer.from);
	printf("uni st to=%c\n", uni_val.exer.to);
	printf("uni st msg=%s\n", uni_val.exer.msg);
	
	return 0;
}
