#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "registration.h"
#include "writeStruct.h"
void registration(char *login,char *parol){
	FILE *file;
	file=fopen("/home/user/workspace/server/registration.txt","a");
	fprintf(file,"%d#%d#%d#%s#%s/\n",0,0,0,login, parol);
	fclose(file);
}
void writeSizeClient(struct Client *c,char *str){
	int i=0;
	int state;
	for(state=0;state<5;state++){
		c->sizes[state]=0;
	}
	state=0;
	while(str[i]!='/'){
		if(str[i]=='#'){
				i++;
				state++;
		}
		c->sizes[state]++;
		i++;
	}
	writeClient(c, str);
}
void writeClient(struct Client *c, char *str){
	int i=0;
	c->login=(char*)malloc(c->sizes[3]*sizeof(char));
	c->parol=(char*)malloc(c->sizes[4]*sizeof(char));
	c->numberTest=c->sizeQuestion=c->sizeTrueAnswer=0;
	int state;int j=0;
	int add;int pow;

	for(state=0;state<3;state++){
	pow=c->sizes[state]-1;
	if(str[i]=='#')
		i++;
	while(str[i]!='#'){
		add=((int)str[i]-'0')*power(10,pow);
		switch(state){
						case 0:c->numberTest=c->numberTest+add;break;
						case 1:c->sizeQuestion=c->sizeQuestion+add;break;
						case 2:c->sizeTrueAnswer=c->sizeTrueAnswer+add;break;
	}
		i++;pow--;
	}}
	while(str[i]!='/'){
		if(str[i]=='#'){
					state++;
					i++;
					j=0;
		}
		switch(state){
				case 4:c->login[j]=str[i];break;
				case 5:c->parol[j]=str[i];break;
		}
		i++;j++;
	}
}
char *writeLastResult(struct Client *c){
	char *string=c->login;
	 char* num=(char*)malloc(50*sizeof(char));
	 sprintf(num, " your last test is %d. Result %d of %d.", c->numberTest,
			 c->sizeQuestion,c->sizeTrueAnswer);
	 string=strcat(string,num);
	 free(num);
	string=strcat(string,"\n");
	return string;
}
void freeClient(struct Client *c){
	free(c->login);
	free(c->parol);

}
