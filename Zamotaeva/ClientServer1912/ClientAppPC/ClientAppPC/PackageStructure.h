#ifndef _PACKAGESTRUCTURE_
#define _PACKAGESTRUCTURE_
#include "CommandStructure.h"

static const int LENGTH_FIELD_SIZE = 3;
static const int TYPE_FIELD_SIZE = 2;
static const int DATA_START_POSITION = LENGTH_FIELD_SIZE+TYPE_FIELD_SIZE;


struct PackageStructure
{
    int package_len; 
    CommandStructure cmdstructure;
};


char * GeneratePackageData(PackageStructure packstructure); 
PackageStructure CreatePackage(CommandStructure cmd);
CommandStructure Parse(char* inputmessage);

#endif