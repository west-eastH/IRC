#include "Privmsg.hpp"

bool Privmsg::exceptionPrivmsg()
{
	if (_curUser.isPass() == false)
	{
		sendToClient(_fd, "", _parsedCommand[0] + " :You need to pass first", SERVER);
		return true;
	}
	if (_parsedCommand.size() == 1)
	{
		sendToClient(_fd, "411", " :No recipient given (" + _parsedCommand[0] + ")", SERVER);
		return true;
	}
	if (_parsedCommand.size() == 2)
	{
		sendToClient(_fd, "412", " :No text to send", SERVER);
		return true;
	}
	int targetFd;
	if ((_parsedCommand[1].front() != '#' && ((targetFd = findNick(_parsedCommand[1])) == -1 || !_clients[targetFd].isActive()))
		|| (_parsedCommand[1].front() == '#' && findChannel(_parsedCommand[1]) == -1))
	{
		sendToClient(_fd, "401", _parsedCommand[1] + " :No such nick/channel", SERVER);
		return true;
	}
	if (_parsedCommand[1].front() == '#' && _curUser.channels.find(_parsedCommand[1]) == _curUser.channels.end())
	{
		sendToClient(_fd, "404", _parsedCommand[1] + " :Cannot send to channel", SERVER);
		return true;
	}
	return false;
}

Privmsg::Privmsg(std::map<int, UserInfo> &clients, std::vector<Channel> &channels, uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(clients, channels, fd, parsedCommand) {}

Privmsg::~Privmsg() {}
void Privmsg::execute()
{
	if (exceptionPrivmsg())
		return ;
	if (_parsedCommand[1].front() == '#')
	{
		int chIdx = findChannel(_parsedCommand[1]);
		std::map<int, UserInfo*>::iterator it;
		for (it = _channels[chIdx]._members.begin(); it != _channels[chIdx]._members.end(); ++it)
			if (it->first != static_cast<int>(_fd))
				sendToClient(it->first, _parsedCommand[0], " " + _parsedCommand[1] + " :" + _parsedCommand[2], CLIENT);
	}
	else
	{
		int targetFd = findNick(_parsedCommand[1]);
		sendToClient(targetFd, _parsedCommand[0], " " + _parsedCommand[1] + " :" + _parsedCommand[2], CLIENT);
	}
}
///connect -nocap localhost 6667 1234
