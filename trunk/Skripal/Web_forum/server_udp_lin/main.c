#include <stdio.h>
#include </usr/include/libxml2/libxml/parser.h>
#include </usr/include/libxml2/libxml/tree.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

void print_topic_names(xmlNode * a_node, char* mess,void *in);
void print_topic(void *in);
void login_func(void *in,char *log);
void search_user(xmlNode * a_node, char *login, char *password, char *id);
void search_new_topics(char *name,void *in);
void search_topics_by_name(xmlNode * a_node, char *name, char *buffer);
void fnp_post_by_id(xmlNode * a_node, char *id,void *in);
void fnp_post_name_by_id(xmlNode * a_node, char *id,void *in);
void search_all_post(xmlNode * a_node, char all_posts[255][6],int *j);
void add_see_topic(xmlNode * a_node, char *login, char *topic_id);
void on_off_user(xmlNode * a_node, char *login,int on_off);
void search_online_user(xmlNode * a_node,void *in);
void add_new_topic(xmlNode * a_node,void *in,char *login);
void add_topic(xmlNode * a_node,char *login, char *topic, char *post, char *text, char *id);
void start_work(void *in);

#define BUFLEN 512
#define PORT 8888

struct sockParams
{
	struct sockaddr_in si_me, si_other;
	int sock,slen,recv_len;
	char buf[BUFLEN];
};

int main( int argc, char *argv[] )
{
	struct sockParams sp;
	sp.slen = sizeof(sp.si_other);

   /* First call to socket() function */
	 if((sp.sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
			printf("Error!!!!\n");

    /* Initialize socket structure */
    memset((char *) &sp.si_me, 0, sizeof(sp.si_me));

    sp.si_me.sin_family = AF_INET;
    sp.si_me.sin_port = htons(PORT);
    sp.si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    /* Now bind the host address using bind() call.*/
    if (bind(sp.sock, (struct sockaddr *) &sp.si_me,
                          sizeof(sp.si_me)) == -1)
    {
         perror("ERROR on binding");
         exit(1);
    }

    /* Now start listening for the clients, here process will
    * go in sleep mode and will wait for the incoming connection
    */
    bzero(sp.buf,256);
    if(recvfrom(sp.sock,sp.buf,BUFLEN,0,(struct sockaddr *) &(sp.si_other),&(sp.slen)) == -1)
    {
    	perror("ERROR reading from socket");
    	exit(1);
    }
    start_work((void *) &sp);
}

void start_work(void *in)
{

	int  n;
	xmlDoc         *doc = NULL;
	xmlNode        *root_element = NULL;
	const char     *TopicFilename = "topics.xml";
	const char     *UserFilename = "users.xml";
	char buffer[256];
	char id[6];
	char login[100];

	struct sockParams *sp = (struct sockParams *)in;
	bzero(login,100);
	    login_func(sp,login);
	    bzero(buffer,256);
	    /*strcpy(buffer,"next\n");
	    if(sendto(sp->sock,buffer, strlen(buffer),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
	    {
	       	perror("ERROR writing to socket");
	       	exit(1);
	    }*/
	    bzero(buffer,256);
	    			if(recvfrom(sp->sock,buffer,BUFLEN,0,(struct sockaddr *) &(sp->si_other),&(sp->slen)) == -1)
	    			{
	    			   	perror("ERROR reading from socket");
	    			   	exit(1);
	    			}
	    			printf("---%s---\n",buffer);
	    while(1)
	    {
	    	/*bzero(buffer,256);
	    	strcpy(buffer,"next");
	    	printf("Cycle\n");
	    	if(sendto(sp->sock,buffer, strlen(buffer),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
	    	{
	    	   	perror("ERROR writing to socket");
	    	   	exit(1);
	    	}
			bzero(buffer,256);
			if(recvfrom(sp->sock,buffer,BUFLEN,0,(struct sockaddr *) &(sp->si_other),&(sp->slen)) == -1)
			{
			   	perror("ERROR reading from socket");
			   	exit(1);
			}
			printf("---%s---\n",buffer);*/
			if(!strncmp(buffer,"topics",6))
			{
				/*printf("TOPICS\n");
				strcpy(buffer,"TOPICS");
				if(sendto(sp->sock,buffer, strlen(buffer),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
					    	{
					    	   	perror("ERROR writing to socket");
					    	   	exit(1);
					    	}
							bzero(buffer,256);
							if(recvfrom(sp->sock,buffer,BUFLEN,0,(struct sockaddr *) &(sp->si_other),&(sp->slen)) == -1)
							{
							   	perror("ERROR reading from socket");
							   	exit(1);
							}
							*/
				print_topic(sp);
				//n = write(newsockfd,buffer,255);
				if(sendto(sp->sock,"next", strlen("next"),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
				{
					perror("ERROR writing to socket");
				    exit(1);
				}
				if(recvfrom(sp->sock,buffer,BUFLEN,0,(struct sockaddr *) &(sp->si_other),&(sp->slen)) == -1)
				{
					perror("ERROR reading from socket");
				    exit(1);
				}
				continue;
			}
			if(!strncmp(buffer,"online",6))
			{
				printf("ONLINE\n");
				doc = xmlReadFile(UserFilename, NULL, 0);
				if (doc == NULL)
				{
					printf("error: could not parse file %s\n", UserFilename);
					if(sendto(sp->sock,"Error!\n", 8,0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
						{
						   	perror("ERROR writing to socket");
						   	exit(1);
						}
					continue;
				}
				else
				{
				    root_element = xmlDocGetRootElement(doc);
				    search_online_user(root_element,sp);
				    xmlFreeDoc(doc);
				}
				xmlCleanupParser();
				if(sendto(sp->sock,"next", strlen("next"),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
				{
					perror("ERROR writing to socket");
				    exit(1);
				}
				if(recvfrom(sp->sock,buffer,BUFLEN,0,(struct sockaddr *) &(sp->si_other),&(sp->slen)) == -1)
				{
					perror("ERROR reading from socket");
				    exit(1);
				}
				continue;
			}
			if(!strncmp(buffer,"show",4))
			{
				printf("SHOW\n");
				bzero(id,6);
				for(n=5;n<10;n++)
					id[n-5]=buffer[n];
			    doc = xmlReadFile(TopicFilename, NULL, 0);
			    if (doc == NULL)
			    {
			        printf("error: could not parse file %s\n", TopicFilename);
			        if(sendto(sp->sock,"Error", 6,0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
			        	{
			        	   	perror("ERROR writing to socket");
			        	   	exit(1);
			        	}
			    }
			    else
			    {
	                root_element = xmlDocGetRootElement(doc);
	                fnp_post_by_id(root_element,id,sp);
	                xmlFreeDoc(doc);
	            }
		        xmlCleanupParser();

		        doc = xmlReadFile(UserFilename, NULL, 0);
		        if (doc == NULL)
		        {
		             printf("error: could not parse file %s\n", UserFilename);
		             if(sendto(sp->sock,"Error", 6,0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
		             	{
		             	   	perror("ERROR writing to socket");
		             	   	exit(1);
		             	}
		        }
		        else
		        {
		             root_element = xmlDocGetRootElement(doc);
		             add_see_topic(root_element,login,id);
		             xmlSaveFile(UserFilename,doc);
		             xmlFreeDoc(doc);
		        }
		       	xmlCleanupParser();
		       	if(sendto(sp->sock,"next", strlen("next"),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
		       	{
		       		perror("ERROR writing to socket");
		       		exit(1);
		       	}
		       	if(recvfrom(sp->sock,buffer,BUFLEN,0,(struct sockaddr *) &(sp->si_other),&(sp->slen)) == -1)
		       	{
		       		perror("ERROR reading from socket");
		       	    exit(1);
		       	}
		       	continue;
			}
			if(!strncmp(buffer,"exit",4))
			{
				printf("EXIT\n");
				bzero(buffer,256);
				strncpy(buffer,"exit\n",5);
				if(sendto(sp->sock,buffer, strlen(buffer),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
					{
					   	perror("ERROR writing to socket");
					   	exit(1);
					}
				doc = xmlReadFile(UserFilename, NULL, 0);
				if (doc == NULL)
				{
					printf("error: could not parse file %s\n", UserFilename);
					bzero(buffer,256);
					strncpy(buffer,"Error\n",6);
					if(sendto(sp->sock,buffer, strlen(buffer),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
					{
					   	perror("ERROR writing to socket");
					   	exit(1);
					}
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
				printf("ADD\n");
				doc = xmlReadFile(TopicFilename, NULL, 0);
				if (doc == NULL)
				{
					printf("error: could not parse file %s\n", TopicFilename);
					bzero(buffer,256);
					strncpy(buffer,"Error\n",6);
					if(sendto(sp->sock,buffer, strlen(buffer),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
					{
					   	perror("ERROR writing to socket");
					   	exit(1);
					}
				}
				else
				{
				    root_element = xmlDocGetRootElement(doc);
				    add_new_topic(root_element,sp,login);
				    xmlSaveFile(TopicFilename,doc);
					xmlFreeDoc(doc);
				}
				xmlCleanupParser();
				if(sendto(sp->sock,"next", strlen("next"),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
				{
					perror("ERROR writing to socket");
				    exit(1);
				}
				if(recvfrom(sp->sock,buffer,BUFLEN,0,(struct sockaddr *) &(sp->si_other),&(sp->slen)) == -1)
				{
					perror("ERROR reading from socket");
				    exit(1);
				}
				continue;
			}
			bzero(buffer,256);
			strcpy(buffer,"next");
				    	printf("Cycle\n");
				    	if(sendto(sp->sock,buffer, strlen(buffer),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
				    	{
				    	   	perror("ERROR writing to socket");
				    	   	exit(1);
				    	}
				    	bzero(buffer,256);
				    				if(recvfrom(sp->sock,buffer,BUFLEN,0,(struct sockaddr *) &(sp->si_other),&(sp->slen)) == -1)
				    				{
				    				   	perror("ERROR reading from socket");
				    				   	exit(1);
				    				}
				    				printf("---%s---\n",buffer);
	    }
}

void print_topic(void *in)
{

        xmlDoc         *doc = NULL;
        xmlNode        *root_element = NULL;
        const char     *Filename = "topics.xml";
        char			buffer[256];
        struct sockParams *sp = (struct sockParams *)in;
        doc = xmlReadFile(Filename, NULL, 0);
        printf("start topics\n");
        if (doc == NULL)
          {
                  printf("error: could not parse file %s\n", Filename);
                  //strncat(buffer,"Error\n",6);
                  bzero(buffer,256);
                  strncpy(buffer,"Error\n",6);
                  if(sendto(sp->sock,buffer, strlen(buffer),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
                  {
                	  perror("ERROR writing to socket");
                	  exit(1);
                  }
          }
        else
          {

                  root_element = xmlDocGetRootElement(doc);
                 // strncat(buffer,"Topics:\n",10);
                  /*bzero(buffer,256);
                  strncpy(buffer,"Topics\n",7);*/
                  printf("TOPICS\n");
                  if(sendto(sp->sock,"Topics\n", 10,0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
                  {
                	  perror("ERROR writing to socket");
                	  exit(1);
                  }
                  if(recvfrom(sp->sock,buffer,BUFLEN,0,(struct sockaddr *) &(sp->si_other),&(sp->slen)) == -1)
                  {
                	  perror("ERROR reading from socket");
                      exit(1);
                  }
                  print_topic_names(root_element,buffer,sp);
                  xmlFreeDoc(doc);
          }

        xmlCleanupParser();

        return;

}

void print_topic_names(xmlNode * a_node, char *mess, void *in)
{
    xmlNode *cur_node = NULL;
    char buf[256];
    bzero(buf,256);
    struct sockParams *sp = (struct sockParams *)in;
    printf("start topic names\n");
    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if((!xmlStrcmp(cur_node->name,(const xmlChar *)"topic")))
            {
            	strncpy(buf,xmlGetProp(cur_node,"name"),strlen(xmlGetProp(cur_node,"name")));
            	strcat(buf,"\n");
            	printf("---%s--\n",buf);
            	if(sendto(sp->sock,buf, strlen(buf),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
            	{
            		perror("ERROR writing to socket");
            	    exit(1);
            	}
            	if(recvfrom(sp->sock,buf,BUFLEN,0,(struct sockaddr *) &(sp->si_other),&(sp->slen)) == -1)
            	{
            	   	perror("ERROR reading from socket");
            	   	exit(1);
            	}

			}
            if((!xmlStrcmp(cur_node->name,(const xmlChar *)"post")))
            {
                strcpy(buf,"	");
            	strncat(buf,xmlGetProp(cur_node,"id"),strlen(xmlGetProp(cur_node,"id")));
            	strcat(buf,"	");
            	strncat(buf,xmlGetProp(cur_node,"name"),strlen(xmlGetProp(cur_node,"name")));
                strcat(buf,"\n");
                if(sendto(sp->sock,buf, strlen(buf),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
                {
                	perror("ERROR writing to socket");
                    exit(1);
                }
                if(recvfrom(sp->sock,buf,BUFLEN,0,(struct sockaddr *) &(sp->si_other),&(sp->slen)) == -1)
                {
                  	perror("ERROR reading from socket");
                   	exit(1);
                }
            }
        }

        print_topic_names(cur_node->children,mess,sp);
    }
}

void login_func(void *in,char * log)
{
	char login[256];
	char password[256];
	char id[256];
	char message[256];
	xmlDoc         *doc = NULL;
	xmlNode        *root_element = NULL;
	const char     *Filename = "users.xml";
	doc = xmlReadFile(Filename, NULL, 0);
	struct sockParams *sp = (struct sockParams *)in;

	bzero(message,256);
	strncpy(message,"Hello, print you login and password\n",36);
	if(sendto(sp->sock,message, strlen(message),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
	  	{
	 	 	perror("ERROR writing to socket");
	   	   	exit(1);
	   	}
	bzero(login,256);
	bzero(password,256);
	if(recvfrom(sp->sock,login,BUFLEN,0,(struct sockaddr *) &(sp->si_other),&(sp->slen)) == -1)
	    {
	      	perror("ERROR reading from socket");
	       	exit(1);
	    }
	bzero(message,256);
	strcpy(message,"OK");
	if(sendto(sp->sock,message, strlen(message),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
	{
	   	perror("ERROR writing to socket");
	   	exit(1);
	}
	if(recvfrom(sp->sock,password,BUFLEN,0,(struct sockaddr *) &(sp->si_other),&(sp->slen)) == -1)
	{
	  	perror("ERROR reading from socket");
	   	exit(1);
	}

	if (doc == NULL)
	{
		printf("error: could not parse file %s\n", Filename);
	}
	else
	{
		if(!strncmp(login,"exit",4))
			exit(1);
		root_element = xmlDocGetRootElement(doc);
		bzero(id,256);
		login[strlen(login)-1] = 0;
		//login[strlen(login)-1] = 0;
		//password[strlen(password)-1] = 0;
		password[strlen(password)-1] = 0;
	    search_user(root_element,login,password,id);
	    if(id[0] == 0)
	    {
	    	printf("Invalid login --%s-- and password --%s-- \n Retry write you login and password\n",login,password);
	    	bzero(message,256);
	    	strcpy(message,"OK");
	    	if(sendto(sp->sock,message, strlen(message),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
	    	{
	    	   	perror("ERROR writing to socket");
	    	   	exit(1);
	    	}
	    	login_func(sp,login);
	    }
	    else
	    {
	    	bzero(message,256);
	    	strcpy(message,"next");
	    	if(sendto(sp->sock,message, strlen(message),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
	    	{
	    	   	perror("ERROR writing to socket");
	    	   	exit(1);
	    	}
	    	search_new_topics(login,sp);
	    	root_element = xmlDocGetRootElement(doc);
	    	on_off_user(root_element,login,1);
	    	xmlSaveFile(Filename,doc);
	    	strcpy(log,login);
	    	bzero(message,256);
	    	/*strcpy(message,"next");
	    	if(sendto(sp->sock,message, strlen(message),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
	    	{
	    	   	perror("ERROR writing to socket");
	    	   	exit(1);
	    	}*/
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
				if((!xmlStrcmp(xmlGetProp(cur_node,"login"),(const xmlChar *)login)))
				{
					if((!xmlStrcmp(xmlGetProp(cur_node,"password"),(const xmlChar *)password)))
						if((!xmlStrcmp(xmlGetProp(cur_node,"stat"),(const xmlChar *)"ff")))
						{
							strcpy(id,xmlGetProp(cur_node,"id"));//strcpu
							return;
						}
				}
			}
	    }
	        search_user(cur_node->children,login,password,id);
	}
}

void search_new_topics(char *name, void *in)
{
	char all_topics[256][6];
	char user_topics[256][6];
	char new_topics[256][6];
	char buffer[256];
	int i,j,k,i2,j2,k2,ch;
	xmlDoc         *doc = NULL;
	xmlNode        *root_element = NULL;
	const char     *Filename = "users.xml";
	const char	   *Filename2 = "topics.xml";
	struct sockParams *sp = (struct sockParams *)in;
	doc = xmlReadFile(Filename, NULL, 0);

	for(i = 0; i< 256;i++)
		{
			bzero(all_topics[i],6);
			bzero(user_topics[i],6);
			bzero(new_topics[i],6);
		}
	//Find view user post
	bzero(buffer,256);
	root_element = xmlDocGetRootElement(doc);
	search_topics_by_name(root_element,name,buffer);
	i=0;//User post size
	k=0;
	for(i =0; k < 250 && k<(strlen(buffer)-1);i++)
	{
		for(j=0;j<5;j++)
		{
			user_topics[i][j]=buffer[k];
			k++;
		}
		k++;
	}

	bzero(buffer,256);
	//Find all post
	doc = xmlReadFile(Filename2, NULL, 0);
	root_element = xmlDocGetRootElement(doc);
	i2=0;
	search_all_post(root_element,all_topics,&i2);
	xmlFreeDoc(doc);
	/*i2=0;//all post size
	k2=0;
	for(i2 =0; k2 < 250 && k2<(strlen(buffer)-1);i2++)
	{
		for(j2=0;j2<5;j2++)
		{
			all_topics[i2][j2]=buffer[k2];
			k2++;
		}
		//printf("Topic id1 %s \n",all_topics[i2]);
		k2++;
		printf("Topic id1 %s \n",all_topics[0]);
	}*/
	//printf("all -%s- -%s- -%s- \n", all_topics[0],all_topics[1],all_topics[2]);
	ch=0;
	j=0;
	for(k=0;k<i2;k++)
	{
		//printf("Topic id2 %s \n",all_topics[k]);
		for(k2=0;k2<i;k2++)
		{
			if(!strncmp(user_topics[k2],all_topics[k],5))
			{
				ch=1;
				//printf("Old topic id %s \n",user_topics[k2]);
			}

		}
		if(ch==0)
		{
			strncpy(new_topics[j],all_topics[k],5);
			//printf("New topic id %s \n",new_topics[j]);
			j++;
		}
		ch =0;
	}
	//printf("%d \n",j);
	doc = xmlReadFile(Filename2, NULL, 0);
	root_element = xmlDocGetRootElement(doc);
	for(j2=0;j2<j;j2++)
	{
		//printf("New topic id %s \n",new_topics[j2]);
		fnp_post_name_by_id(root_element,new_topics[j2],sp);
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
				if((!xmlStrcmp(xmlGetProp(cur_node,"login"),(const xmlChar *)name)))
				{
					strcpy(buffer,xmlGetProp(cur_node,"view"));
					return;
				}
			}
	    }
	        search_topics_by_name(cur_node->children,name,buffer);
	}
}

void fnp_post_by_id(xmlNode * a_node, char *id,void *in)
{
	xmlNode *cur_node = NULL;
	char buf[256];
	struct sockParams *sp = (struct sockParams *)in;
	bzero(buf,256);
			for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
				if (cur_node->type == XML_ELEMENT_NODE) {
					if((!xmlStrcmp(cur_node->name,(const xmlChar *)"post")))
					{
						if((!xmlStrcmp(xmlGetProp(cur_node,"id"),(const xmlChar *)id)))
						{
							strncpy(buf,xmlGetProp(cur_node,"text"),strlen(xmlGetProp(cur_node,"text")));
							strcat(buf,"\n");
							if(sendto(sp->sock,buf, strlen(buf),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
							{
								perror("ERROR writing to socket");
							    exit(1);
							}
							//printf("%s\n",xmlGetProp(cur_node,"text"));
							if(recvfrom(sp->sock,buf,BUFLEN,0,(struct sockaddr *) &(sp->si_other),&(sp->slen)) == -1)
							{
							   	perror("ERROR reading from socket");
							   	exit(1);
							}
							return;
						}
					}
			    }
				fnp_post_by_id(cur_node->children,id,sp);
			}
}

void search_all_post(xmlNode * a_node, char all_posts[255][6],int *j)
{
	xmlNode *cur_node = NULL;
	    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
	        if (cur_node->type == XML_ELEMENT_NODE) {
	            if((!xmlStrcmp(cur_node->name,(const xmlChar *)"post")))
	            {
	            	strncpy(all_posts[*j], xmlGetProp(cur_node,"id"),5);
	            	(*j)++;
				}
	        }

	        search_all_post(cur_node->children,all_posts,j);
	    }
}

void fnp_post_name_by_id(xmlNode * a_node, char *id,void *in)
{
	xmlNode *cur_node = NULL;
	char buf[256];
	struct sockParams *sp = (struct sockParams *)in;
	bzero(buf,256);
			for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
				if (cur_node->type == XML_ELEMENT_NODE) {
					if((!xmlStrcmp(cur_node->name,(const xmlChar *)"post")))
					{
						if((!xmlStrcmp(xmlGetProp(cur_node,"id"),(const xmlChar *)id)))
						{
							strncpy(buf,xmlGetProp(cur_node,"name"),strlen(xmlGetProp(cur_node,"name")));
							strcat(buf,"\n");
							if(sendto(sp->sock,buf, strlen(buf),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
							{
								perror("ERROR writing to socket");
							    exit(1);
							}
							//printf("%s\n",xmlGetProp(cur_node,"text"));
							if(recvfrom(sp->sock,buf,BUFLEN,0,(struct sockaddr *) &(sp->si_other),&(sp->slen)) == -1)
							{
							  	perror("ERROR reading from socket");
							   	exit(1);
							}
							return;
						}
					}
			    }
				fnp_post_by_id(cur_node->children,id,sp);
			}
}

void add_see_topic(xmlNode * a_node, char *login, char *topic_id)
{
	xmlNode *cur_node = NULL;
	char buf[256];
	bzero(buf,256);
	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			if((!xmlStrcmp(cur_node->name,(const xmlChar *)"user")))
			{
				if((!xmlStrcmp(xmlGetProp(cur_node,"login"),(const xmlChar *)login)))
				{
					strncpy(buf,xmlGetProp(cur_node,"view"),strlen(xmlGetProp(cur_node,"view")));
					strncat(buf,topic_id,5);
					strncat(buf," ",1);
					xmlSetProp(cur_node,"view",buf);
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
		char buf[256];
		bzero(buf,256);
		for (cur_node = a_node; cur_node; cur_node = cur_node->next)
		{
			if (cur_node->type == XML_ELEMENT_NODE) {
				if((!xmlStrcmp(cur_node->name,(const xmlChar *)"user")))
				{
					if((!xmlStrcmp(xmlGetProp(cur_node,"login"),(const xmlChar *)login)))
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
void search_online_user(xmlNode * a_node,void *in)
{
	xmlNode *cur_node = NULL;
	char buf[256];
	struct sockParams *sp = (struct sockParams *)in;
	for (cur_node = a_node; cur_node; cur_node = cur_node->next)
	{
		if (cur_node->type == XML_ELEMENT_NODE) {
			if((!xmlStrcmp(cur_node->name,(const xmlChar *)"user")))
			{
				if((!xmlStrcmp(xmlGetProp(cur_node,"stat"),(const xmlChar *)"on")))
				{
					//n = write(newsockfd,xmlGetProp(cur_node,"login"),strlen(xmlGetProp(cur_node,"login")));
					if(sendto(sp->sock,xmlGetProp(cur_node,"login"), strlen(xmlGetProp(cur_node,"login")),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
					{
						perror("ERROR writing to socket");
					    exit(1);
					}
					if(recvfrom(sp->sock,buf,BUFLEN,0,(struct sockaddr *) &(sp->si_other),&(sp->slen)) == -1)
					{
					   	perror("ERROR reading from socket");
					   	exit(1);
					}
				}
			}
		}
		search_online_user(cur_node->children,sp);
	}
}
void add_new_topic(xmlNode * a_node,void *in,char *login)
{
	int i;
	char buffer[256];
	char topic_name[256];
	char post_name[256];
	char post_message[256];
	char id[6];
	struct sockParams *sp = (struct sockParams *)in;

	//n = write(newsockfd,"Print topic name\n ",18);
	if(sendto(sp->sock,"Print topic name\n ", 18,0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
	{
		perror("ERROR writing to socket");
	    exit(1);
	}
	bzero(buffer,256);
	bzero(topic_name,256);
	if(recvfrom(sp->sock,buffer,BUFLEN,0,(struct sockaddr *) &(sp->si_other),&(sp->slen)) == -1)
	{
	   	perror("ERROR reading from socket");
	   	exit(1);
	}
	if(sendto(sp->sock,"next", strlen("next"),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
	{
		perror("ERROR writing to socket");
	    exit(1);
	}
	bzero(buffer,256);
	if(recvfrom(sp->sock,buffer,BUFLEN,0,(struct sockaddr *) &(sp->si_other),&(sp->slen)) == -1)
	{
		perror("ERROR reading from socket");
	    exit(1);
	}
	strcpy(topic_name,buffer);
	topic_name[strlen(topic_name)-1]=0;
	//n = write(newsockfd,"Print post name\n ",18);
	if(sendto(sp->sock,"Print post name\n ", 18,0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
	{
		perror("ERROR writing to socket");
		exit(1);
	}
	bzero(buffer,256);
	bzero(post_name,256);
	if(recvfrom(sp->sock,buffer,BUFLEN,0,(struct sockaddr *) &(sp->si_other),&(sp->slen)) == -1)
	{
	   	perror("ERROR reading from socket");
	   	exit(1);
	}
	if(sendto(sp->sock,"next", strlen("next"),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
	{
		perror("ERROR writing to socket");
	    exit(1);
	}
	bzero(buffer,256);
	if(recvfrom(sp->sock,buffer,BUFLEN,0,(struct sockaddr *) &(sp->si_other),&(sp->slen)) == -1)
	{
		perror("ERROR reading from socket");
	    exit(1);
	}
	strcpy(post_name,buffer);
	post_name[strlen(post_name)-1]=0;
	//n = write(newsockfd,"Print message\n ",18);
	if(sendto(sp->sock,"Print message\n ", 18,0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
	{
		perror("ERROR writing to socket");
		exit(1);
	}
	bzero(buffer,256);
	bzero(post_message,256);
	if(recvfrom(sp->sock,buffer,BUFLEN,0,(struct sockaddr *) &(sp->si_other),&(sp->slen)) == -1)
	{
		perror("ERROR reading from socket");
		exit(1);
	}
	if(sendto(sp->sock,"next", strlen("next"),0,(struct sockaddr *) &(sp->si_other),sp->slen) == -1)
	{
		perror("ERROR writing to socket");
	    exit(1);
	}
	if(recvfrom(sp->sock,buffer,BUFLEN,0,(struct sockaddr *) &(sp->si_other),&(sp->slen)) == -1)
	{
		perror("ERROR reading from socket");
	    exit(1);
	}
	strcpy(post_message,buffer);
	post_message[strlen(post_message)-1]=0;
	bzero(id,6);
	for(i=0;i<5;i++)
		id[i]=(rand()%10+'0');

	printf("new topic is login-%s-topics-%s-post-%s-message-%s-id-%s-\n",
			login,topic_name,post_name,post_message,id);
	add_topic(a_node,login,topic_name,post_name,post_message,id);
}

void add_topic(xmlNode * a_node,char *login, char *topic, char *post, char *text, char *id)
{
	xmlNode *cur_node = NULL;
			char buf[256];
			bzero(buf,256);
			for (cur_node = a_node; cur_node; cur_node = cur_node->next)
			{
				if (cur_node->type == XML_ELEMENT_NODE) {
					if((!xmlStrcmp(cur_node->name,(const xmlChar *)"topic")))
						if((!xmlStrcmp(xmlGetProp(cur_node,"name"),(const xmlChar *)topic)))
						{
							printf("FIND\n");
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
