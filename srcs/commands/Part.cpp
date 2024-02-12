#include "Part.hpp"

Part::Part(uintptr_t fd, std::vector<std::string> parsedCommand) 
	: Command(fd, parsedCommand) {}

Part::~Part() {}

bool Part::handleException()
{
	int channelIdx = findChannel(_parsedCommand[1]);
	if (channelIdx == -1)
	{
		sendToClient(_fd, "403", _parsedCommand[1] + " :No such Channel", SERVER);
		return true;
	}
	Channel& curChannel = Database::getInstance()->getChannel(channelIdx);
	UserAccount& curUser = Database::getInstance()->getAccount(_fd);
	if (curUser.isActive() == false)
	{
		sendToClient(_fd, "", _parsedCommand[0] + " :You need to pass first", SERVER);
		return true;
	}
	if (_parsedCommand.size() != 2)
	{
		sendToClient(_fd, "461", " :Not enough parameters", SERVER);
		return true;
	}
	if (curChannel.isMemberExists(_fd) == false)
	{
		sendToClient(_fd, "442", _parsedCommand[1] + " :You're not on that channel", SERVER);
		return true;
	}
	return false;
}

void Part::execute()
{
	int chIdx = -1;
	std::string msg;

	if (handleException())
		return ;
	chIdx = findChannel(_parsedCommand[1]);
	Channel &curChannel = Database::getInstance()->getChannel(chIdx);
	curChannel.announce(_fd, _parsedCommand[0], " " + _parsedCommand[1], false);
	if (curChannel.partChannel(_fd) == 0)
		Database::getInstance()->deleteChannel(chIdx);
}
