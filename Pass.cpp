#include "Pass.hpp"

Pass::Pass(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand, std::string password)
	: Command(clients, channels, fd, parsedCommand), _password(password) {}

Pass::~Pass()
{
}
void Pass::execute()
{
	if (_curUser.isActive() == true)
		throw std::runtime_error("Already Passed!");
	if (_parsedCommand.size() != 2)
		throw CommandError("461", _parsedCommand[0], " : Not enough parameters");
	if (_parsedCommand[1] != _password)
	{
		_curUser.denyAccess();
	}
	else
	{
		_curUser.allowAccess();
	}
}
