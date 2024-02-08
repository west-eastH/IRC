#include "Invite.hpp"

Invite::Invite(std::map<int, UserAccount>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(clients, channels, fd, parsedCommand) {}

Invite::~Invite() {}

void Invite::execute()
{
	int chIdx = -1;
	int targetFd;
	std::string msg;

	if (exceptionInvite())
		return ;
	targetFd = findNick(_parsedCommand[1]);
	_clients[targetFd].channels[_parsedCommand[2]] = false;
	chIdx = findChannel(_parsedCommand[2]);
	_channels[chIdx].inviteMembers.push_back(_parsedCommand[1]);
	sendToClient(_curUser, _fd, "341", _parsedCommand[1] + " " + _parsedCommand[2], SERVER);
	sendToClient(_curUser, targetFd, _parsedCommand[0], " " + _parsedCommand[1] + " :" + _parsedCommand[2], CLIENT);
}
bool Invite::exceptionInvite()
{
	int targetFd;

	if (_curUser.isActive() == false)
	{
		sendToClient(_curUser, _fd, "", _parsedCommand[0] + " :You need to pass first", SERVER);
		return true;		
	}
	if (_parsedCommand.size() != 3)
	{
		sendToClient(_curUser, _fd, "461", " :Not enough parameters", SERVER);
		return true;
	}
	if (_curUser.channels.find(_parsedCommand[2]) == _curUser.channels.end()
		|| _curUser.channels[_parsedCommand[2]] == false)
	{
		sendToClient(_curUser, _fd, "482", _parsedCommand[2] + " :You're not channel operator", SERVER);
		return true;
	}
	if ((targetFd = findNick(_parsedCommand[1]))== -1 || !_clients[targetFd].isActive())
	{
		sendToClient(_curUser, _fd, "401", _parsedCommand[1] + " :No such nick/channel", SERVER);
		return true;
	}
	if ((_clients[targetFd].channels.find(_parsedCommand[2]) != _clients[targetFd].channels.end()) 
		|| _curUser.getNickName() == _clients[targetFd].getNickName() )
	{
		sendToClient(_curUser, _fd, "443", _parsedCommand[1] + " " + _parsedCommand[2] + " :is already on channel", SERVER);
		return true;	
	}
	return false;
}