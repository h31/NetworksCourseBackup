#include "commandFactory.h"

CommandFactory::CommandFactory()
{
}
//---------------------------------------------
//----------------EchoAnswerCommand------------

EchoAnswerCommand::EchoAnswerCommand(void): Command(CommandsIDs::ECHO_ANSWER) //command type
{
}

char* EchoAnswerCommand::GenerateData() //data
{
    char * answer = new char[1];
    char ans = '1';
    answer[0] = ans;
    this->data = answer;
    this->data_len = 1;
    return answer;
}

//---------------------------------------------
//-----------------EchoRequestCommand----------

EchoRequestCommand::EchoRequestCommand(void):Command(CommandsIDs::ECHO_REQUEST)
{
}

char* EchoRequestCommand::GenerateData()
{
    char * answer = new char[1];
    char ans = '1';
    answer[0] = ans;
    this->data = answer;
    this->data_len = 1;
    return answer;
}

//---------------------------------------------
//-----------LoginRequestCommand---------------
LoginRequestCommand::LoginRequestCommand(std::string Login):Command(CommandsIDs::LOGIN_REQUEST)
{
    this->Login = Login;
}

char* LoginRequestCommand::GenerateData()
{
    char * answer = new char[Login.size()];
    for(unsigned int i= 0;i<Login.size();i++)
    {
        answer[i] = Login[i];
    }
    this->data = answer;
    this->data_len = Login.size();
    return answer;
}



//---------------------------------------------
//-----------------LoginAnswerCommand----------

LoginAnswerCommand::LoginAnswerCommand(bool isGoodLogin) :Command(CommandsIDs::LOGIN_ANSWER)
{
    this->isGoodLogin = isGoodLogin;
}


//---------------------------------------------
//---------------------------------------------

UsersAnswerCommand::UsersAnswerCommand() : Command(CommandsIDs::USERS_ANSWER)
{

}


//---------------------------------------------
//---------------------------------------------


UsersRequestCommand::UsersRequestCommand() : Command(CommandsIDs::USERS_REQUEST)
{
}

//---------------------------------------------
//---------------------------------------------
BadPackageCommand::BadPackageCommand(void) :Command(CommandsIDs::BAD_PACKAGE_ANSWER)
{
}

//---------------------------------------------
//---------------------------------------------
