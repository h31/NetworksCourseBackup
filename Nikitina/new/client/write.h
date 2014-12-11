#ifndef REGISTRATION_H
#define REGISTRATION_H
#define N 4
struct Line{
	int number;
	char *question;
	char **answer;
	int sizeNumber;
	int *sizeAnswer;
	int sizeQuest;
	};
void writeStruct (struct Line *x, char *str);
void writeSize(struct Line *x,char *str);
void freeLine(struct Line *x);
void writeToClient(struct Line *x,char *str,char *stringOut);
int writeListTest(int *listOfTest,char *buffer, char *string);
int power(int x, int n);
int toInt(char *buffer);
int sizeStr(struct Line *x);
struct Client{
	char *login;
	int numberTest;
	int sizeQuestion;
	int sizeTrueAnswer;
	int sizes[4];
};
void writeSizeClient(struct Client *c,char *str);
void writeClient(struct Client *c,char *str);
#endif
