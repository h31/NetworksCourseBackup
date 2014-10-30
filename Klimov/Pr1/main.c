

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlwriter.h>

#include <sys/stat.h>
#include <unistd.h>

#define MY_ENCODING "ISO-8859-1"

//#include "tinyxml/tinyxml.h"

void print_topic_names(xmlNode * a_node, char *mess)
{
    xmlNode *cur_node = NULL;
    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            //if((!xmlStrcmp(cur_node->name,(const xmlChar *)"topic"))){
        	if((!xmlStrcmp(cur_node->name,(const xmlChar *)"PowerScheme"))){
            	//strncat(mess, xmlGetProp(cur_node,"name"),15);
            	//strncat(mess, "\n",15);
            	//strncat(mess, cur_node->name,15);
            	strncat(mess, xmlNodeGetContent(cur_node),15);
            	//xmlNodeGetContent(cur_node);
            	strncat(mess, "\n",15);
            }
        }

        print_topic_names(cur_node->children,mess);
    }
}

void create_xml(char *fileName){
	xmlTextWriterPtr writer;
	int rc;
	//xmlDocPtr doc;
	//xmlNodePtr node;
	//xmlChar *tmp;
	//writer = xmlNewTextWriterFilename("test.xml", 0);
	writer = xmlNewTextWriterFilename(fileName, 0);
	rc = xmlTextWriterStartDocument(writer, NULL, "UTF-8", NULL);
	rc = xmlTextWriterStartElement(writer, "Powersettings");
		rc = xmlTextWriterWriteElement(writer, "PowerScheme", "Testing");
		rc = xmlTextWriterEndElement(writer);
		rc = xmlTextWriterWriteElement(writer, "CPUSpeed", "Adaptive");
		rc = xmlTextWriterEndElement(writer);
	/* ... etc ... */
	rc = xmlTextWriterEndElement(writer);
	rc = xmlTextWriterEndDocument(writer);
	xmlFreeTextWriter(writer);
	//xmlSaveFileEnc(file, doc, "UTF-8");
	//xmlFreeDoc(doc);
}

char *newStr(char *charBuffer) {
  int length = strlen(charBuffer);
  char *str;
  if (length <= 1) {
    str = (char *) malloc(1);
    str[0] = '\0';
  } else {
    str = (char *) malloc(length);
    strcpy(str, &charBuffer[1]);
  }
  return str;
}

char *get_mes(char *str){
	char qolon = '#';
	char *quotPtr = strchr(str, qolon);
	if(quotPtr == NULL)
	{
	  // Handle error
	}
	int position = quotPtr - str;
	char* attrValue = (char*) malloc((position + 1) * sizeof(char));
	memcpy(attrValue, str, position);
	attrValue[position] = '\0';
	return attrValue;
}

void removeSubstring(char *s,const char *toremove)
{
  while( s=strstr(s,toremove) )
    memmove(s,s+strlen(toremove),1+strlen(s+strlen(toremove)));
}

void find_char(char *mes){
	char qolon = '#';
	char *quotPtr = strchr(mes, qolon);
	if(quotPtr == NULL)
	{
	  // Handle error
	}
	int position = quotPtr - mes;
	char* attr = get_mes(mes);
	printf("mes: %s\n", attr);
	strncat(attr, "#",15);
	removeSubstring(mes, attr);
	removeSubstring(mes, "#");
	printf("message: %s\n", mes);
}

char *get_1st_arg(char *mes){
	printf("1st arg: %s\n", get_mes(mes));
	return get_mes(mes);
}

char *get_2nd_arg(char *mes){
	char* attr = get_mes(mes);
	//printf("mes: %s\n", attr);
	strncat(attr, "#",15);
	removeSubstring(mes, attr);


	char qolon = '#';
	char *quotPtr = strchr(mes, qolon);
	if(quotPtr == NULL)
	{
	  // Handle error
	}
	removeSubstring(mes, "#");
	int position = quotPtr - mes;

	mes[position] = '\0';
	printf("2nd arg: %s\n", mes);
	return mes;
}

char *get_mes_from_client(int newsockfd, char *buffer){
    int n;
    //bzero(buffer,256);
    n = read(newsockfd, buffer, 255);
    if (n < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }
    return buffer;
}

void create_file(char *to, char *from, char *msg){
	int rc;
	xmlTextWriterPtr writer;
	xmlDocPtr doc;
	xmlNodePtr node;
	xmlChar *tmp;

	doc = xmlNewDoc(BAD_CAST XML_DEFAULT_VERSION);
	node = xmlNewDocNode(doc, NULL, BAD_CAST "inbox", NULL);
	xmlDocSetRootElement(doc, node);
	writer = xmlNewTextWriterTree(doc, node, 0);
	rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
	rc = xmlTextWriterStartElement(writer, BAD_CAST "mes");
	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "id", BAD_CAST "1");
	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "from", BAD_CAST from);
	rc = xmlTextWriterEndAttribute(writer);
	rc = xmlTextWriterWriteString(writer, (const xmlChar*) msg);
	rc = xmlTextWriterEndElement(writer);
	rc = xmlTextWriterEndDocument(writer);
	xmlFreeTextWriter(writer);

	xmlSaveFileEnc(to, doc, MY_ENCODING);

	xmlFreeDoc(doc);

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
    portno = 5001;
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
    /* If connection is established then start communicating */

    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;
    //const char *fileName = "test1.xml";
    const char *fileName = "myxml.xml";
    doc = xmlReadFile(fileName, NULL, 0);
    if (doc == NULL){
    	printf("error: could not parse file %s\n", fileName);
        strncat(buffer,"Error\n",6);
     }
     else
     {
    	 root_element = xmlDocGetRootElement(doc);
         //strncat(buffer,"Topics:\n",10);
    	 bzero(buffer,256);
         print_topic_names(root_element,buffer);
         xmlFreeDoc(doc);
     }
    xmlCleanupParser();

    printf("test: %s\n", buffer);


    int i, k = 0, m = 0;
    FILE *fp;

    n = write(newsockfd, "Please type ur e-mail address\n", 30);
    bzero(buffer,256);
    n = read( newsockfd,buffer,255 );
    if (n < 0)
    {
    	perror("ERROR reading from socket");
        exit(1);
    }

    //find_char(buffer);
    char *first = get_1st_arg(buffer);
    char *second = get_2nd_arg(buffer);
    //if(!strcmp(get_2nd_arg(buffer), "send")){
    //printf("res: %d\n", strcmp(first, "Serg"));
    //printf("res: %d\n", strcmp(second, "send"));
    //exit(1);
    if(strcmp(second, "send") == 0){
    	char * to_send;
    	to_send = get_mes_from_client(newsockfd, buffer);
    	char *mail_to = get_1st_arg(to_send);
    	char *message = get_2nd_arg(to_send);
    	//create_file(get_1st_arg(to_send), first, get_2nd_arg(to_send));
    	create_file(mail_to, first, message);
    	//printf("to: %s\n", get_1st_arg(to_send));
    	//printf("message: %s\n", get_2nd_arg(to_send));
    }else if (!strcmp(get_2nd_arg(buffer), "readinbox")) {
    	printf("readinbox");
	}
    else{
    	printf("wrong command");
    }
    fp = fopen(buffer, "rb");
    if(fp){
    	//already in base
    	printf("E-mail in base: %s\n", buffer);
    }
    strncpy(name, buffer, 256);
    bzero(buffer,256);
    n = write(newsockfd, "Please type the e-mail address of the recipient\n", 48);

    bzero(buffer,256);
    n = read( newsockfd,buffer,255 );
    if (n < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }

    printf("E-mail: %s\n", buffer);



    for(i = 0; i < 256; i++)
    	if(buffer[i] == '@')
    		k++;
    if(k > 0){
    	fp = fopen(buffer, "ab+");


    	fprintf(fp, "<%s>\n", name);
    	n = write(newsockfd,"OK\nPlease, write a message:\n",29);
    	bzero(buffer,256);
    	n = read( newsockfd,buffer,255 );
    	fprintf(fp, "%s\n", buffer);
    	fprintf(fp, "</%s>\n", name);
    	printf("Message: %s\n", buffer);

    }
    else
    	n = write(newsockfd,"Wrong e-mail address\n",21);

    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }
    return 0;
}


