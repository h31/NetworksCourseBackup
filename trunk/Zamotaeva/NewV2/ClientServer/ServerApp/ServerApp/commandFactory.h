#ifndef COMMANDFACTORY_H
#define COMMANDFACTORY_H


#include "Command.h"
#include <string>
#include "../ServerApp/UserList.h"

class EchoAnswerCommand : public Command
{
public:
    EchoAnswerCommand(void); //create command id 
    void GenerateData(); //create data field
};

//---------------------------------------------
//---------------------------------------------

class EchoRequestCommand : public Command
{
public:
	EchoRequestCommand(void);
	void GenerateData();
};
//---------------------------------------------
//---------------------------------------------
class LoginAnswerCommand : public Command
{
public:
    LoginAnswerCommand(bool isGoodLogin);
    void GenerateData();

private:
    bool isGoodLogin;
};
//---------------------------------------------
//---------------------------------------------

class UsersAnswerCommand :public Command
{
public:
    UsersAnswerCommand(UserList * userlist);
    void GenerateData();
private:
	UserList * userlist;
};

//---------------------------------------------
//---------------------------------------------

class BadPackageCommand : public Command
{
public:
    BadPackageCommand(void);
    void GenerateData();
};
//---------------------------------------------
//---------------------------------------------
class UnAuthoririzedActionCommand: public Command
{
private:
	int action_id;
public:
	UnAuthoririzedActionCommand(int id);
	void GenerateData();
};
//---------------------------------------------
//---------------------------------------------
class QuitCommand: public Command
{
public:
	QuitCommand(void);
	void GenerateData();
};
//---------------------------------------------
//---------------------------------------------
class AdvertCommand: public Command
{
private:
	string advert_text;
public:
	AdvertCommand(string advert_text);
	void GenerateData();
};
//---------------------------------------------
//---------------------------------------------

class CommandFactory
{
public:
    CommandFactory();
};




#endif // COMMANDFACTORY_H
