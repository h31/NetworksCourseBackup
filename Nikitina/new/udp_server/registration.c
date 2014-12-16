#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "registration.h"
#include "writeStruct.h"

void new(char *login, struct Client *c) {
	c->login = login;
	c->numberTest = c->sizeQuestion = c->sizeTrueAnswer = 0;
	c->sizes[0] = c->sizes[1] = c->sizes[2] = 1;
	c->sizes[3] = strlen(login);
}
void writeSizeClient(struct Client *c, char *str) {
	int i = 0;
	int state;
	for (state = 0; state < 4; state++) {
		c->sizes[state] = 0;
	}
	state = 0;
	while (str[i] != '/') {
		if (str[i] == '#') {
			i++;
			state++;
		}
		c->sizes[state]++;
		i++;
	}
	writeClient(c, str);
}
void writeClient(struct Client *c, char *str) {
	int i = 0;
	c->login = (char*) malloc(c->sizes[3] * sizeof(char));
	c->numberTest = c->sizeQuestion = c->sizeTrueAnswer = 0;
	int state;
	int j = 0;
	int add;
	int pow;

	for (state = 0; state < 3; state++) {
		pow = c->sizes[state] - 1;
		if (str[i] == '#')
			i++;
		while (str[i] != '#') {
			add = ((int) str[i] - '0') * power(10, pow);
			switch (state) {
			case 0:
				c->numberTest = c->numberTest + add;
				break;
			case 1:
				c->sizeQuestion = c->sizeQuestion + add;
				break;
			case 2:
				c->sizeTrueAnswer = c->sizeTrueAnswer + add;
				break;
			}
			i++;
			pow--;
		}
	}
	state++;
	i++;
	j = 0;
	while (str[i] != '/') {

		c->login[j] = str[i];

		i++;
		j++;
	}
}
char *writeLastResult(struct Client *c) {
	char string[50];
	strcpy(string, c->login);
	char* num = (char*) malloc(50 * sizeof(char));
	sprintf(num, " your last test is %d. Result %d of %d.", c->numberTest,
			c->sizeQuestion, c->sizeTrueAnswer);
	strcat(string, num);
	free(num);
	strcat(string, "\n");
	return string;
}
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
void newResult(struct Client *c, char number, int testsize, int numberTrueAnswer) {
	c->numberTest = number - '0';
	c->sizeQuestion = testsize;
	c->sizeTrueAnswer = numberTrueAnswer;
}
void freeClient(struct Client *c) {
	free(c->login);

}
int power(int x, int n) {
	int i;
	int a = 1;
	for (i = 0; i < n; i++) {
		a = a * x;
	}
	return a;
}
