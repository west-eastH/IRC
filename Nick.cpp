#include "Nick.hpp"

bool Nick::exceptionNick()
{
	if (_curUser.isPass() == false)
	{
		sendToClient(_curUser, _fd, "", _parsedCommand[0] + " :You need to pass first", SERVER);
		return true;
	}
	if (_parsedCommand.size() != 2)
	{
		sendToClient(_curUser, _fd, "431", " :No nickname given", SERVER);
		return true;
	}
	if (!isPrintable(_parsedCommand[1]) || _parsedCommand[1].length() > 9)
	{
		sendToClient(_curUser, _fd, "432", _parsedCommand[1] + " :Erroneous nickname", SERVER);
		return true;
	}
	if (findNick(_parsedCommand[1]) != -1)
	{
		sendToClient(_curUser, _fd, "433", _parsedCommand[1] + " :Nickname is already in use", SERVER);
		return true;
	}
	return false;
}

Nick::Nick(std::map<int, UserInfo> &clients, std::vector<Channel> &channels, uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(clients, channels, fd, parsedCommand) {}

Nick::~Nick() {}

void Nick::execute()
{
	if (exceptionNick())
		return ;
	sendToClient(_curUser, _fd, _parsedCommand[0], " :" + _parsedCommand[1], CLIENT);
	_curUser.setNickName(_parsedCommand[1]);
}
