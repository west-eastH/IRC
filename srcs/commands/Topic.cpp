#include "Topic.hpp"

Topic::Topic(std::map<int, UserAccount>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(clients, channels, fd, parsedCommand) {}

Topic::~Topic() {}

bool Topic::exceptionTopic()
{
	if (_curUser.isActive() == false)
	{
		sendToClient(_curUser, _fd, "", "You need to login first", SERVER);
		return true;
	}
	if (_parsedCommand.size() < 2 || _parsedCommand.size() > 3)
	{
		sendToClient(_curUser, _fd, "461", " :Not enough parameters", SERVER);
		return true;
	}
	if (_curUser.channels.find(_parsedCommand[1]) == _curUser.channels.end())
	{
		sendToClient(_curUser, _fd, "442", _parsedCommand[1] + " :You're not on that channel" , SERVER);
		return true;
	}
	return false;
}
bool Topic::printTopic(int chIdx)
{
	if (_parsedCommand.size() == 2)
	{
		if (_channels[chIdx].getTopic().length() == 0)
			sendToClient(_curUser, _fd, "331", _parsedCommand[1] + " :No topic is set", SERVER);
		else
			sendToClient(_curUser, _fd, "332", _parsedCommand[1] + " :" +  _channels[chIdx].getTopic(), SERVER);
		return true;
	}
	return false;
}

bool Topic::checkAuth(int chIdx)
{
	if (_channels[chIdx].checkMode("t") && _curUser.channels[_parsedCommand[1]] == false)
	{
		sendToClient(_curUser, _fd, "482", _parsedCommand[1] + " :You're not channel operator", SERVER);
		return true;
	}
	return false;
}

void Topic::execute()
{
	int chIdx = -1;
	std::string msg;

	if (exceptionTopic())
		return ;
	chIdx = findChannel(_parsedCommand[1]);
	if (printTopic(chIdx))
		return ;

	if (checkAuth(chIdx))
		return ;

	std::string topic = makeMsg(2);
	size_t	pos = topic.find(':');
	if (pos != std::string::npos)
		topic = topic.erase(pos, 1);
	_channels[chIdx].setTopic(topic);
	std::map<int, UserAccount*>::iterator it;
	for (it = _channels[chIdx]._members.begin(); it != _channels[chIdx]._members.end(); ++it)
		sendToClient(_curUser, it->first, _parsedCommand[0], " " + _parsedCommand[1] + " :" +  _channels[chIdx].getTopic(), CLIENT);
}
