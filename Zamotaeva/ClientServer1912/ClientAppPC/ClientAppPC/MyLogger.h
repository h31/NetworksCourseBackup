#ifndef _MYLOGGER_H
#define _MYLOGGER_H

#include <iostream>
#include "AppTheme.h"
#include "MessageCollection.h"

void WriteSuccessNetworkEvent(char * text);

void WriteSuccessNetworkEvent(char * text,char *object);

void WriteFailNetworkEvent(char * text);
void WriteFailEvent(char * text);
void WriteMessage(MessageStructure message_str);
void WriteAllMessages(MessageCollection* message_collection);
void WritePackageInfo(char * text,char * status);
void WriteObjectInfo(char *object,char * text);

#endif