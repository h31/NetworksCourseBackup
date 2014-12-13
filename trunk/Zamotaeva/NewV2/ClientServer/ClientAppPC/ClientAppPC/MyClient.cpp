#include "stdafx.h"
#include <stdlib.h>
#include <string.h>


#include "MyClient.h"
#include "CommandFactory.h"
#include "CommandsIDs.h"
#include "MyLogger.h"
#include "MyCreator.h"
#include "GetCurrentTime.h"



#ifdef _WIN32
	#pragma comment(lib, "WS2_32.lib")
#endif

sockaddr_in createSendAddr()
{
	sockaddr_in address;

	address.sin_family = AF_INET;
    //address.sin_addr.s_addr = inet_addr("192.128.0.13"); //server adress
	address.sin_addr.s_addr = inet_addr("127.0.0.1"); //server adress
    address.sin_port = htons( DEFAULT_PORT );
	return address;
}

bool SetUpLib()
{
	#ifdef _WIN32
		WSADATA wsaData;
		int errorcode = WSAStartup(MAKEWORD(2,2),&wsaData);
        if (errorcode)
           {
               //printf("fail setup lib\n");
			   WriteFailNetworkEvent("fail to setup lib");
               return false;
           }
           else
           {
			   WriteSuccessNetworkEvent("setup lib");
               //printf("setup lib\n");
               return true;
           }
	#endif
}

void CleanLib()
{
#ifdef _WIN32
    WSACleanup();
#endif
}

mysocket createSocket()
{
    return socket(AF_INET,SOCK_STREAM,0);
}

bool runTCP()
{
    mysocket s = createSocket();
	UserList list = initEmptyList();
	char *login = getCleanBuffer(1);
	//MessageCollection *collection = initEmptyCollectionPointer();
	MessageCollection *collection = &initEmptyCollection();
    if(s == INVALID_SOCKET)
    {
		WriteFailNetworkEvent("bad socket");
        //printf("bad socket\n");
        return false;
    }
    sockaddr_in adr = createSendAddr();
    addr_size addr_len = sizeof(adr);

    int connectionResult = connect(s,(struct sockaddr *)&adr, addr_len); //connect to server
    if(connectionResult < 0)
    {
		WriteFailNetworkEvent("bad connection");
        //printf("bad connection\n");
        return false;
    }

   // sockaddr_in incomingAddr;
    int send_size = 0;
	int rec_size = 0;
    bool isOnWork = true; //client works - listen from server
	char * receivebuffer = getCleanBuffer(MAX_PACKAGE_SIZE);

	SendMetaInfo info = {login,&list,&s,collection};
	#ifdef _WIN32
		CreateThread(NULL,0,&InputText, &info,0,NULL); 
	#elif __linux
         pthread_t sendthread;
         pthread_create(&sendthread,NULL,InputText,reinterpret_cast<void*>(&info));
         pthread_join(sendthread,NULL);
	#endif

    CommandStructure EchoRequestCmd = GenerateEchoRequestCommand(); //client sends echo request
    PackageStructure package_echo_req = CreatePackage(EchoRequestCmd);
    char *senddata = GeneratePackageData(package_echo_req);
	send_size = sendData(s,senddata);
	if(send_size < 0)
	{
		printf("fail to send echo request\n");
	}
	if(send_size > 0)
	{
		printf("send echo request\n");
	}
	printf("wait server answers\n");
	
    while(isOnWork) // client listen 
    {
		
        while (recv (s,receivebuffer,MAX_PACKAGE_SIZE,0) > 0)
        {
			CommandStructure cmd = Parse(receivebuffer);
			if(cmd.CommandId == IDs::ECHO_ANSWER)
			{
				printf("receive echo answer. server online\n");
				char *inplogin = inputLogin(); //client inputs login
				memcpy(login,inplogin,strlen(inplogin));
				CommandStructure LoginRequestCmd = GenerateLoginRequestCommand(inplogin); //command = type+data
				PackageStructure package_login_req = CreatePackage(LoginRequestCmd);
				char *sendlogindata = GeneratePackageData(package_login_req); //convert (6 0 1 -> 006001)
				send_size = sendData(s,sendlogindata);
				if(send_size < 0)
				{
					printf("fail to send login request\n");
				}
				if(send_size > 0)
				{
					printf("send login request\n");
				}
				//free(sendlogindata);
			}
			if(cmd.CommandId == IDs::LOGIN_ANSWER)
			{
				if(cmd.data[0] == '1')
				{
					printf("login accepted\n");
					CommandStructure UsersRequestCmd = GenerateUsersRequestCommand();
					PackageStructure package_users_req = CreatePackage(UsersRequestCmd);
					char *sendusersdata = GeneratePackageData(package_users_req);
					send_size =sendData(s,sendusersdata);
					if(send_size < 0)
					{
						printf("fail to send users request\n");
					}
					if(send_size > 0)
					{
						printf("send users request\n");
					}

				}
				else
				{
					printf("login rejected. Try again\n");
					char *inplogin_r = inputLogin(); //client inputs login
					memcpy(login,inplogin_r,strlen(inplogin_r));
					CommandStructure LoginRequestCmd = GenerateLoginRequestCommand(login); //command = type+data
					PackageStructure package_login_req = CreatePackage(LoginRequestCmd);
					char *sendlogindata = GeneratePackageData(package_login_req); //convert (6 0 1 -> 006001)
					send_size = sendData(s,sendlogindata);
					if(send_size < 0)
					{
						printf("fail to send login request\n");
					}
					if(send_size > 0)
					{
						printf("send login request\n");
					}
				}
			}
			if(cmd.CommandId == IDs::UNAUTHORIZED_ACTION)
			{
				printf("unauthorized action answer\n");
			}
			if(cmd.CommandId == IDs::USERS_ANSWER)
			{
				printf("receive users answer\n");
				list = ParseUsers(cmd.data);
			}
			if(cmd.CommandId == IDs::MESSAGE_SEND)
			{
				MessageStructure message =  parseData(cmd.data);
				AddNewUnReadedMessage(collection,message);
			}

			if(cmd.CommandId == IDs::ECHO_REQUEST)
			{
				CommandStructure EchoAnswerCmd = GenerateEchoAnswerCommand(); //client sends echo answer
				PackageStructure package_echo_ans = CreatePackage(EchoAnswerCmd);
				char *senddata = GeneratePackageData(package_echo_ans);
				send_size = sendData(s,senddata);
			}
			if(cmd.CommandId == IDs::QUIT_REQUEST)
			{
				WriteFailEvent("Quit command: close app");
				closeSocket(s);
				exit(2);
			}
			if(cmd.CommandId == IDs::ADVERT_MESSAGE)
			{
				MessageStructure message = convertAdvertToMessage(cmd.data);
				AddNewUnReadedMessage(collection,message);
			}
			free(receivebuffer);
			receivebuffer = getCleanBuffer(MAX_PACKAGE_SIZE);
        }
    }

	return true;
}


int sendData(mysocket s,char *data)
{
	int len = strlen(data)+1;
	return send(s,data,len,0);
}

int sendDataUDP(mysocket s,sockaddr_in addr,char *data)
{
	int len = strlen(data)+1;
	return sendto(s,data,len,0, (struct sockaddr *)&addr,sizeof(addr));
}

char * inputLogin()
{
	char * login = getCleanBuffer(256);

	printf("input your name\n");
	scanf("%255s",login);
	return login;
}

char * inputMessage()
{
	char * message = getCleanBuffer(256);
	printf("input message:\n");
    scanf("%255s",message);	//scanf ignore symbols after whitespace
	scanf("%255[^\n]", message);
	//scanf("%255[0-9a-zA-Z ]s", message);
	/*
	fgets (message, 256, stdin);
	if ((strlen(message)>0) && (message[strlen (message) - 1] == '\n'))
	{
		message[strlen (message) - 1] = '\0';
	}
	*/
	return message;
}

int  getAction(SendMetaInfo info) //from console
{
	int action_id = 1;
	do
	{
		ClearConsole();
		PrintfInfoTitleText(info.login,info.userlist->list_len,info.collection->unreaded_size);
		printf("choose action:\n");
		printf("%d. show messages\n", ACTION_SHOW_MESSAGE);
		printf("%d. send message\n",ACTION_SEND_MESSAGE);
		printf("%d. show contacts\n",ACTION_SHOW_CONTACTS);
		printf("%d. quit\n",ACTION_QUIT);
		scanf("%d",&action_id);
	}
	while(action_id < ACTION_SHOW_MESSAGE || action_id > ACTION_QUIT);
	return action_id;
}

User GetUser(UserList* list)
{
	int user_number = -1;
	int own_id = -1;
	do
	{
		printf("choose message receiver:\n");
		for(int i = 0; i < list->list_len;i++)
		{
			//char * username;			
			//username = getCleanBuffer(list.users[i].name_len + 1);
			//memcpy(username,list.users[i].name,list.users[i].name_len);
			//username[list.users[i].name_len] = '\0';
			printf("%d) %s\n",i+1,list->users[i].name);
		}
		printf("0) back\n");
		scanf("%d",&user_number);
	}
	while(user_number < 0 || user_number - 1 >= list->list_len|| user_number == own_id);

	if(user_number == 0)
	{
		return EmptyUser();
	}
	return list->users[user_number - 1];
}

void PrintMessages(MessageCollection* collection)
{
	ClearConsole();

	WriteAllMessages(collection); //show all messages
	//MoveAllUnReadToRead(collection);
	printf("\n<- press any key to return\n");
	int any_key = 0;
	scanf("%s");
}


#ifdef __linux
returnType InputText(void  * info_param) //thread function
#elif _WIN32
 returnType WINAPI InputText(void  * info_param) //thread function
#endif

{
	SendMetaInfo info = *(SendMetaInfo*)info_param;

	char* login = info.login;
	mysocket s = *info.s;
	UserList* list = info.userlist;
	MessageCollection* collection = info.collection;
	int curRegime = info.CurRegime;

	sockaddr_in addr = createSendAddr();


	while(1)
	{
		if(strlen(info.login) <=0   || info.userlist->list_len <= 0) //condition
		{
			#ifdef __linux
			usleep(500*1000);
			#elif _WIN32
			Sleep(500); //uncorrect login or userlist, sleep
			#endif
		}
		else
		{

			int action = 0;
			action = getAction(info);
			if(action == ACTION_SEND_MESSAGE)
			{
				int send_size = 0;
				int rec_size = 0;
				int len = 0;
				User user = GetUser(list);
				if(isEmptyUser(user)== false)
				{
					char * message_text;
					message_text = inputMessage();
					MessageStructure message = generateMessage(message_text,login,user,getCurrentTime());
					CommandStructure MessageSendCommand = GenerateMessageSendCommand(message);
					PackageStructure package_message_send = CreatePackage(MessageSendCommand);
					char *sendmessagedata = GeneratePackageData(package_message_send);
					if(CurrentRegime == TCPregime)
					{
						send_size = sendData(s,sendmessagedata);
					}
					if(CurrentRegime == UDPregime)
					{
						send_size = sendDataUDP(s,addr,sendmessagedata);
					}
					if(send_size < 0)
					{
						printf("fail to send message\n");
					}
					if(send_size > 0)
					{
						printf("send message\n");
					}
				}
				else
				{
					printf("empty user\n");
				}
			
				continue;
			}

			if(action == ACTION_SHOW_CONTACTS)
			{
				//send users request [view actual userlist]
				CommandStructure UsersRequestCmd = GenerateUsersRequestCommand();
				PackageStructure package_users_req = CreatePackage(UsersRequestCmd);
				char *sendusersdata = (char*)UsersRequest;
				if(CurrentRegime == TCPregime)
				{
					int send_size = sendData(s,sendusersdata);
				}
				if(CurrentRegime == UDPregime)
				{
					sendDataUDP(s,addr,sendusersdata);
				}
				#ifdef __linux
				usleep(500*1000);
				#elif _WIN32
				Sleep(500); 
				#endif
				GetUser(list);
				continue;
			}
			if(action == ACTION_SHOW_MESSAGE)
			{
				PrintMessages(collection);
				continue;
			}
			if(action == ACTION_QUIT)
			{
				char *sendquitdata = (char*)quitPackage;
				if(CurrentRegime == TCPregime)
				{
					sendData(s,sendquitdata);
					closeSocket(s);
					exit(0);
				}
				if(CurrentRegime == UDPregime)
				{
					sendDataUDP(s,addr,sendquitdata);
					exit(0);
				}
				
			}
		}
	
	}
	return 0;
}


bool runUDP()
{
	mysocket s = socket(AF_INET,SOCK_DGRAM,0);
	UserList list = initEmptyList();
	char *login = getCleanBuffer(1);
	//MessageCollection *collection = initEmptyCollectionPointer();
	MessageCollection *collection = &initEmptyCollection();
    if(s == INVALID_SOCKET)
    {
		WriteFailNetworkEvent("bad socket");
        //printf("bad socket\n");
        return false;
    }
    struct sockaddr_in servaddr,cliaddr;
	//servaddr.sin_family = AF_INET;
	//servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	//servaddr.sin_port=htons(DEFAULT_PORT);
	servaddr = createSendAddr();

	//bind(s,(struct sockaddr *)&adr,addr_len);

    int send_size = 0;
	int rec_size = 0;
    bool isOnWork = true; //client works - listen from server
	char * receivebuffer = getCleanBuffer(MAX_PACKAGE_SIZE);

	SendMetaInfo info = {login,&list,&s,collection,CurrentRegime};
	#ifdef _WIN32
		CreateThread(NULL,0,&InputText, &info,0,NULL); 
	#elif __linux
         pthread_t sendthread;
         pthread_create(&sendthread,NULL,InputText,reinterpret_cast<void*>(&info));
         pthread_join(sendthread,NULL);
	#endif

    CommandStructure EchoRequestCmd = GenerateEchoRequestCommand(); //client sends echo request
    PackageStructure package_echo_req = CreatePackage(EchoRequestCmd);
    char *senddata = GeneratePackageData(package_echo_req);
	send_size = sendDataUDP(s,servaddr,senddata);
	if(send_size < 0)
	{
		printf("fail to send echo request\n");
	}
	if(send_size > 0)
	{
		printf("send echo request\n");
	}
	printf("wait server answers\n");
	int len = sizeof(cliaddr);
    while(isOnWork) // client listen 
    {
	
		while (recvfrom(s,receivebuffer,MAX_PACKAGE_SIZE,0,(struct sockaddr *)&cliaddr,&len) > 0)
		{
			CommandStructure cmd = Parse(receivebuffer);
			if(cmd.CommandId == IDs::ECHO_ANSWER)
			{
				printf("receive echo answer. server online\n");
				char *inplogin = inputLogin(); //client inputs login
				memcpy(login,inplogin,strlen(inplogin));
				CommandStructure LoginRequestCmd = GenerateLoginRequestCommand(inplogin); //command = type+data
				PackageStructure package_login_req = CreatePackage(LoginRequestCmd);
				char *sendlogindata = GeneratePackageData(package_login_req); //convert (6 0 1 -> 006001)
				send_size = sendDataUDP(s,servaddr,sendlogindata);
				if(send_size < 0)
				{
					printf("fail to send login request\n");
				}
				if(send_size > 0)
				{
					printf("send login request\n");
				}
				//free(sendlogindata);
			}
			if(cmd.CommandId == IDs::LOGIN_ANSWER)
			{
				if(cmd.data[0] == '1')
				{
					printf("login accepted\n");
					CommandStructure UsersRequestCmd = GenerateUsersRequestCommand();
					PackageStructure package_users_req = CreatePackage(UsersRequestCmd);
					char *sendusersdata = GeneratePackageData(package_users_req);
					send_size =sendDataUDP(s,servaddr,sendusersdata);
					if(send_size < 0)
					{
						printf("fail to send users request\n");
					}
					if(send_size > 0)
					{
						printf("send users request\n");
					}

				}
				else
				{
					printf("login rejected. Try again\n");
					char *inplogin_r = inputLogin(); //client inputs login
					memcpy(login,inplogin_r,strlen(inplogin_r));
					CommandStructure LoginRequestCmd = GenerateLoginRequestCommand(login); //command = type+data
					PackageStructure package_login_req = CreatePackage(LoginRequestCmd);
					char *sendlogindata = GeneratePackageData(package_login_req); //convert (6 0 1 -> 006001)
					send_size = sendDataUDP(s,servaddr,sendlogindata);
					if(send_size < 0)
					{
						printf("fail to send login request\n");
					}
					if(send_size > 0)
					{
						printf("send login request\n");
					}
				}
			}
			if(cmd.CommandId == IDs::UNAUTHORIZED_ACTION)
			{
				printf("unauthorized action answer\n");
			}
			if(cmd.CommandId == IDs::USERS_ANSWER)
			{
				printf("receive users answer\n");
				list = ParseUsers(cmd.data);
			}
			if(cmd.CommandId == IDs::MESSAGE_SEND)
			{
				MessageStructure message =  parseData(cmd.data);
				AddNewUnReadedMessage(collection,message);
			}

			if(cmd.CommandId == IDs::ECHO_REQUEST)
			{
				CommandStructure EchoAnswerCmd = GenerateEchoAnswerCommand(); //client sends echo answer
				PackageStructure package_echo_ans = CreatePackage(EchoAnswerCmd);
				char *senddata = GeneratePackageData(package_echo_ans);
				send_size = sendDataUDP(s,servaddr,senddata);
			}
			if(cmd.CommandId == IDs::QUIT_REQUEST)
			{
				WriteFailEvent("Quit command: close app");
				closeSocket(s);
				exit(2);
			}
			free(receivebuffer);
			receivebuffer = getCleanBuffer(MAX_PACKAGE_SIZE);
        }
    }
	return true;
}

void closeSocket(mysocket socket)
{
#ifdef _WIN32
	shutdown(socket,SD_BOTH);
	closesocket(socket);
#elif __linux
	shutdown(socket,SHUT_RDWR);
	close(socket);
#endif
}