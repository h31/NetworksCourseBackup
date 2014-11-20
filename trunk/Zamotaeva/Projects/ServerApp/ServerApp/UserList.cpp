#include "stdafx.h"
#include "UserList.h"
#include "string.h"

UserList::UserList(void)
{
}


UserList::~UserList(void)
{
}


void UserList::DecreaseTimeUsers(int decValue_ms) //decrement time many users
{
    for(unsigned int i=0;i<this->users.size();i++)
    {
        users[i].DecreaseLiveTime(decValue_ms);
    }
}


string UserList::getIPByName(string name)
{
     for(unsigned int i=0;i<this->users.size();i++)
     {
        User user = users[i];
        if(name == user.getUserName())
        {
            return user.getUserIp();
        }
     }
     return "127.000.000.001";
}

bool UserList::isNameUnique(string name)
{
    for(unsigned int i=0;i<this->users.size();i++)
    {
        User user = users[i];
        if (name == user.getUserName())
        {
            return false;
        }

    }
    return true;

}

bool UserList::isNameRight(string name)
{
    if(name.length() <= 0)
    {
        return false;
    }
    if(name.length() > 32)
    {
        return false;
    }
    if(name.find('\0') > 0)
    {
        return false;
    }
    if(name.find('\n') > 0)
    {
        return false;
    }
    return true;
}

User* UserList::getUserByIP(string ip)
{
    for(int i=0;i<this->users.size();i++)
    {
        if(users[i].getUserIp() == ip)
        {
            return &users[i];
        }
    }
    return NULL;
}

bool UserList::RestoreTimeByIP(string ip)
{
    User* user = getUserByIP(ip);
    if(user != NULL)
    {
        user->RestoreLiveTime();
        return true;
    }
    return false;

}
