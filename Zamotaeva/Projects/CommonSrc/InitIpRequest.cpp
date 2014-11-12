#include "stdafx.h"
#include "InitIpRequest.h"
#include "GetMyIP.h"
#include <string.h>


InitIpRequest::InitIpRequest(void):Command(CommandsIDs::INIT_IP_REQUEST)
{

}


InitIpRequest::~InitIpRequest(void)
{
}
char * InitIpRequest::GenerateData()
{
    char * answer = (char*)GetMyIP::getIPAddress();
	this->data = answer;
	this->data_len = strlen((char*)answer);
	return answer;
}

