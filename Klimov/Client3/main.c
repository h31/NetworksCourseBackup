#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#include <unistd.h>
#include <assert.h>

#define SERVER "127.0.0.1"
#define BUFLEN 512  //Max length of buffer
#define PORT 5001   //The port on which to send data

void die(char *s)
{
    perror(s);
    exit(1);
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

	/* print the result */

	//for (i = 0; i < (n_spaces+1); ++i)
	  //printf ("res[%d] = %s\n", i, res[i]);

	/* free the memory allocated */

	free (res);
	return res;
}

int get_cmd(char* buffer, int num){
	int cmd;
	char **temp;
	char* arr1;
	char* arr2;
	temp=str_split(buffer, '#');

	        	if(temp){
	        		int i;
	        	    for (i = 0; *(temp + i); i++)
	        	    {
	        	    	//printf("inp :%s\n", *(temp + i));
	        	        if(i == num)
	        	        	cmd = atoi(*(temp + i));
	        	        else if(i == 1)
	        	        	//memcpy(arr1, (*(temp + i)), strlen(*(temp + i))+1);
	        	        	//strcpy(arr1, (**(temp + i)));
	        	        //else if(i == 2)
	        	        	//strcpy(arr2, (*(temp + i)));
	        	        free(*(temp + i));
	        	    }
	        	    	//printf("\n");
	        	    free(temp);
	        	}
	//printf("After: %s\n", arr1);
	//printf("After: %s\n", arr2);
	return cmd;
}

void get_cmd_ch(char* buffer, int num, char* res){
	char* cmd;
	char **temp;
	temp=str_split(buffer, '#');

	        	if(temp){
	        		int i;
	        	    for (i = 0; *(temp + i); i++)
	        	    {
	        	    	printf("inp_ch :%s\n", *(temp + i));
	        	    	//printf("i= %d inp :%s\n", i, teeemp);

	        	        	//cmd = (*(temp + i));
	        	        	//cmd = teeemp;
	        	        	//res = teeemp;
	        	        //strcpy(res, cmd);
	        	        if(i == num)
	        	        	strcpy(res, *(temp + i));
	        	        free(*(temp + i));
	        	    }
	        	    	//printf("\n");
	        	    free(temp);
	        	}
	//return cmd;
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
		//printf("i:%d, t:%s", i, token);
		i++;
		token = strtok(NULL, s);
		if(num==i){
			res = token;
			return 0;
		}
	}
	return 0;
}

void get_args(char* inp_str, char* arg[]){//buffer and array to write
	int i = 0;
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

int main(int argc, char *argv[])
{

	int com,p;

	//printf("serv: %s port: %s", argv[1], argv[2]);
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

    if (inet_aton(argv[1] , &si_other.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
    int recv_len;
/*
    int recv_len;
    if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
            {
                die("recvfrom()");
            }

            //print details of the client/peer and the data received
            //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
            printf("From serv: %s\n" , buf);

            //now reply the client with the same data
            if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
            {
                die("sendto()");
            }
            bzero(buf, sizeof(buf));
*/

    socklen_t slen_o = sizeof(si_other);

    strcpy(message, "Hello");

    if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen_o)==-1)
                {
                    die("sendto()");
                }

                //receive a reply and print it
                //clear the buffer by filling null, it might have previously received data
                memset(buf,'\0', BUFLEN);
                //try to receive some data, this is a blocking call
                if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen_o) == -1)
                {
                    die("recvfrom()");
                }

                puts(buf);

                if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen_o)) == -1)
                        {
                            die("recvfrom()");
                        }

                        //print details of the client/peer and the data received
                        //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
                        printf("%s\n" , buf);//from server

                        //now reply the client with the same data
                        if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen_o) == -1)
                        {
                            die("sendto()");
                        }
                        bzero(buf, sizeof(buf));

    bzero(message, sizeof(message));
    //printf("Enter message : ");
            gets(message);

            //send the message

            if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen_o)==-1)
            {
                die("sendto()");
            }

            //receive a reply and print it
            //clear the buffer by filling null, it might have previously received data
            memset(buf,'\0', BUFLEN);
            //try to receive some data, this is a blocking call
            if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen_o) == -1)
            {
                die("recvfrom()");
            }

            //puts(buf);


    strcpy(message, "Start");

    if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen_o)==-1)
            {
                die("sendto()");
            }

            //receive a reply and print it
            //clear the buffer by filling null, it might have previously received data
            memset(buf,'\0', BUFLEN);
            //try to receive some data, this is a blocking call
            if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen_o) == -1)
            {
                die("recvfrom()");
            }

            puts(buf);

            if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen_o)) == -1)
                    {
                        die("recvfrom()");
                    }

                    //print details of the client/peer and the data received
                    //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
                    printf("%s\n" , buf);

                    //now reply the client with the same data
                    if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen_o) == -1)
                    {
                        die("sendto()");
                    }
                    bzero(buf, sizeof(buf));
                    char* arg[4];
                    char** arg_spli;
                    char** qq;
                    char* arg1;
                    char* arg2;


    while(1)
    {
        //printf("Enter message : ");
        gets(message);

        //send the message

        if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen_o)==-1)
        {
            die("sendto()");
        }

        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        memset(buf,'\0', BUFLEN);
        //try to receive some data, this is a blocking call
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen_o) == -1)
        {
            die("recvfrom()");
        }

        //puts(buf);

/*
        qq=str_split(buf, '#');

        if(qq){
        	int i;
        	        for (i = 0; *(qq + i); i++)
        	        {
        	            //printf("inp :%s\n", *(qq + i));

        	            if(i == 0)
        	            	com = atoi(*(qq + i));
        	            free(*(qq + i));
        	        }
        	        //printf("\n");
        	        free(qq);
        }*/

        int spli;
        arg_spli = splito_array(buf);
        //for(spli=0;spli<sizeof(arg_spli);spli++)
        	//printf("aa: %s\n", arg_spli[spli]);
        com = get_cmd(buf, 0);
        //get_cmd_ch(buf, 1, arg1);
        //get_cmd_ch(buf, 2, arg2);
        //use_token(buf, 1, arg1);
        //use_token(buf, 2, arg2);
        //printf("test arg1: %s\n", arg1);
        //printf("test arg2: %s\n", arg2);
        //printf("com: %d",com);

        //get_args(buf, arg);
        //for(p = 0; p<4;p++)
        	//printf("i = %d inp: %s\n", i, arg[p]);
        //puts(arg[0]);
        //if(arg[0]!=NULL){
        //	com = atoi(arg[0]);
        //}
        if(com == 1){
        	//puts("CL: coma 1");
        }
        else if(com == 2){
        	while(strcmp(buf, "allmesgs") != 0){
        	if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen_o)) == -1)
        	                    {
        	                        die("recvfrom()");
        	                    }

        	                    //print details of the client/peer and the data received
        	                    //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        	                    printf("%s\n" , buf);

        	                    //now reply the client with the same data
        	                    if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen_o) == -1)
        	                    {
        	                        die("sendto()");
        	                    }

        	}
        	                	/*if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen_o)) == -1)
        	                	                    {
        	                	                        die("recvfrom()");
        	                	                    }

        	                	                    //print details of the client/peer and the data received
        	                	                    //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        	                	                    printf("%s\n" , buf);

        	                	                    //now reply the client with the same data
        	                	                    if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen_o) == -1)
        	                	                    {
        	                	                        die("sendto()");
        	                	                    }*/
        	                	                	/*if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen_o)) == -1)
        	                	                        	                	                    {
        	                	                        	                	                        die("recvfrom()");
        	                	                        	                	                    }

        	                	                        	                	                    //print details of the client/peer and the data received
        	                	                        	                	                    //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        	                	                        	                	                    printf("%s\n" , buf);

        	                	                        	                	                    //now reply the client with the same data
        	                	                        	                	                    if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen_o) == -1)
        	                	                        	                	                    {
        	                	                        	                	                        die("sendto()");
        	                	                        	                	                    }*/
        	                    //puts(buf);
        	                    bzero(buf, sizeof(buf));

        	//puts("CL: coma 2");

        }
        else if(com == 3){
        	puts("Bye!\n");
        	exit(1);
        }
        else
        	puts("Wrong input\n");

        /*if(strcmp(message, "Start") == 0){
            //
            if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen_o)) == -1)
                    {
                        die("recvfrom()");
                    }

                    //print details of the client/peer and the data received
                    //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
                    printf("From serv: %s\n" , buf);

                    //now reply the client with the same data
                    if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen_o) == -1)
                    {
                        die("sendto()");
                    }
                    bzero(buf, sizeof(buf));
        }*/
        bzero(message, sizeof(message));
    }

    close(s);
    return 0;
}
