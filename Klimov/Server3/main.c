#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#include <assert.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlmemory.h>
#include <libxml/xpath.h>

#define BUFLEN 512  //Max length of buffer
#define MY_PORT 5001   //The port on which to listen for incoming data

void die(char *s)
{
    perror(s);
    exit(1);
}

char** splito_array(char* str){
	//char    str[]= "ls -l";
	char ** res  = NULL;
	char *  p    = strtok (str, "#");
	int n_spaces = 0, i;


	/* split string and append tokens to 'res' */

	while (p) {
	  res = realloc (res, sizeof (char*) * ++n_spaces);

	  if (res == NULL)
	    exit (-1); /* memory allocation failed */

	  res[n_spaces-1] = p;

	  p = strtok (NULL, "#");
	}

	/* realloc one extra element for the last NULL */

	res = realloc (res, sizeof (char*) * (n_spaces+1));
	res[n_spaces] = 0;

	/* free the memory allocated */

	free (res);
	return res;
}

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

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

void do_send(char* from, char* to, char* mes){
	printf("Send command\n");
	create_file(to, from, mes);
}

void do_read(int newsockfd, char *buffer, char* client, struct sockaddr_in si_other, int slen){
	printf("Read command\n");
	print_mesgs(buffer, newsockfd, client, si_other, slen);
}

void read_mes(xmlNode* node, char* mess, int s, struct sockaddr_in si_other, int slen){
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

                if (sendto(s, buf, sizeof(buf),
                                              		0 , (struct sockaddr *) &si_other, slen)==-1)
                                                      {
                                                          die("sendto()");
                                                      }

                                                      //receive a reply and print it
                                                      //clear the buffer by filling null, it might have previously received data
                                                      //memset(buf,'\0', BUFLEN);
                                                      //try to receive some data, this is a blocking call
                                                      if (recvfrom(s, buf, sizeof(buf),
                                                      		0, (struct sockaddr *) &si_other, &slen) == -1)
                                                      {
                                                      	//puts("qq");
                                                          //die("recvfrom()");
                                                      }

	        }
        }

	    read_mes(cur_node->children,mess,s, si_other, slen);
    }
}

void print_mesgs(char* buffer ,int s, char* user, struct sockaddr_in si_other, int slen)
{

        xmlDoc         *doc = NULL;
        xmlNode        *root_element = NULL;
        int n;
        doc = xmlReadFile(user, NULL, 0);
        if (doc == NULL)
          {
                  printf("error: could not parse file %s\n", user);
          }
        else
          {

                  root_element = xmlDocGetRootElement(doc);

                  if (sendto(s, "Messages:\n", sizeof("Messages:\n"),
                              		0 , (struct sockaddr *) &si_other, slen)==-1)
                                      {
                                          die("sendto()");
                                      }

                                      //receive a reply and print it
                                      //clear the buffer by filling null, it might have previously received data
                                      //memset(buf,'\0', BUFLEN);
                                      //try to receive some data, this is a blocking call
                                      if (recvfrom(s, "Messages:\n", sizeof("Messages:\n"),
                                      		0, (struct sockaddr *) &si_other, &slen) == -1)
                                      {
                                      	//puts("qq");
                                          //die("recvfrom()");
                                      }


                  //n = write(newsockfd,"Messages:\n",11);
                  read_mes(root_element,buffer,s, si_other, slen);
                  //n = write(newsockfd,"end",3);

                  if (sendto(s, "allmesgs", sizeof("allmesgs"),
                                                		0 , (struct sockaddr *) &si_other, slen)==-1)
                                                        {
                                                            die("sendto()");
                                                        }

                                                        //receive a reply and print it
                                                        //clear the buffer by filling null, it might have previously received data
                                                        //memset(buf,'\0', BUFLEN);

                  if (recvfrom(s, "allmesgs", sizeof("allmesgs"),
                                                                        		0, (struct sockaddr *) &si_other, &slen) == -1)
                                                                        {
                                                                        	//puts("qq");
                                                                            //die("recvfrom()");
                                                                        }

                  xmlFreeDoc(doc);
          }

        xmlCleanupParser();

        return;

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

int get_cmd(char* buffer, int num){
	int cmd;
	char **temp;
	temp=str_split(buffer, '#');

	        	if(temp){
	        		int i;
	        	    for (i = 0; *(temp + i); i++)
	        	    {
	        	    	printf("inp :%s\n", *(temp + i));
	        	        if(i == num)
	        	        	cmd = atoi(*(temp + i));
	        	        free(*(temp + i));
	        	    }
	        	    free(temp);
	        	}
	return cmd;
}

char* get_cmd_ch(char* buffer, int num, char* res){
	char* cmd;
	char **temp;
	char *teeemp;
	temp=str_split(buffer, '#');

	        	if(temp){
	        		int i;
	        	    for (i = 0; *(temp + i); i++)
	        	    {

	        	        if(i == num)
	        	        	cmd = (*(temp + i));

	        	        strcpy(res, (*(temp + i)));
	        	        free(*(temp + i));
	        	    }
	        	    free(temp);
	        	}
	return cmd;
}

int use_token(char* str, int num, char* res){
	const char s[2] = "#";
	char *token;
	int i = 0;

	token = strtok(str, s);
	if(num==0){
		res = token;
		return 0;
	}
	while(token != NULL){
		printf("i:%d, t:%s", i, token);
		i++;
		token = strtok(NULL, s);
		if(num==i){
			res = token;
			return 0;
		}
	}
	return 0;
}

int main(void)
{
    struct sockaddr_in si_me, si_other;

    int s, i, slen = sizeof(si_other) , recv_len;
    char buf[BUFLEN];

    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(MY_PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }

    //keep listening for data

socklen_t slen_o = sizeof(si_other);
int sa = sizeof("Commands: 1#kitty#Hello# - Send message 'Hello' to user kitty;\n"
		"2# - read ur inbox; 3# - exit\n");
int k=0;
char* arg[4];
char* arg_tmp[4];
//char user_name[10]="nnm";
char* user_name;
char* send_to;
char* mess_to;
char buf_tmp[BUFLEN];
char buf_tmp2[BUFLEN];
char buf_tmp3[BUFLEN];
char **qq;

    while(1)
    {
    	bzero(buf, sizeof(buf));
        //printf("Waiting for data...");
        fflush(stdout);

        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen_o)) == -1)
        {
            die("recvfrom()");
        }



        //now reply the client with the same data
        if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen_o) == -1)
        {
            die("sendto()");
        }


        strcpy(buf_tmp,buf);
        int buu = 0;
        buu = get_cmd(buf,0);
        //printf("b = %d", buu);

        if(strcmp(buf, "Hello") == 0){
            if (sendto(s, "Enter ur name ending with '#'\n", sizeof("Enter ur name ending with '#'\n"),
            		0 , (struct sockaddr *) &si_other, slen_o)==-1)
                    {
                        die("sendto()");
                    }

                    //receive a reply and print it
                    //clear the buffer by filling null, it might have previously received data
                    //memset(buf,'\0', BUFLEN);
                    //try to receive some data, this is a blocking call
                    if (recvfrom(s, "Enter ur name ending with '#'\n", sizeof("Enter ur name ending with '#'\n"),
                    		0, (struct sockaddr *) &si_other, &slen_o) == -1)
                    {
                    	//puts("qq");
                        //die("recvfrom()");
                    }
                    if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen_o)) == -1)
                            {
                                die("recvfrom()");
                            }

                            //print details of the client/peer and the data received
                            //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
                            //printf("Data: %s\n" , buf);

                            //now reply the client with the same data
                            if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen_o) == -1)
                            {
                                die("sendto()");
                            }


                            bzero(user_name, sizeof(user_name));
                            get_cmd_ch(buf, 0, user_name);

                            printf("User: %s\n", user_name);
                            bzero(buf, sizeof(buf));
        }
        else if(strcmp(buf, "Start") == 0){
        	if (sendto(s, "Commands: 1#kitty#Hello# - Send message 'Hello' to user kitty;\n"
        	            		"2# - read ur inbox; 3# - exit\n", sa , 0 , (struct sockaddr *) &si_other, slen_o)==-1)
        	                    {
        	                        die("sendto()");
        	                    }

        	                    //receive a reply and print it
        	                    //clear the buffer by filling null, it might have previously received data
        	                    //memset(buf,'\0', BUFLEN);
        	                    //try to receive some data, this is a blocking call
        	                    if (recvfrom(s, "Commands: 1#kitty#Hello# - Send message 'Hello' to user kitty;\n"
        	        		    		"2# - read ur inbox; 3# - exit\n", sa, 0, (struct sockaddr *) &si_other, &slen_o) == -1)
        	                    {
        	                    	//puts("qq");
        	                        //die("recvfrom()");
        	                    }
        	                    bzero(buf, sizeof(buf));

        }
        else if(buu < 4){

        	puts("OBRABOTKA\n");

        	qq = splito_array(buf_tmp);

        	if(buu!=0){
        		if (buu==1){
        			printf("name: %s, to: %s, mes: %s\n", user_name, qq[1], qq[2]);
        			do_send(user_name, qq[1], qq[2]);
        			//puts("com 1");
        		}
        		else if (buu==2){
        			printf("Client name: %s\n", user_name);
        			do_read(s, buf, user_name, si_other, slen);
        			//puts("com 2");
        		}
        		else if (buu==3){
        			/*if(strcmp(user,"root") == 0){
        				do_exit(user);
        				exit(1);
        			}
        			else
        				do_exit(user);*/
        			//puts("com 3");
        		}
        		else {
        			puts("Wrong command!\n");
        		}
        	}


        	//printf("Data: %s\n" , buf);
        	bzero(buf, sizeof(buf));
        }
        bzero(buf, sizeof(buf));
    }

    close(s);
    return 0;
}
