#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define SERVER "127.0.0.1"
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to send data

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];

    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    if (inet_aton(SERVER , &si_other.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

        //send the message
        if (sendto(s, "Start", 5 , 0 , (struct sockaddr *) &si_other, slen)==-1)
        {
            die("sendto()");
        }

while(1)
{
	bzero(buf,256);
	if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
	{
	    die("recvfrom()");
	}
	printf("%s\n",buf);

    bzero(message,256);
	printf("Print login\n");
    fgets(message,255,stdin);
    if(sendto(s,message, strlen(message),0,(struct sockaddr *) &si_other,slen) == -1)
    {
    	perror("ERROR writing to socket");
    	exit(1);
    }


    bzero(buf,256);
    if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
            {
                die("recvfrom()");
            }
    //printf("----%s----\n",buffer);

    bzero(message,256);
    printf("Print password\n");
    fgets(message,255,stdin);
    if(sendto(s,message, strlen(message),0,(struct sockaddr *) &si_other,slen) == -1)
    {
       	perror("ERROR writing to socket");
       	exit(1);
    }

    bzero(buf,256);
    if(recvfrom(s,buf,BUFLEN,0,(struct sockaddr *) &si_other,&slen) == -1)
    {
      	perror("ERROR reading from socket");
       	exit(1);
    }

    if((!strncmp(buf,"next",4)))
    {
    	bzero(message,256);
    	strncpy(message,"OK",2);
    	if(sendto(s,message, strlen(message),0,(struct sockaddr *) &si_other,slen) == -1)
    	{
    	 	perror("ERROR writing to socket");
    	   	exit(1);
    	}
    	break;
    }

}
    while(1)
    {
    	/*bzero(buf,BUFLEN);
    	if(recvfrom(s,buf,BUFLEN,0,(struct sockaddr *) &si_other,&slen) == -1)
    	    	{
    	    	  	perror("ERROR reading from socket");
    	    	   	exit(1);
    	    	}
    	    	printf("Reading %s\n",buf);
    	    	bzero(message,256);
    	    	    		fgets(message,255,stdin);
    	    	    		if(sendto(s,message, strlen(message),0,(struct sockaddr *) &si_other,slen) == -1)
    	    	    		{
    	    	    		  	perror("ERROR writing to socket");
    	    	    		   	exit(1);
    	    	    		}*/
    	bzero(buf,256);
    	if(recvfrom(s,buf,BUFLEN,0,(struct sockaddr *) &si_other,&slen) == -1)
    	{
    	      	perror("ERROR reading from socket");
    	       	exit(1);
    	}
    	//printf("Reading %s\n",buf);
    	if((strncmp(buf,"OK",2)))
    	{
    		if((strncmp(buf,"next",4)))
    			printf("%s\n",buf);
    	}

    	if(!(strncmp(buf,"next",4)))
    	{
    		printf("Writing\n");
    		bzero(message,256);
    		fgets(message,255,stdin);
    		if(sendto(s,message, strlen(message),0,(struct sockaddr *) &si_other,slen) == -1)
    		{
    			perror("ERROR writing to socket");
    		    exit(1);
    		}
    	}
    	else
    	{
    		bzero(message,256);
    		strncpy(message,"OK",2);
    		if(sendto(s,message, strlen(message),0,(struct sockaddr *) &si_other,slen) == -1)
    		{
    			perror("ERROR writing to socket");
    		    exit(1);
    		}
    	}

    }

}

/*#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>

#define SERVER "127.0.0.1"
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to send data

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];

    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    if (inet_aton(SERVER , &si_other.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    while(1)
    {
        printf("Enter message : ");
        gets(message);

        //send the message
        if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
        {
            die("sendto()");
        }

        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        memset(buf,'\0', BUFLEN);
        //try to receive some data, this is a blocking call
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
        {
            die("recvfrom()");
        }

        puts(buf);
    }

    close(s);
    return 0;
}*/

