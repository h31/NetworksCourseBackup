#include "stdafx.h"
#include "UserList.h"


UserList::UserList(void)
{
}


UserList::~UserList(void)
{
}


void UserList::DecreaseTimeUsers(int decValue_ms) //decrement time many users
{
    for(int i=0;i<this->users.size();i++)
    {
        users[i].DecreaseLiveTime(decValue_ms);
    }
}
