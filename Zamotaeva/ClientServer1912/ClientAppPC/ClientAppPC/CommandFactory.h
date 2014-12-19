#ifndef _PACKAGESTRUCTURE_H_
#define _PACKAGESTRUCTURE_H_
#include "PackageStructure.h"
#include "MessageStructure.h"


CommandStructure GenerateEchoRequestCommand();
CommandStructure GenerateEchoAnswerCommand();
CommandStructure GenerateLoginRequestCommand(char *login_name);
CommandStructure GenerateMessageSendCommand(MessageStructure msg_str);
CommandStructure GenerateUsersRequestCommand();
CommandStructure GenerateQuitRequest();


#endif