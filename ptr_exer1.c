#include <stdio.h>

#include <string.h>

#define ARY_MAX 3

char **get_modify_str_ary(char *ptr, int pt){
    char add_str[][2] = {"e", "e", "a"};
	
	strcat(ptr, add_str[pt]);
}

int main(int argc, char *argv[])
{
    char inp_ary1[][8] = {"Appl", "Orang", "Banan"};
    for(int i=0; i<ARY_MAX; i++){
        printf("Before str ary[%d]=%s\n", i, inp_ary1[i]);
    }

    for(int i=0; i<ARY_MAX; i++){
        get_modify_str_ary(&inp_ary1[i][0], i);
    }

    printf("------------------------------------\n");

	for(int i=0; i<ARY_MAX; i++){
        printf("After str ary[%d]=%s\n", i, inp_ary1[i]);
    }

    return 0;
}
