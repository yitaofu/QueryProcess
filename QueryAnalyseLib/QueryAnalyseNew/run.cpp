#include "ts_chinese_translate.h"
#include <stdio.h>

#define MAX_STRING 4096

int main(){
	char *minWs = (char *)malloc(sizeof(char) * MAX_STRING);
	int ts_flag = 0;
	char *dic_file = "data/ts_convert.txt";
	int i = 1;
	TSinit(dic_file);
	while (1) {
	        char *flag = fgets(minWs, MAX_STRING, stdin);
	        char *wordlist[MAX_STRING];
		int back = translate(wordlist, minWs, ts_flag);
        	for (i = 0; i < back; i++) printf("%s", wordlist[i]);
		printf("\n");
		for (i = 0; i < back; i++) free(wordlist[i]);
	}
	freeTS();
	free(minWs);
        free(dic_file);
	return 0;
}
