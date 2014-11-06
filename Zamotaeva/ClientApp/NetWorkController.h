#pragma once

#include "MyClient.h"
class NetWorkController
{
private:
	MyClient* client;
	void InitController();
public:
	NetWorkController(void);
	~NetWorkController(void);

	
	void ConnectToServer();
	void CloseConnection();

	void SendEchoRequest();
	void SendInitIPrequest();
	void SendLoginRequest();

	
};

