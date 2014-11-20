#ifndef COMMANDFACTORY_H
#define COMMANDFACTORY_H


#include "Command.h"
#include <string>


class EchoAnswerCommand : public Command
{
public:
    EchoAnswerCommand(void); //create command id 
    char* GenerateData(); //create data field
};
//---------------------------------------------
//---------------------------------------------

class EchoRequestCommand : public Command
{
public:
    EchoRequestCommand(void);
    char* GenerateData();
};

//---------------------------------------------
//---------------------------------------------

class LoginRequestCommand : public Command
{
public:
    std::string Login;
    LoginRequestCommand(std::string Login);
    char* GenerateData();
};
//---------------------------------------------
//---------------------------------------------

class LoginAnswerCommand : public Command
{
public:
    LoginAnswerCommand(bool isGoodLogin);
    char* GenerateData();

private:
    bool isGoodLogin;
};
//---------------------------------------------
//---------------------------------------------

class UsersAnswerCommand :public Command
{
public:
    UsersAnswerCommand();
    char* GenerateData();
};

//---------------------------------------------
//---------------------------------------------

class UsersRequestCommand :public Command
{
public:
    UsersRequestCommand();
    char* GenerateData();
};

//---------------------------------------------
//---------------------------------------------

class BadPackageCommand : public Command
{
public:
    BadPackageCommand(void);
    ~BadPackageCommand(void);
    char* GenerateData();
};

//---------------------------------------------
//---------------------------------------------

class CommandFactory
{
public:
    CommandFactory();
};

#endif // COMMANDFACTORY_H
