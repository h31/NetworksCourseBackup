#pragma once
class CommandsIDs
{
public:
    enum IDs
	{
		ECHO_REQUEST, //client -- sprash servera ti tut  0 
		ECHO_ANSWER, //server otvech  1

		LOGIN_REQUEST, //client zalog hochet	//Yulia
		LOGIN_ANSWER, //server otv - uspesno	//1 0 

		INIT_IP_REQUEST, //client hochet otprv ip svoi	//10.200.2.1
		INIT_IP_ANSWER,									//1

		CLIENT_IP_REQUEST, //client hochet sprosit u servera ip druga	//Petya
		CLIENT_IP_ANSWER,												//10.200.1.1 / NO_FOUND polz

		BAD_PACKAGE_ANSWER, //

		MESSAGE_SEND, //client hochet otprav mes komy to cherez server	//Petya ### Hello
		MESSAGE_INCOORECT_NAME,//nekorrect name
		MESSAGE_REC, //server otpr mes poluch		//Hello

        SPAM_MESSAGE	//	//visit our site


    };
};

