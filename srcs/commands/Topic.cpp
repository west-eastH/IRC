#include "Topic.hpp"

Topic::Topic(uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(fd, parsedCommand) {}

Topic::~Topic() {}

bool Topic::handleException()
{
	UserAccount& curUser = Database::getInstance()->getAccount(_fd);
	if (curUser.isActive() == false)
	{
		sendToClient(_fd, "", "You need to login first", SERVER);
		return true;
	}
	if (_parsedCommand.size() < 2 || _parsedCommand.size() > 3)
	{
		sendToClient(_fd, "461", " :Not enough parameters", SERVER);
		return true;
	}
	Channel& curChannel = Database::getInstance()->getChannel(findChannel(_parsedCommand[1]));
	if (curChannel.isMemberExists(_fd))
	{
		sendToClient(_fd, "442", _parsedCommand[1] + " :You're not on that channel" , SERVER);
		return true;
	}
	return false;
}
bool Topic::printTopic(int chIdx)
{
	Channel &curChannel = Database::getInstance()->getChannel(chIdx);
	if (_parsedCommand.size() == 2)
	{
		if (curChannel.getTopic().length() == 0)
			sendToClient(_fd, "331", _parsedCommand[1] + " :No topic is set", SERVER);
		else
			sendToClient(_fd, "332", _parsedCommand[1] + " :" +  curChannel.getTopic(), SERVER);
		return true;
	}
	return false;
}

bool Topic::checkAuth(int chIdx)
{
	Channel &curChannel = Database::getInstance()->getChannel(chIdx);
	if (curChannel.checkMode("t") && curChannel.isAdmin(_fd) == false)
	{
		sendToClient( _fd, "482", _parsedCommand[1] + " :You're not channel operator", SERVER);
		return true;
	}
	return false;
}

void Topic::execute()
{
	int chIdx = -1;
	std::string msg;

	if (handleException())
		return ;
	chIdx = findChannel(_parsedCommand[1]);
	if (printTopic(chIdx))
		return ;

	if (checkAuth(chIdx))
		return ;
	Channel &curChannel = Database::getInstance()->getChannel(chIdx);
	std::string topic = makeMessage(2);
	size_t	pos = topic.find(':');
	if (pos != std::string::npos)
		topic = topic.erase(pos, 1);
	curChannel.setTopic(topic);
	std::map<int, UserAccount*>::iterator it;
	curChannel.announce(_parsedCommand[0], " " + _parsedCommand[1] + " :" + curChannel.getTopic());
}
