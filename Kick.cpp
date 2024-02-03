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
	std::map<int, UserInfo*>::iterator it;
	for (it = _channels[chIdx]._members.begin(); it != _channels[chIdx]._members.end(); ++it)
		sendToClient(it->first, _parsedCommand[0], " " + _parsedCommand[1] + " " + _parsedCommand[2] + " " + (_parsedCommand.size() == 4 ? _parsedCommand[3] : _parsedCommand[2]), CLIENT);
	sendToClient(targetFd, _parsedCommand[0], " " + _parsedCommand[1] + " " + _parsedCommand[2] + " " + (_parsedCommand.size() == 4 ? _parsedCommand[3] : _parsedCommand[2]), CLIENT);
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
		// :localhossssssssssssst 482 feelgood_ #ttt :You're not a channel operator
		// :zirconium.libera.chat 482 phan31 #aabbcc1 :You're not a channel operator
		//:123 482 jonhan #qwer :You're not channel operator
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
	// if (_curUser.getNickName() == _clients[targetFd].getNickName())
	// 	throw std::runtime_error("Can not kick yourself!!");
	return false;
}
