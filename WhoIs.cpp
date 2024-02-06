#include "WhoIs.hpp"

bool WhoIs::exceptionWhoIs()
{
	if (_curUser.isPass() == false)
	{
		sendToClient(_curUser, _fd, "", " You need to pass first", SERVER);
		return true;
	}
	if (_parsedCommand.size() != 2)
	{
		sendToClient(_curUser, _fd, "431", _parsedCommand[0] + " :No nickname given", SERVER);
		return true;
	}
	if (findNick(_parsedCommand[1]) == -1)
	{
		sendToClient(_curUser, _fd, "401", _parsedCommand[1] + " :No such nick/channel", SERVER);
		return true;
	}
	return false;
}

WhoIs::WhoIs(std::map<int, UserInfo> &clients, std::vector<Channel> &channels, uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(clients, channels, fd, parsedCommand) {}

WhoIs::~WhoIs() {}

void WhoIs::execute()
{
	if (exceptionWhoIs())
		return;
}