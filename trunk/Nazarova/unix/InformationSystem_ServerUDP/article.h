/*
 * article.h
 *
 *  Created on: Nov 7, 2014
 *      Author: user
 */

#ifndef ARTICLE_H_
#define ARTICLE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <libgen.h>



#define MAX_FILES 100
#define BUF_SIZE 128
#define MAX_SIZE 1024

typedef struct art
{
	char filename[BUF_SIZE];
	char title[BUF_SIZE];
	char author[BUF_SIZE];
} Article;

int check_file_existence(char *dir_name);
int add_article(char *dir_name, char *name, char* author, char *content);
int find_for_author(int sock, char *dir_name, char *author, struct sockaddr_in *ptr_addr);
char *lower(char *str);

#endif /* ARTICLE_H_ */
