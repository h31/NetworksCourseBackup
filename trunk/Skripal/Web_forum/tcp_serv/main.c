#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

void print_topic_names(xmlNode * a_node, char* mess);
void print_topic(char *buffer);

int main( int argc, char *argv[] )
{
    int sockfd, newsockfd, port_number, client;
    char buffer[256];
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
    port_number = 5001;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port_number);

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
    client = sizeof(cli_addr);

    /* Accept actual connection from the client */
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,
                                &client);
    if (newsockfd < 0)
    {
        perror("ERROR on accept");
        exit(1);
    }
    /* If connection is established then start communicating */
    n = write(newsockfd,"Ready\n",6 );
    bzero(buffer,256);
    n = read( newsockfd,buffer,255 );
    if (n < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }
    if(strcmp(buffer,"topics"))
    {
    	bzero(buffer,256);
    	print_topic(buffer);
        n = write(newsockfd,buffer,255);
    }
    /* Write a response to the client */
    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }
    return 0;
}

void print_topic(char *buffer)
{

        xmlDoc         *doc = NULL;
        xmlNode        *root_element = NULL;
        const char     *Filename = "test.xml";
        doc = xmlReadFile(Filename, NULL, 0);

        if (doc == NULL)
          {
                  printf("error: could not parse file %s\n", Filename);
                  strncat(buffer,"Error\n",6);
          }
        else
          {

                  root_element = xmlDocGetRootElement(doc);
                  strncat(buffer,"Topics:\n",10);
                  print_topic_names(root_element,buffer);

                  xmlFreeDoc(doc);;
          }

        xmlCleanupParser();

        return;

}

void print_topic_names(xmlNode * a_node, char *mess)
{
    xmlNode *cur_node = NULL;
    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if((!xmlStrcmp(cur_node->name,(const xmlChar *)"topic")))
            	strncat(mess, xmlGetProp(cur_node,"name"),15);
        }

        print_topic_names(cur_node->children,mess);
    }
}
