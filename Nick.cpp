#include "Nick.hpp"

bool Nick::exceptionNick()
{
	if (_curUser.isPass() == false)
	{
		errorToClient("", _parsedCommand[0], "You need to pass first");
		return true;
	}
	if (_parsedCommand.size() != 2)
	{
		errorToClient("431", _parsedCommand[0], "No nickname given");
		return true;
	}
	if (!isPrintable(_parsedCommand[1]) || _parsedCommand[1].length() > 9)
	{
		errorToClient("432", _parsedCommand[1], "Erroneous nickname");
		return true;
	}
	if (findNick(_parsedCommand[1]) != -1)
	{
		errorToClient("433", _parsedCommand[1], "Nickname is already in use");
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
	sendToClient(_fd, _parsedCommand[0], _parsedCommand[1], 0);
	_curUser.setNickName(_parsedCommand[1]);
}