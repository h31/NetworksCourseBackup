#ifndef _PACKAGESTRUCTURE_H_
#define _PACKAGESTRUCTURE_H_
#include "PackageStructure.h"
#include "MessageStructure.h"


CommandStructure GenarateEchoRequestCommand();
CommandStructure GenarateEchoAnswerCommand();
CommandStructure GenarateLoginRequestCommand(char *login_name);
CommandStructure GenarateLoginAnswerCommand(bool answer);
CommandStructure GenarateAdvCommand(char *adv_text);
CommandStructure GenarateMessageSendCommand(MessageStructure msg_str);
CommandStructure GenarateMessageDeliverCommand();


#endif