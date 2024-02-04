#include "Part.hpp"

Part::Part(std::map<int, UserInfo> &clients, std::vector<Channel> &channels, uintptr_t fd, std::vector<std::string> parsedCommand) : Command(clients, channels, fd, parsedCommand) {}

Part::~Part()
{
}

bool Part::exceptionPart()
{
	int channelFd = findChannel(_parsedCommand[1]);
	if (_curUser.isActive() == false)
	{
		sendToClient(_fd, "", _parsedCommand[0] + " :You need to pass first", SERVER);
		return true;
	}
	if (_parsedCommand.size() != 2)
	{
		sendToClient(_fd, "461", " :Not enough parameters", SERVER);
		return true;
	}
	if (_curUser.channels.find(_parsedCommand[1]) == _curUser.channels.end())
	{
		sendToClient(_fd, "442", _parsedCommand[1] + " :You're not on that channel", SERVER);
		return true;
	}
	if (channelFd == -1)
	{
		sendToClient(_fd, "403", _parsedCommand[1] + " :No such Channel", SERVER);
		return true;
	}
	return false;
}

void Part::execute()
{
	int chIdx = -1;
	std::string msg;

	if (exceptionPart())
		return ;
	_clients[_fd].channels.erase(_parsedCommand[1]);
	chIdx = findChannel(_parsedCommand[1]);
	std::map<int, UserInfo*>::iterator it;
	for (it = _channels[chIdx]._members.begin(); it != _channels[chIdx]._members.end(); ++it)
		sendToClient(it->first, _parsedCommand[0], " " + _parsedCommand[1], CLIENT);
	if (_channels[chIdx].partChannel(_fd) == 0)
		_channels.erase(_channels.begin() + chIdx);
}
