#include "Pass.hpp"

Pass::Pass(std::map<int, UserInfo&>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand, std::string password)
	: Command(clients, channels, fd, parsedCommand), _password(password) {}

Pass::~Pass()
{
}
void Pass::execute()
{
	if (_curUser.isActive() == true)
		throw std::runtime_error("Already Passed!");
	if (_parsedCommand.size() != 2)
		throw std::runtime_error(_parsedCommand[0] + " : Not enough parameters");
	if (_parsedCommand[1] != _password)
	{
		_curUser.denyAccess();
		putString(_fd, "not good.\n");
	}
	else
	{
		_curUser.allowAccess();
		putString(_fd, "very good.\n");
	}
}
