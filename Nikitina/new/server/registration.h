#ifndef REGISTRATION_H
#define REGISTRATION_H
struct Client{
	char *login;
	int numberTest;
	int sizeQuestion;
	int sizeTrueAnswer;
	int sizes[4];
};
void writeSizeClient(struct Client *c,char *str);
char *writeLastResult(struct Client *c);
void newResult(struct Client *c,char number,int testsize,int numberTrueAnswer);
void writeClient(struct Client *c,char *str);
void freeClient(struct Client *c);
void new(char *login,struct Client *c);
int power(int x, int n);
#endif
