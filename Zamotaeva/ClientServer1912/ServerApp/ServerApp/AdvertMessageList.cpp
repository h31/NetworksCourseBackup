#include "stdafx.h"
#include "AdvertMessageList.h"


AdvertMessageList::AdvertMessageList(void)
{
	advertList.push_back("http://habrahabr.ru - be smart");
	advertList.push_back("http://ideone.com    Online coding. Be mobile!");
	advertList.push_back("https://www.google.ru - we know");
	this->Counter = 0;
}

AdvertMessageList::~AdvertMessageList(void)
{
}


string AdvertMessageList::getNextAdvert() //choose advert
{

	if( advertList.size() == 0 )
	{
		return "";
	}
	if(Counter >= advertList.size())
	{
		Counter = 0;
	}
	Counter++;
	return advertList[Counter - 1];
}
