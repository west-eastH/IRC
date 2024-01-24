#include "Pass.hpp"

Pass::Pass(std::map<int, UserInfo> &clients, uintptr_t fd, std::vector<std::string> temp_split, std::string password)
	:Command(clients, fd, temp_split), _password(password)
{
}

Pass::~Pass()
{
}

void Pass::execute()
{
	if (_curUser.isActive() == 0)
	{
		if (_parsedCommand.size() != 2)
			write(_fd, "Wrong Pass args.\n", 17);
		else
		{
			if (_parsedCommand[1] == _password)
				_curUser.setPass(1);
			else
				_curUser.setPass(0);
		}
	}
	else
		write(_fd, "Already Passed.\n", 16);
}
