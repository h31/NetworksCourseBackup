#ifndef _COMMANDSTRUCTURE_
#define _COMMANDSTRUCTURE_


enum IDs
	{
    ECHO_REQUEST,
    ECHO_ANSWER,

    LOGIN_REQUEST,
    LOGIN_ANSWER,

    USERS_REQUEST,
    USERS_ANSWER,

    BAD_PACKAGE,
    UNAUTHORIZED_ACTION,// user not login

    MESSAGE_SEND,
    MESSAGE_INCORRECT_NAME,
    MESSAGE_SUCCESS_SENDED,
    MESSAGE_USER_OFFLINE,
    MESSAGE_DELIVERY,

    ADVERT_MESSAGE,

    ALIVE_REQUEST, //server asks client about life
    ALIVE_ANSWER, //client wants to be alive

    QUIT_REQUEST, //client want to disconnect


	};
struct CommandStructure
{
	int CommandId;
	char* data;
	int data_len;
};

#endif