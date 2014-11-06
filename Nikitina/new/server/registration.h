#ifndef REGISTRATION_H
#define REGISTRATION_H
struct Client{
	char *login;
	char *parol;
	int numberTest;
	int sizeQuestion;
	int sizeTrueAnswer;
	int sizes[5];
};
void registration(struct Client *c,int reWrite);
void writeSizeClient(struct Client *c,char *str);
char *writeLastResult(struct Client *c);
void newResult(struct Client *c,int number,int testsize,int numberTrueAnswer);
void writeClient(struct Client *c,char *str);
void freeClient(struct Client *c);
void new(char *login,char *parol,struct Client *c);
#endif
