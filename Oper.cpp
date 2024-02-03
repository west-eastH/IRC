#include "Oper.hpp"

Oper::Oper(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand, std::string id, std::string password)
	: Command(clients, channels, fd, parsedCommand), _rootId(id), _rootPw(password) {}

Oper::~Oper() {}

void Oper::execute()
{
	if (exceptionOper())
		return ;
	_curUser.authorize();
	sendToClient(_fd, "381", " :You are now an IRC operator", SERVER);
	//putString(_fd, "You are now root!!!\n");
}

bool Oper::exceptionOper()
{
	if (_curUser.isActive() == false)
	{
		sendToClient(_fd, "", _parsedCommand[0] + " :You need to pass first", SERVER);
		return true;
	}
	if (_parsedCommand.size() != 3)
	{
		sendToClient(_fd, "461", " :Not enough parameters", SERVER);
		return true;
	}
	if (_parsedCommand[1] != _rootId || _parsedCommand[2] != _rootPw)
	{
		sendToClient(_fd, "464", " :Password incorrect", SERVER);
		return true;
	}
	if (_curUser.isRoot())
	{
		sendToClient(_fd, "", " You are already root!", SERVER);
		return true;
	}
	return false;
}
