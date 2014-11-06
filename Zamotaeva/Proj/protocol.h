#ifndef PROTOCOL_H
#define PROTOCOL_H

enum command //perechisl    polz - server
{
    CMD_MSG,
    CMD_USERLIST
};

struct CMD //kommanda
{
    unsigned int cmd;
    unsigned int id;
    unsigned int res;
};

struct MSG
{
    unsigned int daddr; //adres poluch
    unsigned int length; //dlina soobchen
};

struct USER_LIST //skolko polz
{
    unsigned int count;
};

struct USER_INFO
{
    unsigned int user_id;
    ///Позже можно будет добавить ник
};


#endif // PROTOCOL_H
