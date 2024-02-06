#include "Pass.hpp"

Pass::Pass(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand, std::string password)
	: Command(clients, channels, fd, parsedCommand), _password(password) {}

Pass::~Pass() {}

void Pass::execute()
{
	if (exceptionPass())
		return ;
	if (_parsedCommand[1] != _password)
		_curUser.denyAccess();
	else
		_curUser.allowAccess();
}

bool Pass::exceptionPass()
{
	if (_curUser.isActive() == true)
	{
		sendToClient(_curUser, _fd, "462", " :Unauthorized command (already registered)", SERVER);
		return true;
	}
	if (_parsedCommand.size() != 2)
	{
		sendToClient(_curUser, _fd, "461", " :Not enough parameters", SERVER);
		return true;
	}
	return false;
}
