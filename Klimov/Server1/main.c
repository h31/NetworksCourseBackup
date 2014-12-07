
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlmemory.h>
#include <libxml/xpath.h>

#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define MY_ENCODING "ISO-8859-1"
#define MY_PORT 5001

void get_args(char* inp_str, char* arg[]){//buffer and array to write
	int i = 0, j;
	const char s[2] = "#";
	char *token;
	token = strtok(inp_str, s);
	arg[i] = token;
	while(token != NULL){
		i++;
		token = strtok(NULL, s);
		arg[i] = token;
	}
	i--;
	arg[i] = NULL;
}

char *get_mes_from_client(int newsockfd, char *buffer){
    int n;
    bzero(buffer,256);
    n = read(newsockfd, buffer, 255);
    if (n < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }
    return buffer;
}

char* get_hello(int newsockfd, char *buffer, char* arg[]){
	int i,j=0;
	char user[10] = "no user";
	if((arg[0]!=NULL)&&(arg[1]!=NULL)){
		strcpy(user, arg[0]);
		char command[7];
		strcpy(command, arg[1]);
		j = atoi(command);
		printf("j=%d\n",j);
		if (j==1){
			do_send(user, arg[2], arg[3]);
		}
		else if (j==2){
			do_read(newsockfd, buffer, user);
		}
		else if (j==3){
			do_exit();
		}
		else {
			puts("Wrong command!\n");
		}
	}
	printf("user: %s\n", user);
	return user;
}

void do_send(char* from, char* to, char* mes){
	printf("Send command\n");
	create_file(to, from, mes);
}

void do_read(int newsockfd, char *buffer, char* client){
	printf("Read command\n");
	print_mesgs(buffer, newsockfd, client);
}

void do_exit(){
	printf("Exit command\n");
	exit(1);
}

void do_serve(int newsockfd, char *buffer, char* arg[], char* user){
	int i,j=0;
	if(arg[0]!=NULL){
		char command[7];
		strcpy(command, arg[0]);
		j = atoi(command);
		printf("j=%d\n",j);
		if (j==1){
			do_send(user, arg[1], arg[2]);
		}
		else if (j==2){
			do_read(newsockfd, buffer, user);
		}
		else if (j==3){
			do_exit();
		}
		else {
			puts("Wrong command!\n");
		}
	}
}

void do_wait_mes(int newsockfd, char *buffer, char* user){
	char* inp;
	char* get_inp[4];
	while(1){
		inp=get_mes_from_client(newsockfd, buffer);
		get_args(inp, get_inp);
		do_serve(newsockfd, buffer, get_inp, user);
	}
}

void add_mes(xmlNode* node, char* from, char* msg){
	xmlNode* cur_node = NULL;
	char buf[256];
	bzero(buf,256);
	for (cur_node = node; cur_node; cur_node = cur_node->next)
	{
		if (cur_node->type == XML_ELEMENT_NODE) {
			if((!xmlStrcmp(cur_node->name,(const xmlChar *)"inbox"))){
					xmlNodePtr nNode = xmlNewNode(0,(const xmlChar *)"mes");
					xmlSetProp(nNode,(const xmlChar *)"from",(const xmlChar *)from);
					xmlSetProp(nNode,(const xmlChar *)"text",(const xmlChar *)msg);
					//xmlNodeSetContent(nNode, (xmlChar*)msg);
					xmlAddChild(cur_node,nNode);
					return;
			}
		}
		add_mes(cur_node->children, from, msg);
	}
}

void read_mes(xmlNode* node, char* mess, int newsockfd){
	xmlNode *cur_node = NULL;
	char buf[256];
	bzero(buf,256);
	int n;
    for (cur_node = node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if((!xmlStrcmp(cur_node->name,(const xmlChar *)"mes")))
            {
                strcpy(buf,"from: ");
            	strncat(buf,xmlGetProp(cur_node,"from"),strlen(xmlGetProp(cur_node,"from")));
            	strcat(buf,"	message: ");
            	strncat(buf,xmlGetProp(cur_node,"text"),strlen(xmlGetProp(cur_node,"text")));
                strcat(buf,"\n");
                n = write(newsockfd,buf,strlen(buf));
                //n=read(newsockfd,buf,255);
	        }
        }

	    read_mes(cur_node->children,mess,newsockfd);
    }
}

void print_mesgs(char* buffer ,int newsockfd, char* user)
{

        xmlDoc         *doc = NULL;
        xmlNode        *root_element = NULL;
        int n;
        doc = xmlReadFile(user, NULL, 0);
        if (doc == NULL)
          {
                  printf("error: could not parse file %s\n", user);
                  //strncat(buffer,"Error\n",6);
                  n = write(newsockfd,"Error\n",6);
          }
        else
          {

                  root_element = xmlDocGetRootElement(doc);
                  n = write(newsockfd,"Messages:\n",11);
                  read_mes(root_element,buffer,newsockfd);
                  //n = write(newsockfd,"end",3);
                  xmlFreeDoc(doc);
          }

        xmlCleanupParser();

        return;

}

void create_file(char *to, char *from, char *msg){
	int rc;
	xmlTextWriterPtr writer;
	xmlDocPtr doc;
	xmlNodePtr node, root;
	xmlChar *tmp;
	if(doc = xmlReadFile(to, NULL, 0)){
		root = xmlDocGetRootElement(doc);
		add_mes(root, from, msg);
		xmlSaveFile(to, doc);
		xmlFreeDoc(doc);
	}else{
		doc = xmlNewDoc(BAD_CAST XML_DEFAULT_VERSION);
		node = xmlNewDocNode(doc, NULL, BAD_CAST "inbox", NULL);
		xmlDocSetRootElement(doc, node);
		add_mes(node, from, msg);
		xmlSaveFile(to, doc);
		xmlFreeDoc(doc);
	}
	xmlCleanupParser();
}

int main( int argc, char *argv[] )
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    char name[256];
    struct stat st = {0};
    struct sockaddr_in serv_addr, cli_addr;
    int  n;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }
    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    //portno = 5001;
    portno = MY_PORT;
    //portno = 7771;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
                          sizeof(serv_addr)) < 0)
    {
         perror("ERROR on binding");
         exit(1);
    }

    /* Now start listening for the clients, here process will
    * go in sleep mode and will wait for the incoming connection
    */
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    /* Accept actual connection from the client */
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,
                                &clilen);
    if (newsockfd < 0)
    {
        perror("ERROR on accept");
        exit(1);
    }
    bzero(buffer,256);
    n = read( newsockfd,buffer,255 );
    if (n < 0)
    {
    	perror("ERROR reading from socket");
        exit(1);
    }
    int j;
    char* get_inp[4];
    get_args(buffer, get_inp);
    char user_name[10]="nnm";
    strcpy(user_name, get_hello(newsockfd, buffer, get_inp));
    do_wait_mes(newsockfd, buffer, user_name);
    return 0;
}
