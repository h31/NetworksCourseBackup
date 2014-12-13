#include "StdAfx.h"
#include "AdvertMessageList.h"


AdvertMessageList::AdvertMessageList(void)
{
	advertList.push_back("http://habrahabr.ru_-_be_smart");
	advertList.push_back("http://ideone.com-online_coding.Be_mobile");
	advertList.push_back("https://www.google.ru-_we_know");
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