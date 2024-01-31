#include "User.hpp"

bool User::exceptionUser()
{
	if (_curUser.isPass() == false)
	{
		errorToClient("", _parsedCommand[0], "You need to pass first");
		return true;
	}
	if (_curUser.isActive() == 1)
	{
		errorToClient("462", _parsedCommand[0], "Unauthorized command (already registered)");
		return true;
	}
	if (_parsedCommand.size() < 5)
	{
		errorToClient("461", _parsedCommand[0], "Not enough parameters");
		return true;
	}
	if (_parsedCommand[4][0] != ':')
	{
		errorToClient("", _parsedCommand[0], "Wrong realname arg!");
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
	responseToClient("001", _parsedCommand[0], "Welcome to the Internet Relay Network");
}