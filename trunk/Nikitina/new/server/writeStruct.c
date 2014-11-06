#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "writeStruct.h"

int power(int x, int n)
 {
	int i;
	int a=1;
     for(i=0;i<n;i++){
    	 a=a*x;
     }
     return a;
 }
void writeStruct(struct Line *x, char *str){
	int i;
	x->question=(char*)malloc(x->sizeQuest*sizeof(char));
	x->answer=(char**)malloc(N*sizeof(char*));
	for(i=0;i<N;i++){
		x->answer[i]=(char*)malloc(x->sizeAnswer[i]*sizeof(char));
	}
	x->number=0;
	int state=0;int j=0;
	i=0;
	while(str[i]!='#'){
		x->number=x->number+((int)str[i]-'0')*power(10,x->sizeNumber-1-i);
		i++;
	}
	while(str[i]!='/'){
		if(str[i]=='#'){
				state++;
				i++;
				j=0;
		}
		if(str[i]=='!')
			i++;
		switch(state){
				case 1:x->question[j]=str[i];break;
				default: x->answer[state-2][j]=str[i];break;
		}
		i++;j++;
	}
}
void writeSize(struct Line *x,char *str){
	x->sizeQuest=0;
	x->sizeNumber=0;
	x->sizeAnswer=(int*)malloc(N*sizeof(int));
	int i;
	for(i=0;i<N;i++){
		x->sizeAnswer[i]=0;
	}
	i=0;
	int state=0;
	while(str[i]!='/'){
		if(str[i]=='#'){
				state++;
				i++;
		}
		if(str[i]=='!'){
			x->trueAnswer=state-1+'0';
			i++;
		}
		switch(state){
				case 0:x->sizeNumber++;break;
				case 1:x->sizeQuest++;break;
				default: x->sizeAnswer[state-2]++;break;
		}
		i++;
	}
	writeStruct(x, str);
}
void freeLine(struct Line *x){
	int i;
	free(x->question);
	free(x->sizeAnswer);
	for(i=0;i<N;i++){
		free(x->answer[i]);
	}
	free(x->answer);

}
char *writeToClient(struct Line *x){
	char *string=x->question;
	int i=0;
	for(i=0;i<4;i++){
	char* num=(char*)malloc((x->sizeAnswer[i]+3)*sizeof(char));
	 sprintf(num, " %d)%s", i+1, x->answer[i]);
	 string=strcat(string,num);
	 free(num);
	   }
	string=strcat(string,"\n");
	return string;
}
int sizeFile(char *str){
	FILE *file;
	int i=0;
	char string[50];
	file = fopen(str, "r");
	while(fgets(string,sizeof(string),file))
		i++;
	fclose(file);
	return i;
}
