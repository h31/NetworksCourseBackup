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
void registration(char *login,char *parol);
void writeSizeClient(struct Client *c,char *str);
char *writeLastResult(struct Client *c);
void writeClient(struct Client *c,char *str);
void freeClient(struct Client *c);
#endif
