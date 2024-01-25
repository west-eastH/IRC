#include "Pass.hpp"

Pass::Pass(std::map<int, UserInfo> &clients, uintptr_t fd, std::vector<std::string> temp_split, std::string password)
	:Command(clients, fd, temp_split), _password(password)
{
	std::cout << "Pass constructor called" << std::endl;
}

Pass::~Pass()
{
}
void Pass::execute()
{
	if (_curUser.isActive() == 0)
	{
		if (_parsedCommand.size() != 2)
			putString(_fd, _parsedCommand[0] + " :Not enough parameters\n");
		else
		{
			if (_parsedCommand[1] == _password)
			{
				std::cout << _parsedCommand[1] << " == " << _password << std::endl;
				_curUser.setPass(1);
				putString(_fd, "very good.\n");
			}
			else
			{
				std::cout << _parsedCommand[1] << " != " << _password << std::endl;
				_curUser.setPass(0);
				putString(_fd, "not good.\n");
			}
		}
	}
	else
		putString(_fd, "Already Passed.\n");
}
