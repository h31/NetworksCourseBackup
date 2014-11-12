#pragma on
#include "Command.h"
#include <string>
class LoginRequestCommand : public Command
{
public:
	std::string Login;
	LoginRequestCommand(std::string Login);
	~LoginRequestCommand(void);
    char* GenerateData();
};

