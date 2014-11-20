#ifndef _COMMANDSIDS_H_
#define _COMMANDSIDS_H_

enum IDs
	{
    ECHO_REQUEST,
    ECHO_ANSWER,

    LOGIN_REQUEST,
    LOGIN_ANSWER,

    USERS_REQUEST,
    USERS_ANSWER,

    BAD_PACKAGE_ANSWER,
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

#endif
