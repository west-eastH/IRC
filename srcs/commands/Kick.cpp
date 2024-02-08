#include "Kick.hpp"

Kick::Kick(uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(fd, parsedCommand) {}

Kick::~Kick() {}

bool Kick::handleException()
{
	int channelFd = findChannel(_parsedCommand[1]);
	UserAccount& curUser = Database::getInstance();

	if (curUser.isActive() == false)
	{
		sendToClient(_fd, "", _parsedCommand[0] + " :You need to pass first", SERVER);
		return true;
	}
	if (_parsedCommand.size() < 3 || _parsedCommand.size() > 4)
	{
		sendToClient(_fd, "431", " :No nickname given", SERVER);
		return true;
	}
	if (curUser.channels.find(_parsedCommand[1]) == curUser.channels.end())
	{
		sendToClient(_fd, "442", _parsedCommand[1] + " :You're not on that channel", SERVER);
		return true;
	}
	if (curUser.channels[_parsedCommand[1]] == false)
	{
		sendToClient(_fd, "482", _parsedCommand[1] + " :You're not a channel operator", SERVER);
		return true;
	}	
	int targetFd = findNick(_parsedCommand[2]);
	if (targetFd == -1 || _clients[targetFd].channels.find(_parsedCommand[1]) == _clients[targetFd].channels.end())
	{
		sendToClient(_fd, "441", _parsedCommand[2] + " " + _parsedCommand[1] + " :They aren't on that channel", SERVER);
		return true;
	}
	if (channelFd == -1)
	{
		sendToClient(_fd, "403", _parsedCommand[1] + " :No such Channel", SERVER);
		return true;
	}
	return false;
}

void Kick::execute()
{
	int chIdx = -1;
	int targetFd;
	std::string msg;

	if (handleException())
		return ;
	targetFd = findNick(_parsedCommand[2]);
	_clients[targetFd].channels.erase(_parsedCommand[1]);
	chIdx = findChannel(_parsedCommand[1]);
	std::map<int, UserAccount*>::iterator it;
	for (it = _channels[chIdx]._members.begin(); it != _channels[chIdx]._members.end(); ++it)
	{
		sendToClient(_curUser, it->first, _parsedCommand[0], " " + _parsedCommand[1] + " " + _parsedCommand[2] + " " + (_parsedCommand.size() >= 4 ? makeMsg(3) : _parsedCommand[2]), CLIENT);
	}
	if (_channels[chIdx].partChannel(targetFd) == 0)
		_channels.erase(_channels.begin() + chIdx);
}
