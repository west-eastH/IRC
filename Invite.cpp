#include "Invite.hpp"

Invite::Invite(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(clients, channels, fd, parsedCommand) {}

Invite::~Invite() {}

void Invite::execute()
{
	int chIdx = -1;
	int targetFd;
	std::string msg;

	if (exceptionInvite())
		return ;
	_clients[targetFd].channels[_parsedCommand[1]] = false;
	chIdx = findChannel(_parsedCommand[1]);
	_channels[chIdx].joinChannel(targetFd, _clients[targetFd]);

	sendToClient(_fd, "341", _parsedCommand[1] + " " + _parsedCommand[2], SERVER);
	//301 추가해야할지도 모름
	msg = "You are invited from " + _parsedCommand[1] + "\n";
	send(targetFd, msg.c_str(), msg.length(), 0);
}

bool Invite::exceptionInvite()
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
	if (_curUser.channels.find(_parsedCommand[1]) == _curUser.channels.end()
		|| _curUser.channels[_parsedCommand[1]] == false)
	{
		sendToClient(_fd, "482", _parsedCommand[1] + " :You're not channel operator", SERVER);
		return true;
	}
	if ((targetFd = findNick(_parsedCommand[2])) == -1 || !_clients[targetFd].isActive())
	{
		sendToClient(_fd, "401", _parsedCommand[2] + " :No such nick/channel", SERVER);
		return true;
	}
	if ((_clients[targetFd].channels.find(_parsedCommand[1]) != _clients[targetFd].channels.end()) 
		|| _curUser.getNickName() == _clients[targetFd].getNickName() )
	{
		sendToClient(_fd, "443", _parsedCommand[2] + " " + _parsedCommand[1] + " :is already on channel", SERVER);
		return true;	
	}
	return false;
}