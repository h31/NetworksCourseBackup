#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "writeStruct.h"

int readTrueAnswer( char *str) {
	int i,trueAnswer;
	int state=0;
		for (i = 0; i < strlen(str); i++) {
			if (str[i] == '#')
				state++;
			if (str[i] == '!') {
				trueAnswer = state - 1;
				break;
			}
}
	return trueAnswer;
}
int sizeFile(char *str) {
	FILE *file;
	int i = 0;
	char string[50];
	file = fopen(str, "r");
	while (fgets(string, sizeof(string), file))
		i++;
	fclose(file);
	return i;
}
