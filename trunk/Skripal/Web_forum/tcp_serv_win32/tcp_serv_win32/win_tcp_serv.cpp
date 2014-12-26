#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include "parser.h"
#include "tree.h"
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#include <assert.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#pragma comment (lib, "libxml2.lib")

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)  
#define bcopy(b1,b2,len) (memmove((b2), (b1), (len)), (void) 0)

#define	BUFSIZE		512
#define	PORT		5001
#define LOGINSIZE	100
#define	MAXUSERS	5

void print_topic_names(char *buffer,xmlNode * a_node, char* mess,int newsockfd);
void print_topic(char *buffer,int newsockfd);
void login_func(int newsockfd,char *log);
void search_user(xmlNode * a_node, char *login, char *password, char *id);
void search_new_topics(char *name,int newsockfg);
void search_topics_by_name(xmlNode * a_node, char *name, char *buffer);
void fnp_post_by_id(char *buffer,xmlNode * a_node, char *id,int newsockfd);
void fnp_post_name_by_id(xmlNode * a_node, char *id,int newsockfd);
void search_all_post(xmlNode * a_node, char all_posts[BUFSIZE-1][6],int *j);
void add_see_topic(xmlNode * a_node, char *login, char *topic_id);
void on_off_user(xmlNode * a_node, char *login,int on_off);
void search_online_user(char *buffer,xmlNode * a_node,int newsockfd);
void add_new_topic(char *buffer,xmlNode * a_node,int newsockfd,char *login);
void add_topic(xmlNode * a_node,char *login, char *topic, char *post, char *text, char *id);
void start_work(int newsockfd);
DWORD WINAPI startThread(LPVOID lpParam);

DWORD dwThreadId[MAXUSERS];
HANDLE thread[MAXUSERS];

struct sockParams
{
	int sockfd, newsockfd, port_number, client,countnumber;
	struct sockaddr_in serv_addr, cli_addr;
};

struct userThread
{
	char login[MAXUSERS][LOGINSIZE];
	int socket[MAXUSERS];
	int count;
};

struct userThread usersthr;

DWORD WINAPI workMainTh(LPVOID lpParam)
{
	char buffer[LOGINSIZE];
	int i;
	while(1)
	{
		fgets(buffer,LOGINSIZE-1,stdin);
		for(i=0;i<MAXUSERS;i++)
		{
			if(strcmp(buffer,usersthr.login[i]))
			{
				ExitThread(dwThreadId[i]);
				closesocket(usersthr.socket[i]);
			}
		}
	}
	return 0;
}

int main( int argc, char *argv[] )
{

	HANDLE mainthread;
	DWORD mtID;
	int i,j;
	struct sockParams sp;
	WSADATA wsaData;
	mainthread=CreateThread(NULL, 0, workMainTh, NULL, 0, &mtID);
	WSAStartup(MAKEWORD(2,2),&wsaData);
	usersthr.count=0;
	i=0;
	j=0;
    sp.sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bzero((char *) &sp.serv_addr, sizeof(sp.serv_addr));
    sp.port_number = PORT;
    sp.serv_addr.sin_family = AF_INET;
    sp.serv_addr.sin_addr.s_addr = INADDR_ANY;
    sp.serv_addr.sin_port = htons(sp.port_number);
	bind(sp.sockfd, (struct sockaddr *) &sp.serv_addr, sizeof(sp.serv_addr));
    while(1)
    {
    	listen(sp.sockfd,5);
    	sp.client = sizeof(sp.cli_addr);
		sp.newsockfd = accept(sp.sockfd, (struct sockaddr *)&sp.cli_addr, &sp.client);
		sp.port_number=i;
		thread[i] = CreateThread(NULL, 0, startThread, (LPVOID)&sp, 0, &dwThreadId[i]);
		i++;
	}
}

DWORD WINAPI startThread(LPVOID lpParam)
{
	struct sockParams *sp = (struct sockParams *)lpParam;
	start_work(sp->newsockfd);
	return 0;
}

void start_work(int newsockfd)
{
	int  n;
	xmlDoc         *doc = NULL;
	xmlNode        *root_element = NULL;
	const char     *TopicFilename = "topics.xml";
	const char     *UserFilename = "users.xml";
	char buffer[BUFSIZE],
		message[BUFSIZE];
	char id[6];
	char login[LOGINSIZE];

	bzero(login,LOGINSIZE);
	login_func(newsockfd,login);
	strcpy(usersthr.login[usersthr.count],login);
	usersthr.socket[usersthr.count]=newsockfd;
	usersthr.count++;
	bzero(buffer,BUFSIZE);
	strcpy(buffer,"next\n");
	n=send(newsockfd,buffer,7,0);
	while(1)
	{
		bzero(buffer,BUFSIZE);
		bzero(message,BUFSIZE);
		n= recv( newsockfd,buffer,BUFSIZE-1,0);
		printf("Buffer :%s\n",buffer);
			if(!strncmp(buffer,"topics",strlen("topics")))
	    	{
	    		print_topic(message,newsockfd);
	    	}
	    	if(!strncmp(buffer,"online",strlen("online")))
	    	{
	    		doc = xmlReadFile(UserFilename, NULL, 0);
	    	    if (doc == NULL)
	    	    {
	    			printf("error: could not parse file %s\n", UserFilename);
	    	        strcpy(message,"error");
				}
	    	    else
	    		{
	    			root_element = xmlDocGetRootElement(doc);
	    	        search_online_user(message,root_element,newsockfd);
	    	        xmlFreeDoc(doc);
	    	    }
	    		xmlCleanupParser();
	    	}
	    	if(!strncmp(buffer,"show",strlen("show")))
	    	{
	    	    bzero(id,6);
	    	    for(n=5;n<10;n++)
	    			id[n-5]=buffer[n];
	    	        doc = xmlReadFile(TopicFilename, NULL, 0);
	    	        if (doc == NULL)
	    	        {
	    	            printf("error: could not parse file %s\n", TopicFilename);
	    	        }
	    	        else
	    	        {
	    	            root_element = xmlDocGetRootElement(doc);
	    	            fnp_post_by_id(message,root_element,id,newsockfd);
	    	            xmlFreeDoc(doc);
	    	        }
	    	        xmlCleanupParser();
	    	        doc = xmlReadFile(UserFilename, NULL, 0);
	    	        if (doc == NULL)
	    	        {
	    				printf("error: could not parse file %s\n", UserFilename);
	    	            strcpy(message,"Error\n");
	    	        }
	    	        else
	    	        {
	    	            root_element = xmlDocGetRootElement(doc);
	    	            add_see_topic(root_element,login,id);
	    	            xmlSaveFile(UserFilename,doc);
	    	            xmlFreeDoc(doc);
	    	        }
	    	        xmlCleanupParser();
	    	   }
	    	   if(!strncmp(buffer,"exit",strlen("exit")))
	    	   {
	    	        bzero(buffer,BUFSIZE);
	    	        strncpy(message,"exit\n",strlen("exit\n"));
	    	        doc = xmlReadFile(UserFilename, NULL, 0);
	    	        if (doc == NULL)
	    	        {
	    	            printf("error: could not parse file %s\n", UserFilename);
	    	        }
	    	        else
	    	        {
	    	            root_element = xmlDocGetRootElement(doc);
	    	            on_off_user(root_element,login,0);
	    	            xmlSaveFile(UserFilename,doc);
	    	            xmlFreeDoc(doc);
	    	        }
	    	        xmlCleanupParser();
	    	        return;
	    	   }
	    	   if(!strncmp(buffer,"add",3))
	    	   {
	    	        doc = xmlReadFile(TopicFilename, NULL, 0);
	    	        if (doc == NULL)
	    	        {
	    	            printf("error: could not parse file %s\n", TopicFilename);
	    	            strcpy(message,"Error\n");
	    	        }
	    	        else
	    	        {
	    	            root_element = xmlDocGetRootElement(doc);
	    	            add_new_topic(buffer,root_element,newsockfd,login);
	    	            xmlSaveFile(TopicFilename,doc);
	    	            xmlFreeDoc(doc);
	    	        }
	    	        xmlCleanupParser();
	    	    }
	    	    if(message[0]==0)
	    	    {
	    	        strcpy(message,"next");
	    	    }
	    	    n=send(newsockfd,message,strlen(message),0);
	    }
}

void print_topic(char *buffer,int newsockfd)
{

    xmlDoc         *doc = NULL;
    xmlNode        *root_element = NULL;
    const char     *Filename = "topics.xml";
    doc = xmlReadFile(Filename, NULL, 0);
    if (doc == NULL)
    {
        printf("error: could not parse file %s\n", Filename);
    }
    else
    {
        root_element = xmlDocGetRootElement(doc);
         strcpy(buffer,"Topics:\n");
        print_topic_names(buffer,root_element,buffer,newsockfd);
        xmlFreeDoc(doc);
    }
    xmlCleanupParser();
    return;
}

void print_topic_names(char *buffer,xmlNode * a_node, char *mess, int newsockfd)
{
    xmlNode *cur_node = NULL;
    char buf[BUFSIZE];
    bzero(buf,BUFSIZE);
    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if((!xmlStrcmp(cur_node->name,(const xmlChar *)"topic")))
            {
                strncpy(buf,(char *)xmlGetProp(cur_node,(const xmlChar *)"name"),strlen((char *)xmlGetProp(cur_node,(const xmlChar *)"name")));
				
                strcat(buf,"\n");
                strcat(buffer,buf);
            }
            if((!xmlStrcmp(cur_node->name,(const xmlChar *)"post")))
            {
                strcat(buf,"	");
                strncat(buf,(char *)xmlGetProp(cur_node,(const xmlChar *)"id"),strlen((char *)xmlGetProp(cur_node,(const xmlChar *)"id")));
                strcat(buf,"	");
                strncat(buf,(char *)xmlGetProp(cur_node,(const xmlChar *)"name"),strlen((char *)xmlGetProp(cur_node,(const xmlChar *)"name")));
                strcat(buf,"\n");
                strcat(buffer,buf);
            }
        }
        print_topic_names(buffer,cur_node->children,mess,newsockfd);
    }
}

void login_func(int newsockfd,char * log)
{
	char login[BUFSIZE];
	char password[BUFSIZE];
	int n;
	char id[BUFSIZE];

	xmlDoc         *doc = NULL;
	xmlNode        *root_element = NULL;
	const char     *Filename = "users.xml";
	doc = xmlReadFile(Filename, NULL, 0);
	n = send(newsockfd,"Hello, print you login and password\n",36,0);
	bzero(login,BUFSIZE);
	bzero(password,BUFSIZE);
	n = recv( newsockfd,login,BUFSIZE-1,0);
	n=send(newsockfd,"OK",strlen("OK"),0);
	n = recv( newsockfd,password,BUFSIZE-1,0);
	if (doc == NULL)
	{
		printf("error: could not parse file %s\n", Filename);
	}
	else
	{
		if(!strncmp(login,"exit",strlen("exit")))
			exit(1);
		root_element = xmlDocGetRootElement(doc);
		bzero(id,BUFSIZE);
		login[strlen(login)-1] = 0;
		password[strlen(password)-1] = 0;
	    search_user(root_element,login,password,id);
	    if(id[0] == 0)
	    {
	    	printf("Invalid login --%s-- and password --%s-- \n Retry write you login and password\n",login,password);
	    	n=send(newsockfd,"OK",strlen("OK"),0);
	    	login_func(newsockfd,login);
	    }
	    else
	    {
	    	n=send(newsockfd,"next",strlen("next"),0);
	    	search_new_topics(login,newsockfd);
	    	root_element = xmlDocGetRootElement(doc);
	    	on_off_user(root_element,login,1);
	    	xmlSaveFile(Filename,doc);
	    	strcpy(log,login);
	    	n=send(newsockfd,"next",strlen("next"),0);
	    }

	    xmlFreeDoc(doc);
	}
	xmlCleanupParser();
	return;
}

void search_user(xmlNode * a_node, char *login, char *password,char *id)
{
	xmlNode *cur_node = NULL;
	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			if((!xmlStrcmp(cur_node->name,(const xmlChar *)"user")))
			{
				if((!xmlStrcmp(xmlGetProp(cur_node,(const xmlChar *)"login"),(const xmlChar *)login)))
				{
					if((!xmlStrcmp(xmlGetProp(cur_node,(const xmlChar *)"password"),(const xmlChar *)password)))
						if((!xmlStrcmp(xmlGetProp(cur_node,(const xmlChar *)"stat"),(const xmlChar *)"ff")))
						{
							strcpy(id,(char *)xmlGetProp(cur_node,(const xmlChar *)"id"));//strcpu
							return;
						}
				}
			}
	    }
	        search_user(cur_node->children,login,password,id);
	}
}

void search_new_topics(char *name, int newsockfg)
{
	char all_topics[BUFSIZE][6];
	char user_topics[BUFSIZE][6];
	char new_topics[BUFSIZE][6];
	char buffer[BUFSIZE];
	int i,j,k,i2,j2,k2,ch;
	xmlDoc         *doc = NULL;
	xmlNode        *root_element = NULL;
	const char     *Filename = "users.xml";
	const char	   *Filename2 = "topics.xml";
	doc = xmlReadFile(Filename, NULL, 0);
	for(i = 0; i< BUFSIZE;i++)
	{
		bzero(all_topics[i],6);
		bzero(user_topics[i],6);
		bzero(new_topics[i],6);
	}
	bzero(buffer,BUFSIZE);
	root_element = xmlDocGetRootElement(doc);
	search_topics_by_name(root_element,name,buffer);
	i=0;
	k=0;
	for(i =0; ((k < BUFSIZE-6) && (k<((int)strlen(buffer)-1)));i++)
	{
		for(j=0;j<5;j++)
		{
			user_topics[i][j]=buffer[k];
			k++;
		}
		k++;
	}
	bzero(buffer,BUFSIZE);
	doc = xmlReadFile(Filename2, NULL, 0);
	root_element = xmlDocGetRootElement(doc);
	i2=0;
	search_all_post(root_element,all_topics,&i2);
	xmlFreeDoc(doc);
	ch=0;
	j=0;
	for(k=0;k<i2;k++)
	{
		for(k2=0;k2<i;k2++)
		{
			if(!strncmp(user_topics[k2],all_topics[k],5))
			{
				ch=1;
			}

		}
		if(ch==0)
		{
			strncpy(new_topics[j],all_topics[k],5);
			j++;
		}
		ch =0;
	}
	doc = xmlReadFile(Filename2, NULL, 0);
	root_element = xmlDocGetRootElement(doc);
	for(j2=0;j2<j;j2++)
	{
		fnp_post_name_by_id(root_element,new_topics[j2],newsockfg);
	}
	xmlFreeDoc(doc);
	xmlCleanupParser();
}

void search_topics_by_name(xmlNode * a_node, char *name, char *buffer)
{
	xmlNode *cur_node = NULL;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			if((!xmlStrcmp(cur_node->name,(const xmlChar *)"user")))
			{
				if((!xmlStrcmp(xmlGetProp(cur_node,(const xmlChar *)"login"),(const xmlChar *)name)))
				{
					strcpy(buffer,(char *)xmlGetProp(cur_node,(const xmlChar *)"view"));
					return;
				}
			}
	    }
	        search_topics_by_name(cur_node->children,name,buffer);
	}
}

void fnp_post_by_id(char *buffer,xmlNode * a_node, char *id,int newsockfd)
{
    xmlNode *cur_node = NULL;
    char buf[BUFSIZE];
    bzero(buf,BUFSIZE);
    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if((!xmlStrcmp(cur_node->name,(const xmlChar *)"post")))
            {
                if((!xmlStrcmp(xmlGetProp(cur_node,(const xmlChar *)"id"),(const xmlChar *)id)))
                {
                    strncpy(buf,(char *)xmlGetProp(cur_node,(const xmlChar *)"text"),strlen((char *)xmlGetProp(cur_node,(const xmlChar *)"text")));
                    strcat(buf,"\n");
                    strcpy(buffer,buf);
                    return;
                }
            }
        }
        fnp_post_by_id(buffer,cur_node->children,id,newsockfd);
    }
}

void search_all_post(xmlNode * a_node, char all_posts[BUFSIZE-1][6],int *j)
{
	xmlNode *cur_node = NULL;
	    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
	        if (cur_node->type == XML_ELEMENT_NODE) {
	            if((!xmlStrcmp(cur_node->name,(const xmlChar *)"post")))
	            {
	            	strncpy(all_posts[*j], (char*)xmlGetProp(cur_node,(const xmlChar *)"id"),5);
	            	(*j)++;
				}
	        }
	        search_all_post(cur_node->children,all_posts,j);
	    }
}

void fnp_post_name_by_id(xmlNode * a_node, char *id,int newsockfd)
{
	xmlNode *cur_node = NULL;
	int n;
	char buf[BUFSIZE];
	bzero(buf,BUFSIZE);
			for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
				if (cur_node->type == XML_ELEMENT_NODE) {
					if((!xmlStrcmp(cur_node->name,(const xmlChar *)"post")))
					{
						if((!xmlStrcmp(xmlGetProp(cur_node,(const xmlChar *)"id"),(const xmlChar *)id)))
						{
							strncpy(buf,(char *)xmlGetProp(cur_node,(const xmlChar *)"name"),strlen((char *)xmlGetProp(cur_node,(const xmlChar *)"name")));
							strcat(buf,"\n");
							n = send(newsockfd,buf,strlen(buf),0);
							n=recv(newsockfd,buf,BUFSIZE-1,0);
							return;
						}
					}
			    }
				fnp_post_name_by_id(cur_node->children,id,newsockfd);
			}
}

void add_see_topic(xmlNode * a_node, char *login, char *topic_id)
{
	xmlNode *cur_node = NULL;
	char buf[BUFSIZE];
	bzero(buf,BUFSIZE);
	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			if((!xmlStrcmp(cur_node->name,(const xmlChar *)"user")))
			{
				if((!xmlStrcmp(xmlGetProp(cur_node,(const xmlChar *)"login"),(const xmlChar *)login)))
				{
					strncpy(buf,(char *)xmlGetProp(cur_node,(const xmlChar *)"view"),strlen((char *)xmlGetProp(cur_node,(const xmlChar *)"view")));
					strncat(buf,topic_id,5);
					strncat(buf," ",1);
					xmlSetProp(cur_node,(const xmlChar *)"view",(const xmlChar *)buf);
					return;
				}
			}
	    }
		printf("%s\n",buf);
		add_see_topic(cur_node->children,login,topic_id);
	}
}
void on_off_user(xmlNode * a_node, char *login,int on_off)
{
	xmlNode *cur_node = NULL;
		char buf[BUFSIZE];
		bzero(buf,BUFSIZE);
		for (cur_node = a_node; cur_node; cur_node = cur_node->next)
		{
			if (cur_node->type == XML_ELEMENT_NODE) {
				if((!xmlStrcmp(cur_node->name,(const xmlChar *)"user")))
				{
					if((!xmlStrcmp(xmlGetProp(cur_node,(const xmlChar *)"login"),(const xmlChar *)login)))
					{
						if(on_off==1)
							xmlSetProp(cur_node,(const xmlChar *)"stat",(const xmlChar *)"on");
						else
						{
							xmlSetProp(cur_node,(const xmlChar *)"stat",(const xmlChar *)"ff");
						}
						return;
					}
				}
		    }
			on_off_user(cur_node->children,login,on_off);
		}
}
void search_online_user(char *buffer,xmlNode * a_node,int newsockfd)
{
    xmlNode *cur_node = NULL;
    for (cur_node = a_node; cur_node; cur_node = cur_node->next)
    {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if((!xmlStrcmp(cur_node->name,(const xmlChar *)"user")))
            {
                if((!xmlStrcmp((const xmlChar *)xmlGetProp(cur_node,(const xmlChar *)"stat"),(const xmlChar *)"on")))
                {
                	strcat(buffer,(char *)xmlGetProp(cur_node,(const xmlChar *)"login"));
                }
            }
        }
        search_online_user(buffer,cur_node->children,newsockfd);
    }
}
void add_new_topic(char *buffer,xmlNode * a_node,int newsockfd,char *login)
{
    int n=0,i;
    char topic_name[BUFSIZE];
    char post_name[BUFSIZE];
    char post_message[BUFSIZE];
    char id[6];
    bzero(topic_name,BUFSIZE);
    for(i=4;buffer[i]!='&';i++)
    {
    	topic_name[n]=buffer[i];
    	n++;
    }
    i++;
    bzero(post_name,BUFSIZE);
    n=0;
    for(;buffer[i]!='&';i++)
    {
        post_name[n]=buffer[i];
        n++;
    }
    i++;
    bzero(post_message,BUFSIZE);
    n=0;
        for(;buffer[i]!=0;i++)
        {
            post_message[n]=buffer[i];
            n++;
        }
    post_message[strlen(post_message)-1]=0;
    printf("---%s---%s---%s---\n",topic_name,post_name,post_message);
    bzero(id,6);
    for(i=0;i<5;i++)
        id[i]=(rand()%10+'0');
    add_topic(a_node,login,topic_name,post_name,post_message,id);
}

void add_topic(xmlNode * a_node,char *login, char *topic, char *post, char *text, char *id)
{
	xmlNode *cur_node = NULL;
	char buf[BUFSIZE];
	bzero(buf,BUFSIZE);
	for (cur_node = a_node; cur_node; cur_node = cur_node->next)
	{
		if (cur_node->type == XML_ELEMENT_NODE)
		{
			if(!xmlStrcmp(cur_node->name,(const xmlChar *)"topic"))
				if(!xmlStrcmp((const xmlChar *)xmlGetProp(cur_node,(const xmlChar *)"name"),(const xmlChar *)topic))
				{
					xmlNodePtr nNode = xmlNewNode(0,(const xmlChar *)"post");
					xmlSetProp(nNode,(const xmlChar *)"id",(const xmlChar *)id);
					xmlSetProp(nNode,(const xmlChar *)"name",(const xmlChar *)post);
					xmlSetProp(nNode,(const xmlChar *)"autor",(const xmlChar *)login);
					xmlSetProp(nNode,(const xmlChar *)"text",(const xmlChar *)text);
					xmlAddChild(cur_node,nNode);
					return;
				}
		}
		add_topic(cur_node->children,login, topic, post, text, id);
	}
}
