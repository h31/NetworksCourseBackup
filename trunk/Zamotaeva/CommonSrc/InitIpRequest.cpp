#include "StdAfx.h"
#include "InitIpRequest.h"
#include "GetMyIP.h"


InitIpRequest::InitIpRequest(void):Command(CommandsIDs::INIT_IP_REQUEST)
{

}


InitIpRequest::~InitIpRequest(void)
{
}
BYTE * InitIpRequest::GenerateData()
{
	BYTE * answer = (BYTE*)GetMyIP::getIPAddress();
	this->data = answer;
	this->data_len = strlen((char*)answer);
	return answer;
}

