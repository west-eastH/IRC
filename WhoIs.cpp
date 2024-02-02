#include "WhoIs.hpp"

bool WhoIs::exceptionWhoIs()
{
	if (_curUser.isPass() == false)
	{
		sendToClient(_fd, "", " You need to pass first", SERVER);
		return true;
	}
	if (_parsedCommand.size() != 2)
	{
		sendToClient(_fd, "431", _parsedCommand[0] + " :No nickname given", SERVER);
		return true;
	}
	if (findNick(_parsedCommand[1]) == -1)
	{
		sendToClient(_fd, "401", _parsedCommand[1] + " :No such nick/channel", SERVER);
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
	// sendToClient(_fd, _parsedCommand[0], _parsedCommand[1], 0);
	// _curUser.setWhoIsName(_parsedCommand[1]);
	// responseToClient("311", _parsedCommand[0], _curUser.getNickName() + " " + _curUser.getHostName() + " " + _curUser.getServerName() + " * :" + _curUser.getRealName());
	//std::string success = "311 WHOIS " + _curUser.getNickName() + " " + _curUser.getHostName() + " " + _curUser.getServerName() + " * :" + _curUser.getRealName() + "\r\n";
	//const char *msg = success.c_str();
	//send(_fd, const_cast<char *>(msg), std::strlen(msg), 0);
	//responseToClient("318", _curUser.getNickName(), "End of WHOIS list");
		// std::stromg success = "318 WHOIS " + _c 318 RPL_ENDOFWHOIS
		// 					  "<nick> :End of WHOIS list"

}