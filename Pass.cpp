#include "Pass.hpp"

Pass::Pass(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand, std::string password)
	: Command(clients, channels, fd, parsedCommand), _password(password) {
		std::cout << "Pass constructor called" << std::endl;
	}

Pass::~Pass()
{
}
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
		sendToClient(_fd, "462", " :Unauthorized command (already registered)", SERVER);
		return true;
	}
	if (_parsedCommand.size() != 2)
	{
		sendToClient(_fd, "461", " :Not enough parameters", SERVER);
		return true;
	}
	return false;
}

//NICK phan
//USER phan phan 0 :pilho
//WHOIS phan
//PING irc.libera.chat