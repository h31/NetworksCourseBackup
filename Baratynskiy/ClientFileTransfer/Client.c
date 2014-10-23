#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void sendMessage()
{

}

int main(int argc, char *argv[])
{
    int sock, port, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    long size;
    int b;
    char fileName[30];
    char commandLine[15];
    char buffer[256];
    char charSize[256];
    int i=0;
    int firstTime = 1;

    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }
    port = atoi(argv[2]);
    while(1)
    {
    	firstTime = 1;
    	printf("Enter file name: ");
    	scanf("%s",fileName);
    	puts(fileName);
    	FILE *file = fopen(fileName,"r+");
    	fseek(file,0,SEEK_END);
    	size = ftell(file);
    	sprintf(charSize,"%d",size);
    	fseek(file,0,SEEK_SET);
    	/* Create a socket point */
    	while(!feof(file)){
    		if (firstTime == 0)
    		{
    			b=fread(buffer,1,sizeof(buffer),file);
    		}
    		sock = socket(AF_INET, SOCK_STREAM, 0);
    		if (sock < 0)
    		{
    			perror("ERROR opening socket");
    			exit(1);
    		}
    		server = gethostbyname(argv[1]);
    		if (server == NULL) {
    			fprintf(stderr,"ERROR, no such host\n");
    			exit(0);
    		}

    		bzero((char *) &serv_addr, sizeof(serv_addr));
    		serv_addr.sin_family = AF_INET;

    		serv_addr.sin_port = htons(port);

    		/* Now connect to the server */
    		if (connect(sock,&serv_addr,sizeof(serv_addr)) < 0)
    		{
    			perror("ERROR connecting");
    			exit(1);
    		}
    		/* Now ask for a message from the user, this message
    		 * will be read by server
    		 */
    		if (firstTime == 1)
    		{
    			write(sock,fileName,strlen(fileName));
    			firstTime = 2;
    			n = read(sock,buffer,255);
    			if (n < 0)
    			{
    				perror("ERROR reading from socket");
    				exit(1);
    			}
    			continue;
    		}
    		else if (firstTime == 2)
    		{
    			write(sock,charSize,strlen(charSize));
    			firstTime = 0;
    			n = read(sock,buffer,255);
    			if (n < 0)
    			{
    				perror("ERROR reading from socket");
    				exit(1);
    			}
    			continue;

    		}
        	n = write(sock,buffer,strlen(buffer));
        	if (n < 0)
        	{
        		perror("ERROR writing to socket");
        		exit(1);
        	}

    /* Now read server response */
        bzero(buffer,256);
        n = read(sock,buffer,255);
        if (n < 0)
        {
        	perror("ERROR reading from socket");
        	exit(1);
        }
        printf("%s\n",buffer);
    	}
    fclose(file);
    close(sock);
    }
    return 0;
}
