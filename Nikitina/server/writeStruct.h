#ifndef WRITESTRUCT_H
#define WRITESTRUCT_H
#define N 4

struct Line{
	int number;
	char *question;
	char **answer;
	int sizeNumber;
	int *sizeAnswer;
	int sizeQuest;
	char trueAnswer;
	};
int sizeFile();
int power(int x, int n) ;
void writeStruct (struct Line *x, char *str);
void writeSize(struct Line *x,char *str);
void freeLine(struct Line *x);
char *writeToClient(struct Line *x);
#endif
