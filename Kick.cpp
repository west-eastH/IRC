#include "Kick.hpp"

Kick::Kick(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(clients, channels, fd, parsedCommand) {}

Kick::~Kick() {}

void Kick::execute()
{
	int chIdx = -1;
	int targetFd = findNick(_parsedCommand[2]);
	std::string msg;

	if (exceptionKick())
		return ;
	_clients[targetFd].channels.erase(_parsedCommand[1]);
	chIdx = findChannel(_parsedCommand[1]);
	_channels[chIdx].kickMember(targetFd);
	std::string members = _channels[chIdx].getMembers();
	sendToClient(_fd, _parsedCommand[0], " " + _parsedCommand[1] + " " + _parsedCommand[2] + " :" + (_parsedCommand.size() == 3 ? _parsedCommand[3] : _parsedCommand[2]), CLIENT);
	sendToClient(targetFd, _parsedCommand[0], " " + _parsedCommand[1] + " " + _parsedCommand[2] + " :" + (_parsedCommand.size() == 3 ? _parsedCommand[3] : _parsedCommand[2]), CLIENT);
	sendToClient(_fd, "353", "= " + _parsedCommand[1] + " :" + members, SERVER);
	sendToClient(_fd, "366", _parsedCommand[1] + " :End of /NAMES list", SERVER);
}

bool Kick::exceptionKick()
{
	int channelFd = findChannel(_parsedCommand[1]);
	if (_curUser.isActive() == false)
	{
		sendToClient(_fd, "", _parsedCommand[0] + " :You need to pass first", SERVER);
		return true;
	}
	if (_parsedCommand.size() < 3 || _parsedCommand.size() > 4)
	{
		sendToClient(_fd, "431", " :No nickname given", SERVER);
		return true;
	}
	if (_curUser.channels.find(_parsedCommand[1]) == _curUser.channels.end())
	{
		sendToClient(_fd, "442", _parsedCommand[1] + " :You're not on that channel", SERVER);
		return true;
	}
	if (_curUser.channels[_parsedCommand[1]] == false)
	{
		sendToClient(_fd, "482", _parsedCommand[1] + " :You're not channel operator", SERVER);
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
	// if (_curUser.getNickName() == _clients[targetFd].getNickName())
	// 	throw std::runtime_error("Can not kick yourself!!");
	return false;
}
