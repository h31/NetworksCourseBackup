#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void print_topic_names(xmlNode * a_node, char* mess,int newsockfd);
void print_topic(char *buffer,int newsockfd);
void login_func(int newsockfd,char *log);
void search_user(xmlNode * a_node, char *login, char *password, char *id);
void search_new_topics(char *name,int newsockfg);
void search_topics_by_name(xmlNode * a_node, char *name, char *buffer);
void fnp_post_by_id(xmlNode * a_node, char *id,int newsockfd);
void fnp_post_name_by_id(xmlNode * a_node, char *id,int newsockfd);
void search_all_post(xmlNode * a_node, char all_posts[255][6],int *j);
void add_see_topic(xmlNode * a_node, char *login, char *topic_id);
void on_off_user(xmlNode * a_node, char *login,int on_off);
void search_online_user(xmlNode * a_node,int newsockfd);
void add_new_topic(xmlNode * a_node,int newsockfd,char *login);
void add_topic(xmlNode * a_node,char *login, char *topic, char *post, char *text, char *id);
void start_work(int newsockfd);
void startThread(void *in);

struct sockParams
{
	int sockfd, newsockfd, port_number, client,countnumber;
	struct sockaddr_in serv_addr, cli_addr;
};

struct userThread
{
	char login[5][100];
	int count;
};

struct userThread usersthr;

void workMainTh(void *in)
{
	char buffer[100];
	int i;

	pthread_t *t = (pthread_t *)in;
	while(1)
	{
		fgets(buffer,99,stdin);
		for(i=0;i<5;i++)
		{
			if(strcmp(buffer,usersthr.login[i]))
				  pthread_cancel(t[i]);
		}
	}
}

int main( int argc, char *argv[] )
{

	pthread_t thread[5],mainthread;
	int i,j;
	struct sockParams sp;

	usersthr.count=0;
	i=0;
	j=0;
    /* First call to socket() function */
    sp.sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sp.sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }
    /* Initialize socket structure */
    bzero((char *) &sp.serv_addr, sizeof(sp.serv_addr));
    sp.port_number = 5001;
    sp.serv_addr.sin_family = AF_INET;
    sp.serv_addr.sin_addr.s_addr = INADDR_ANY;
    sp.serv_addr.sin_port = htons(sp.port_number);

    /* Now bind the host address using bind() call.*/
    if (bind(sp.sockfd, (struct sockaddr *) &sp.serv_addr,
                          sizeof(sp.serv_addr)) < 0)
    {
         perror("ERROR on binding");
         exit(1);
    }

    /* Now start listening for the clients, here process will
    * go in sleep mode and will wait for the incoming connection
    */
    while(1)
    {
    	listen(sp.sockfd,5);
    	sp.client = sizeof(sp.cli_addr);

    /* Accept actual connection from the client */
    	sp.newsockfd = accept(sp.sockfd, (struct sockaddr *)&sp.cli_addr,

                                &sp.client);
		if (sp.newsockfd < 0)
		{
			perror("ERROR on accept");
			exit(1);
		}
		/* If connection is established then start communicating */
		sp.port_number=i;
		pthread_create(&thread[i],NULL,startThread,(void*) &sp);
		//start_work(sp.newsockfd);
		i++;
    }
    for(j=0;j<5;j++)
    	pthread_join(thread[j],NULL);
    return 0;
}

void startThread(void *in)
{
	struct sockParams *sp = (struct sockParams *)in;
	start_work(sp->newsockfd);
}

void start_work(int newsockfd)
{

	int  n;
	xmlDoc         *doc = NULL;
	xmlNode        *root_element = NULL;
	const char     *TopicFilename = "topics.xml";
	const char     *UserFilename = "users.xml";
	char buffer[256];
	char id[6];
	char login[100];

	bzero(login,100);
	    login_func(newsockfd,login);
	    strcpy(usersthr.login[usersthr.count],login);
	    usersthr.count++;
	    bzero(buffer,256);
	    strcpy(buffer,"next\n");
	    n=write(newsockfd,buffer,7);

	    while(1)
	    {
	    	bzero(buffer,256);
	    	strcpy(buffer,"next\n");
			n=write(newsockfd,buffer,7);
	    	//printf("next\n");
			bzero(buffer,256);
			n = read( newsockfd,buffer,255 );
			//printf("Buffer :%s\n",buffer);
			if (n < 0)
			{
				perror("ERROR reading from socket");
				exit(1);
			}
			if(!strncmp(buffer,"topics",6))
			{
				print_topic(buffer,newsockfd);
				//n = write(newsockfd,buffer,255);
			}
			if(!strncmp(buffer,"online",6))
			{
				doc = xmlReadFile(UserFilename, NULL, 0);
				if (doc == NULL)
				{
					printf("error: could not parse file %s\n", UserFilename);
					n = write(newsockfd,"Error\n",6);
				}
				else
				{
				    root_element = xmlDocGetRootElement(doc);
				    search_online_user(root_element,newsockfd);
				    xmlFreeDoc(doc);
				}
				xmlCleanupParser();
			}
			if(!strncmp(buffer,"show",4))
			{
				bzero(id,6);
				for(n=5;n<10;n++)
					id[n-5]=buffer[n];
			    doc = xmlReadFile(TopicFilename, NULL, 0);
			    if (doc == NULL)
			    {
			        printf("error: could not parse file %s\n", TopicFilename);
			        n = write(newsockfd,"Error\n",6);
			    }
			    else
			    {
	                root_element = xmlDocGetRootElement(doc);
	                fnp_post_by_id(root_element,id,newsockfd);
	                xmlFreeDoc(doc);
	            }
		        xmlCleanupParser();

		        doc = xmlReadFile(UserFilename, NULL, 0);
		        if (doc == NULL)
		        {
		             printf("error: could not parse file %s\n", UserFilename);
		             n = write(newsockfd,"Error\n",6);
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
			if(!strncmp(buffer,"exit",4))
			{
				bzero(buffer,256);
				strncpy(buffer,"exit\n",6);
				n = write(newsockfd,buffer,255);
				doc = xmlReadFile(UserFilename, NULL, 0);
				if (doc == NULL)
				{
					printf("error: could not parse file %s\n", UserFilename);
					n = write(newsockfd,"Error\n",6);
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
					n = write(newsockfd,"Error\n",6);
				}
				else
				{
				    root_element = xmlDocGetRootElement(doc);
				    add_new_topic(root_element,newsockfd,login);
				    xmlSaveFile(TopicFilename,doc);
					xmlFreeDoc(doc);
				}
				xmlCleanupParser();
			}
			else
			{
				n=write(newsockfd,"OK",2);
			}
			/* Write a response to the client */
			if (n < 0)
			{
				perror("ERROR writing to socket");
				exit(1);
			}
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
                  //strncat(buffer,"Error\n",6);
                  n = write(newsockfd,"Error\n",6);
          }
        else
          {

                  root_element = xmlDocGetRootElement(doc);
                 // strncat(buffer,"Topics:\n",10);
                  write(newsockfd,"Topics:\n",10);
                  print_topic_names(root_element,buffer,newsockfd);
                  xmlFreeDoc(doc);
          }

        xmlCleanupParser();

        return;

}

void print_topic_names(xmlNode * a_node, char *mess, int newsockfd)
{
    xmlNode *cur_node = NULL;
    char buf[256];
    bzero(buf,256);

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if((!xmlStrcmp(cur_node->name,(const xmlChar *)"topic")))
            {
            	strncpy(buf,xmlGetProp(cur_node,"name"),strlen(xmlGetProp(cur_node,"name")));
            	strcat(buf,"\n");
            	write(newsockfd,buf,strlen(buf));
            	read(newsockfd,buf,255);
			}
            if((!xmlStrcmp(cur_node->name,(const xmlChar *)"post")))
            {
                strcpy(buf,"	");
            	strncat(buf,xmlGetProp(cur_node,"id"),strlen(xmlGetProp(cur_node,"id")));
            	strcat(buf,"	");
            	strncat(buf,xmlGetProp(cur_node,"name"),strlen(xmlGetProp(cur_node,"name")));
                strcat(buf,"\n");
                        	write(newsockfd,buf,strlen(buf));
                        	read(newsockfd,buf,255);
            			}
        }

        print_topic_names(cur_node->children,mess,newsockfd);
    }
}

void login_func(int newsockfd,char * log)
{
	char login[256];
	char password[256];
	int n;
	char id[256];

	xmlDoc         *doc = NULL;
	xmlNode        *root_element = NULL;
	const char     *Filename = "users.xml";
	doc = xmlReadFile(Filename, NULL, 0);

	n = write(newsockfd,"Hello, print you login and password\n",36);
	bzero(login,256);
	bzero(password,256);
	n = read( newsockfd,login,255 );
	if (n < 0)
	{
		perror("ERROR reading from socket");
		exit(1);
	}
	n=write(newsockfd,"OK",2);
	n = read( newsockfd,password,255 );
	if (n < 0)
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
	    	write(newsockfd,"OK",5);
	    	login_func(newsockfd,login);
	    }
	    else
	    {
	    	write(newsockfd,"next",5);
	    	search_new_topics(login,newsockfd);
	    	root_element = xmlDocGetRootElement(doc);
	    	on_off_user(root_element,login,1);
	    	xmlSaveFile(Filename,doc);
	    	strcpy(log,login);
	    	write(newsockfd,"next",5);
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

void search_new_topics(char *name, int newsockfg)
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

void fnp_post_by_id(xmlNode * a_node, char *id,int newsockfd)
{
	xmlNode *cur_node = NULL;
	char buf[256];
	bzero(buf,256);
			for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
				if (cur_node->type == XML_ELEMENT_NODE) {
					if((!xmlStrcmp(cur_node->name,(const xmlChar *)"post")))
					{
						if((!xmlStrcmp(xmlGetProp(cur_node,"id"),(const xmlChar *)id)))
						{
							strncpy(buf,xmlGetProp(cur_node,"text"),strlen(xmlGetProp(cur_node,"text")));
							strcat(buf,"\n");
							write(newsockfd,buf,strlen(buf));
							//printf("%s\n",xmlGetProp(cur_node,"text"));
							read(newsockfd,buf,255);
							return;
						}
					}
			    }
				fnp_post_by_id(cur_node->children,id,newsockfd);
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

void fnp_post_name_by_id(xmlNode * a_node, char *id,int newsockfd)
{
	xmlNode *cur_node = NULL;
	char buf[256];
	bzero(buf,256);
			for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
				if (cur_node->type == XML_ELEMENT_NODE) {
					if((!xmlStrcmp(cur_node->name,(const xmlChar *)"post")))
					{
						if((!xmlStrcmp(xmlGetProp(cur_node,"id"),(const xmlChar *)id)))
						{
							strncpy(buf,xmlGetProp(cur_node,"name"),strlen(xmlGetProp(cur_node,"name")));
							strcat(buf,"\n");
							write(newsockfd,buf,strlen(buf));
							//printf("%s\n",xmlGetProp(cur_node,"text"));
							read(newsockfd,buf,255);
							return;
						}
					}
			    }
				fnp_post_by_id(cur_node->children,id,newsockfd);
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
void search_online_user(xmlNode * a_node,int newsockfd)
{
	xmlNode *cur_node = NULL;
	char buf[256];
			for (cur_node = a_node; cur_node; cur_node = cur_node->next)
			{
				if (cur_node->type == XML_ELEMENT_NODE) {
					if((!xmlStrcmp(cur_node->name,(const xmlChar *)"user")))
					{
						if((!xmlStrcmp(xmlGetProp(cur_node,"stat"),(const xmlChar *)"on")))
						{
							write(newsockfd,xmlGetProp(cur_node,"login"),strlen(xmlGetProp(cur_node,"login")));
							read(newsockfd,buf,255);
						}
					}
			    }
				search_online_user(cur_node->children,newsockfd);
			}
}
void add_new_topic(xmlNode * a_node,int newsockfd,char *login)
{
	int i;
	char buffer[256];
	char topic_name[256];
	char post_name[256];
	char post_message[256];
	char id[6];

	write(newsockfd,"Print topic name\n ",18);
	bzero(buffer,256);
	bzero(topic_name,256);
	read( newsockfd,buffer,255 );
	strcpy(topic_name,buffer);
	topic_name[strlen(topic_name)-1]=0;

	write(newsockfd,"Print post name\n ",18);
	bzero(buffer,256);
	bzero(post_name,256);
	read( newsockfd,buffer,255 );
	strcpy(post_name,buffer);
	post_name[strlen(post_name)-1]=0;

	write(newsockfd,"Print message\n ",18);
	bzero(buffer,256);
	bzero(post_message,256);
	read( newsockfd,buffer,255 );
	strcpy(post_message,buffer);
	post_message[strlen(post_message)-1]=0;

	bzero(id,6);
	for(i=0;i<5;i++)
		id[i]=(rand()%10+'0');

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
