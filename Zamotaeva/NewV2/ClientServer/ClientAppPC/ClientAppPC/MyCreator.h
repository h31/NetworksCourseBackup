#ifndef _MYCREATOR_H
#define _MYCREATOR_H

#include "MessageStructure.h"


char* getCleanBuffer(int Size);
char checkSymbol(char symb);

static const char * quitPackage = "006161";
static const char * EchoAnswer = "006011";
static const char * UsersRequest = "006041";
static const char * EchoRequest = "006001";

#endif