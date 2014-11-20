#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "write.h"
#include <string.h>
int toInt(char *buffer){
		int i=strlen(buffer)-2;
		int pow=0;
		int size=0;
		while(i>=0){
					size = size + ((int) buffer[i] - '0') * power(10, pow);
						pow++;i--;
				}
		return size;
}
int writeListTest(int *listOfTest,char *buffer,char *string){
	int j=0;
	int pow=0;
	int i=strlen(buffer)-1;
	strcpy(string,"List of test: ");
	char s[3];
		while (buffer[i] != '/') {
			listOfTest[j] =0;
			while(buffer[i] != '#'){
				listOfTest[j] = listOfTest[j]
							+ ((int) buffer[i] - '0') * power(10, pow);
					i--;pow++;
			}
			i--;j++;
			pow=0;
		}
		int size=j;
		for(i=0;i<size;i++){
		sprintf(s, "%d ", listOfTest[i]);
		strcat(string, s);
	}
		return size;
}
int power(int x, int n) {
	int i;
	int a = 1;
	for (i = 0; i < n; i++) {
		a = a * x;
	}
	return a;
}
void writeStruct(struct Line *x, char *str) {
	int i;
	x->question = (char*) malloc(x->sizeQuest * sizeof(char));
	x->answer = (char**) malloc(N * sizeof(char*));
	for (i = 0; i < N; i++) {
		x->answer[i] = (char*) malloc(x->sizeAnswer[i] * sizeof(char));
	}
	x->number = 0;
	int state = 0;
	int j = 0;
	i = 0;
	while (str[i] != '#') {
		x->number = x->number
				+ ((int) str[i] - '0') * power(10, x->sizeNumber - 1 - i);
		i++;
	}
	while (str[i] != '/') {
		if (str[i] == '#') {
			state++;
			i++;
			j = 0;
		}
		if (str[i] == '!')
			i++;
		switch (state) {
		case 1:
			x->question[j] = str[i];
			break;
		default:
			x->answer[state - 2][j] = str[i];
			break;
		}
		i++;
		j++;
	}
}
void writeSize(struct Line *x, char *str) {
	x->sizeQuest = 0;
	x->sizeNumber = 0;
	x->sizeAnswer = (int*) malloc(N * sizeof(int));
	int i;
	for (i = 0; i < N; i++) {
		x->sizeAnswer[i] = 0;
	}
	i = 0;
	int state = 0;
	while (str[i] != '/') {
		if (str[i] == '#') {
			state++;
			i++;
		}
		if (str[i] == '!')
			i++;
		switch (state) {
		case 0:
			x->sizeNumber++;
			break;
		case 1:
			x->sizeQuest++;
			break;
		default:
			x->sizeAnswer[state - 2]++;
			break;
		}
		i++;
	}
	writeStruct(x, str);
}
void freeLine(struct Line *x) {
	int i;
	free(x->question);
	free(x->sizeAnswer);
	for (i = 0; i < N; i++) {
		free(x->answer[i]);
	}
	free(x->answer);

}
void writeToClient(struct Line *x,char *str,char *stringOut) {

	int i,j,k;
	for(i=0;i<x->sizeQuest;i++)
	str[i]=x->question[i];
	j=i;
	for (k = 0; k < 4; k++) {
		str[j++]=k+1+'0';
		str[j++]=')';
		for(i=0;i<x->sizeAnswer[k];i++)
			str[j++]=x->answer[k][i];
		str[j++]=' ';
	}
	str[j++]='\n';
	for(i=0;str[i]!='\n';i++)
		stringOut[i]=str[i];
}
int sizeStr(struct Line *x){
	int i;
	int number=x->sizeQuest+13;
	for (i = 0; i< N; i++)
		number=number+x->sizeAnswer[i];
	return number;
}

