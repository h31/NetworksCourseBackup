/*
 * article.c
 *
 *  Created on: Nov 7, 2014
 *      Author: user
 */

#include "article.h"

int add_article(char *dir_name, char *name, char* author, char *content)
{
	FILE *fp;
	if ((fp = fopen(dir_name,  "r" )) == NULL)
		if (errno == ENOENT)
			if ((fp = fopen(dir_name,  "w+" )) == NULL)
			{
				perror("File creation error");
				return -2;
			}
			else
			{
				fputs(name, fp);
				fputs(author, fp);
				fputs(content, fp);
				rewind(fp);
				close(fp);
				return 0;
			}
	close(fp);
	return -1;
}

int check_file_existence(char *dir_name)
{
	FILE *fp;
	if ((fp = fopen(dir_name,  "r" )) == NULL)
		if (errno == ENOENT)
			return 0;
	close(fp);
	return -1;
}

int find_for_author(SOCKET sock, char *dir_name, char *author)
{
	char buffer[BUF_SIZE];
	char path[MAX_SIZE];
	char *ptr;
	const char *delimiter = "----------------------|";
	int msg_size;
	char *filename;
	FILE *fp;
	DIR *dir = opendir(dir_name);
	memset(buffer,0, sizeof(buffer));
	struct stat about_file;
	Article *arts=(Article *)malloc(MAX_FILES * sizeof(Article));
	int i, k = 0;
	if(dir)
	{
		struct dirent *ent;
		while((ent = readdir(dir)) != NULL)
		{
			strcpy(path, dir_name);
			filename = ent->d_name;
			if ((fp = fopen(strcat(path, filename), "r")) == NULL)
			{
				printf("error %s\n", filename);
				perror("Opening of file error");
			}
			fstat(fileno(fp), &about_file);
			if ((about_file.st_mode & S_IFMT) != S_IFDIR)
			{
				for( i = 0; (ptr = fgets(buffer, sizeof(buffer), fp)) != NULL && i <2; i++)
				{
					if (i == 0)
						strcpy(arts[k].title, ptr);
					else if (i == 1)
						strcpy(arts[k].author, ptr);
					memset(ptr, 0, strlen(ptr));
				}
				strcpy(arts[k].filename, filename);
				k++;
			}
			fclose(fp);
		}
		closedir(dir);
		memset(buffer, 0, sizeof(buffer));
		strcat(buffer, "Search results for author: ");
		strcat(buffer, author);
		strcat(buffer, "|");
		strcat(buffer, delimiter);
		for (i = 0; k >= 0; --k)
		{
			if (strstr(lower(arts[k].author), lower(author)) != NULL)
			{
				strcat(buffer, arts[k].author);
				strcat(buffer, ":    ");
				strcat(buffer, arts[k].filename);
				strcat(buffer, "|");
				i++;
			}
		}
		strcat(buffer, delimiter);
		if (i == 0)
		{
			memset(buffer, 0, sizeof(buffer));
			strcat(buffer, "There are no articles of ");
			strcat(buffer, author);
			strcat(buffer, "|");
		}
		if ((msg_size = send(sock, buffer, strlen(buffer), 0)) == SOCKET_ERROR)
		{
			printf("SEND found result error: %d\n", WSAGetLastError());
			return -1;
		}
		printf("SEND  [%d bytes]: found result '%s'\n", msg_size, buffer);
		return 0;
	}
	free(arts);
	return -1;
}


char *lower(char *str)
{
	int i;
	char *new = strdup(str);
	for (i = 0; i < strlen(new); i++)
		new[i] = tolower(new[i]);
	return new;
}
