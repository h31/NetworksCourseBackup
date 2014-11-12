#pragma once
#include "CommandStructure.h"

static const int LENGTH_FIELD_SIZE = 3; //3 byte dlina vsego packeta
static const int TYPE_FIELD_SIZE = 2;	// 2 byte pod tip kommd
static const int SENDER_FIELD_SIZE = 15;//ip
static const int DATA_FIELD_SIZE = 4; //
static const int DATA_START_POSITION = LENGTH_FIELD_SIZE+TYPE_FIELD_SIZE+SENDER_FIELD_SIZE;


struct PackageStructure
{
    int package_len; //dlina
    char * sender; //ip otpr
    CommandStructure cmdstructure;//tip kommand , dann
};


char * GeneratePackageData(PackageStructure packstructure); //delaem paket - buffer(symv mass) otpr v socket
PackageStructure CreatePackage(CommandStructure cmd, char* sender); //cozd struct iz cmd, ip
