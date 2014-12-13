#include "stdafx.h"
#include "MyServer.h"

#include "commandFactory.h"
#include "StringsHelper.h"
#include "MyMessage.h"
#include "AdvertMessageList.h"

#ifdef _WIN32
#pragma comment(lib, "WS2_32.lib")
#elif __linux
#include <unistd.h>
#endif



MyServer::MyServer(void) 
{
}


MyServer::~MyServer(void)
{
}

bool MyServer::SetupLibrary()
{
#ifdef _WIN32 //Windows

	WSADATA wsaData;
	int errorcode = WSAStartup(MAKEWORD(2,2),&wsaData);
	if (errorcode)
	{
		MyLogger::WriteFailNetworkEvent("setup lib error");
		return false;
	}
	else
	{
		MyLogger::WriteSuccessNetworkEvent("setub lib complete");
		return true;
	}
#endif
	return true;
}

void MyServer::CleanUpLibrary()
{
#ifdef _WIN32
	WSACleanup();
#endif
}

sockaddr_in MyServer::createListenAddr()
{
	sockaddr_in serv;
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = INADDR_ANY;
	serv.sin_port = htons( DEFAULT_PORT );
	return serv;

}

bool MyServer::runTCP()
{
	mysocket s;
	s = socket(AF_INET,SOCK_STREAM,0);
	if(s == INVALID_SOCKET)
	{
		MyLogger::WriteFailNetworkEvent("invalid socket");
		return false;
	}

	sockaddr_in server_addr = createListenAddr();
	int servaddr_len = sizeof(server_addr);
	if (bind(s,(struct sockaddr * )&server_addr,servaddr_len) < 0)
	{
		MyLogger::WriteFailNetworkEvent("bind error\n");
		return false;
	}
	sockaddr_in in_addr;
	sock_len addr_len = sizeof(in_addr);
	listen(s , SOMAXCONN);
	int read_size = 0;
	mysocket s_in;
	printf("wait connections\n");

	//**************************
	#ifdef _WIN32
	CreateThread(NULL,0,&TimerUserTimeOut, &userlist,0,NULL); //timer thread
	#elif __linux
	pthread_t sendthread;
	pthread_create(&sendthread,NULL,TimerUserTimeOut,reinterpret_cast<void*>(&userlist));
	#endif
	//***********************


	//*****************
	#ifdef _WIN32
	CreateThread(NULL,0,&AdvertMessage, &userlist,0,NULL); //advert thread
	#elif __linux
	pthread_t sendthread;
	pthread_create(&sendthread,NULL,AdvertMessage,reinterpret_cast<void*>(&userlist));
	#endif

	//********************


	while( s_in = accept(s , (struct sockaddr * )&in_addr,&addr_len))
	{

		printf(" connection accepted\n");
		SendMetaInfo info = {s_in,&userlist}; //common userlist
#ifdef _WIN32
		CreateThread(NULL,0,&Communicate, &info,0,NULL); //thread
#elif __linux
		pthread_t sendthread;
		pthread_create(&sendthread,NULL,Communicate,reinterpret_cast<void*>(&info));
		//pthread_join(sendthread,NULL);
#endif
	}

	MyServer::closeSocket(s);
	return true;
}

bool MyServer::runUDP()
{
	mysocket s;
	s = socket(AF_INET,SOCK_DGRAM,0);
	int send_size = 0;
	if(s == INVALID_SOCKET)
	{
		MyLogger::WriteFailNetworkEvent("invalid socket");
		return false;
	}
	struct sockaddr_in servaddr,cliaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons( DEFAULT_PORT );

	if (bind(s,(struct sockaddr * )&servaddr,sizeof(servaddr)) < 0)
	{
		MyLogger::WriteFailNetworkEvent("bind error\n");
		return false;
	}
	char* buffer_in = StringsHelper::getCleanBuffer(Package::MAX_PACKAGE_SIZE);
	mysocket s_in;
	printf("waiting connection\n");
	for(;;)
	{
		int len = sizeof(cliaddr);
		int rec_size = recvfrom(s,buffer_in,Package::MAX_PACKAGE_SIZE,0,(struct sockaddr*)&cliaddr,&len); //cliaddr-incomming address
		printf("get %d byte\n",rec_size);
		buffer_in[rec_size] = 0;
		Command cmd = Package::Parse(buffer_in);
		if(cmd.commandID == CommandsIDs::ECHO_REQUEST)
		{
			MyLogger::WritePackageInfo("echo request","receive");
			EchoAnswerCommand echoAnswer;
			Package EchoPack = Package::Generate(echoAnswer);
			SendDataUDP(EchoPack,cliaddr,s,"echo request");
		}
		if(cmd.commandID == CommandsIDs::LOGIN_REQUEST)
		{
			MyLogger::WritePackageInfo("login request","receive");
			string login = User::ParseName(cmd.data);
			bool isUnique = userlist.isNameUnique(login);
			bool isRight  = userlist.isNameRight(login);

			bool Answer = isUnique && isRight;
			if(Answer == true)
			{
				User* newUser = new User(login,cliaddr);
				userlist.tryAddNewUser(newUser);
				MyLogger::WriteSuccessNetworkEvent("add new user",(char*)login.c_str());
			}
			else
			{
				if(isUnique == false)
				{
					MyLogger:: WriteFailEvent("bad login:not unique [" + login + "]");
				}
				if(isRight == false)
				{
					MyLogger:: WriteFailEvent("bad login: incorrect lenght or symbols ["+login + "]");
				}
			}
			LoginAnswerCommand loginAnswer(Answer);
			Package LoginPack = Package::Generate(loginAnswer);
			SendDataUDP(LoginPack,cliaddr,s,"login answer");
			if(send_size > 0)
			{
				MyLogger::WritePackageInfo("login answer","send");
			}
		}
		if(cmd.commandID == CommandsIDs::USERS_REQUEST)
		{
			if(userlist.isAuthorizedUser(cliaddr)==false)
			{
				//send unauthorized action package
				MyLogger::WriteFailEvent("unauthorized user action");
			}
			MyLogger::WritePackageInfo("users request","receive");
			UsersAnswerCommand usersanswer(&userlist);
			Package UserPack = Package::Generate(usersanswer);
			SendDataUDP(UserPack,cliaddr,s,"user answer");
			if(send_size > 0)
			{
				MyLogger::WritePackageInfo("users answer","send");
			}
		}
		if(cmd.commandID == CommandsIDs::MESSAGE_SEND)
		{
			MyLogger::WritePackageInfo("message","receive");

			MyMessage incoming_message = MyMessage::ConvertDataToMessage(cmd.data);
			//mysocket s_out = userlist.getSocketByName(incoming_message.receiver_name);
			sockaddr_in addr = userlist.getAddrByName(incoming_message.receiver_name);
			send_size = sendto(s,buffer_in,strlen(buffer_in),0,(struct sockaddr*)&addr,sizeof(addr));
			if(send_size > 0)
			{
				MyLogger::WritePackageInfo("message","send");
			}
			else
			{
				MyLogger::WriteFailEvent("fail to delivery incoming message");
			}
		}
		if(cmd.commandID == CommandsIDs::ECHO_ANSWER)
		{
			MyLogger::WritePackageInfo("echo answer","receive");
		}
		if(cmd.commandID == CommandsIDs::QUIT_REQUEST)
		{
			MyLogger::WritePackageInfo("quit request","receive");
			userlist.deleteQuitUser(cliaddr);
		}
	}
	return true;
}

returnType MyServer::TimerUserTimeOut(void * user_list_arg) //timer thread function 
{
	UserList* userlist = (UserList*)(user_list_arg);
	while(1)
	{


		for(unsigned int i = 0; i<userlist->users.size() ; i++)
		{
			User* curUser = &userlist->users[i];
			if(curUser->getLiveTime() <= 0)
			{
				QuitCommand quitCmd;
				quitCmd.GenerateData();
				Package packquit = Package::Generate(quitCmd);
				SendData(packquit,curUser->getUserSocket(),"quit timeout command to " + curUser->getUserName());
			}
		}

		for(unsigned int i = 0; i<userlist->users.size() ; i++)
		{
			User* curUser = &userlist->users[i];
			if(curUser->getLiveTime() <=  MyServer::USER_WARNING_TIME && curUser->getWarning() == false)
			{
				curUser->setWarning(true);
				EchoRequestCommand echoRequestCmd;
				echoRequestCmd.GenerateData();
				Package packEchoReq = Package::Generate(echoRequestCmd);
				SendData(packEchoReq,curUser->getUserSocket(),"timeout warning to " + curUser->getUserName());
			}
		}
#ifdef __linux
		sleep(MyServer::USER_UPDATE_TIME_S);
#elif _WIN32
		Sleep(MyServer::USER_UPDATE_TIME_MS);
#endif
		userlist->DecreaseTimeUsers(MyServer::USER_UPDATE_TIME_MS); //decrease lifetime for all users 3sec, sleep 3sec
		userlist->deleteTimeOutedUsers(); //delete users (lifetime < 0)
	}
	return NULL;
}

returnType MyServer::Communicate(void  * socket_param)
{
	SendMetaInfo info = *(SendMetaInfo*)socket_param;
	mysocket s_in = info.socket;
	UserList * userlist = info.userlist;

	char* buffer_in = StringsHelper::getCleanBuffer(Package::MAX_PACKAGE_SIZE);
	//string buffer_in;
	int read_size = 0;
	int send_size = 0;
	while(read_size = recv(s_in,buffer_in,Package::MAX_PACKAGE_SIZE,0) > 0)
	{
		Command cmd = Package::Parse(buffer_in);
		userlist->RestoreTimeBySocket(s_in);
		if(cmd.commandID == CommandsIDs::ECHO_REQUEST)
		{
			MyLogger::WritePackageInfo("echo request","receive");
			EchoAnswerCommand echoAnswer;
			Package EchoPack = Package::Generate(echoAnswer);
			SendData(EchoPack,s_in,"echo request");
		}
		if(cmd.commandID == CommandsIDs::LOGIN_REQUEST)
		{
			MyLogger::WritePackageInfo("login request","receive");
			string login = User::ParseName(cmd.data);
			bool isUnique = userlist->isNameUnique(login);
			bool isRight  = userlist->isNameRight(login);

			bool Answer = isUnique && isRight;
			if(Answer == true)
			{
				userlist->tryAddNewUser(login,s_in);
				_Longlong descr = (int)s_in;
				string newUserInfo = "["+login+" , " + to_string(descr) + "]";
				MyLogger::WriteSuccessNetworkEvent("add new user",(char*)newUserInfo.c_str());
			}
			else
			{
				if(isUnique == false)
				{
					MyLogger:: WriteFailEvent("bad login:not unique [" + login + "]");
				}
				if(isRight == false)
				{
					MyLogger:: WriteFailEvent("bad login: incorrect lenght or symbols ["+login + "]");
				}
			}
			LoginAnswerCommand loginAnswer(Answer);
			Package LoginPack = Package::Generate(loginAnswer);
			send_size =  send(s_in,LoginPack.data.c_str(),LoginPack.packagesize,0);
			if(send_size > 0)
			{
				MyLogger::WritePackageInfo("login answer","send");
			}
		}
		if(cmd.commandID == CommandsIDs::USERS_REQUEST)
		{
			if(userlist->isAuthorizedUser(s_in)==false)
			{
				//send unauthorized action package
				MyLogger::WriteFailEvent("unauthorized user action");
			}
			MyLogger::WritePackageInfo("users request","receive");
			UsersAnswerCommand usersanswer(userlist);
			Package UserPack = Package::Generate(usersanswer);
			send_size =  send(s_in,UserPack.data.c_str(),UserPack.packagesize,0);
			if(send_size > 0)
			{
				MyLogger::WritePackageInfo("users answer","send");
			}
		}
		if(cmd.commandID == CommandsIDs::MESSAGE_SEND)
		{
			MyLogger::WritePackageInfo("message","receive");

			MyMessage incoming_message = MyMessage::ConvertDataToMessage(cmd.data);
			mysocket s_out = userlist->getSocketByName(incoming_message.receiver_name);
			send_size =  send(s_out,buffer_in,strlen(buffer_in),0);
			if(send_size > 0)
			{
				MyLogger::WritePackageInfo("message","send");
			}
			else
			{
				MyLogger::WriteFailEvent("fail to delivery incoming message");
			}
		}
		if(cmd.commandID == CommandsIDs::ECHO_ANSWER)
		{
			MyLogger::WritePackageInfo("echo answer","receive");
		}
		if(cmd.commandID == CommandsIDs::QUIT_REQUEST)
		{
			MyLogger::WritePackageInfo("quit request","receive");
			userlist->deleteQuitUser(s_in);
		}
	}
	return NULL;
}

void MyServer::closeSocket(mysocket socket)
{
#ifdef _WIN32
	shutdown(socket,SD_BOTH);
	closesocket(socket);
#elif __linux
	shutdown(socket,SHUT_RDWR);
	close(socket);
#endif
}

void MyServer::SendData(Package package, mysocket socket,string info_message)
{
	int send_size = 0;

	send_size =  send(socket,package.data.c_str(),package.packagesize,0);
	if(send_size > 0)
	{
		MyLogger::WritePackageInfo((char*)info_message.c_str(),"send");
	}
	else
	{
		string fail_text = "fail to send " + info_message;
		MyLogger::WriteFailEvent((char*)fail_text.c_str());
	}
}

void MyServer::SendDataUDP(Package package,sockaddr_in addr, mysocket socket,string info_message)
{
	int send_size = 0;
	sock_len addr_len = sizeof(addr);
	send_size = sendto(socket,package.data.c_str(),package.packagesize,0,(struct sockaddr*)&addr,addr_len);
}

returnType MyServer::AdvertMessage(void * user_list_arg) 
{
	UserList* userlist = (UserList*)(user_list_arg);
	AdvertMessageList advert_list; //advertmess
	while(1)
	{
		string text = advert_list.getNextAdvert();
		AdvertCommand advertCmd(text);
		advertCmd.GenerateData();
		Package packAdvert = Package::Generate(advertCmd);
		for(unsigned int i = 0; i<userlist->users.size() ; i++)
		{
			User* curUser = &userlist->users[i];
			SendData(packAdvert,curUser->getUserSocket(),"advert to " + curUser->getUserName());
		}
		#ifdef __linux
		sleep(MyServer::ADVERT_SEND_PAUSE_S);
		#elif _WIN32
		Sleep(MyServer::ADVERT_SEND_PAUSE_MS);
		#endif
	}
}