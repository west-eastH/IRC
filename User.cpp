#include "User.hpp"

bool User::exceptionUser()
{
	if (_curUser.isPass() == false)
	{
		sendToClient(_fd, "", _parsedCommand[0] + " :You need to pass first", SERVER);
		return true;
	}
	if (_curUser.isActive() == 1)
	{
		sendToClient(_fd, "462", _parsedCommand[0] + " :Unauthorized command (already registered)", SERVER);
		return true;
	}
	if (_parsedCommand.size() < 5)
	{
		sendToClient(_fd, "461", " :Not enough parameters", SERVER);
		return true;
	}
	if (_parsedCommand[4][0] != ':')
	{
		sendToClient(_fd, "", _parsedCommand[0] + " :Wrong realname arg!", SERVER);
		return true;
	}
	return false;
}

User::User(std::map<int, UserInfo> &clients, std::vector<Channel> &channels, uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(clients, channels, fd, parsedCommand)
{
}

User::~User()
{
}

void User::execute()
{
	if (exceptionUser())
		return ;
	//USER <사용자이름> <호스트이름> <서버이름> :<실제이름>
	_curUser.setUserName(_parsedCommand[1]);
	_curUser.setHostName(_parsedCommand[2]);
	_curUser.setServerName(_parsedCommand[3]);

	std::string realname;
	for (int i = 4; i < (int)_parsedCommand.size(); i++)
		realname += _parsedCommand[i] + " ";
	realname.erase(std::find(realname.begin(), realname.end(), ':'));
	realname.erase(realname.find_last_not_of(" ") + 1);
	_curUser.setRealName(realname);
	_curUser.activate();
	std::string postfix = _curUser.getNickName() + "!~" + _curUser.getUserName() + "@" + _curUser.getServerName();
	sendToClient(_fd, "001", "Welcome to the Internet Relay Network " + postfix, SERVER);
	sendToClient(_fd, "002", "Your host is " + _curUser.getServerName() + ", running version 0.0.1", SERVER);
	sendToClient(_fd, "003", "This server was created 24/02/02", SERVER);
	sendToClient(_fd, "004", _curUser.getServerName() + " 0.0.1 ", SERVER);
}