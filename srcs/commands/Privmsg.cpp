#include "Privmsg.hpp"

Privmsg::Privmsg(uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(fd, parsedCommand) {}

Privmsg::~Privmsg() {}

void Privmsg::execute()
{
	if (handleException())
		return ;
	if (_parsedCommand[1].front() == '#')
	{
		int chIdx = findChannel(_parsedCommand[1]);
		Channel& curChannel = Database::getInstance()->getChannel(chIdx);
		curChannel.announce(_fd, _parsedCommand[0], " " + _parsedCommand[1] + " " + makeMessage(2), true);
	}
	else
	{
		int targetFd = findNick(_parsedCommand[1]);
		_DB->sendToClient(_fd, targetFd, _parsedCommand[0], " " + _parsedCommand[1] + " " + makeMessage(2), CLIENT);
	}
}

bool Privmsg::handleException()
{
	UserAccount& curUser = Database::getInstance()->getAccount(_fd);
	if (curUser.isPass() == false)
	{
		_DB->sendToClient(_fd, _fd, "", _parsedCommand[0] + " :You need to pass first", SERVER);
		return true;
	}
	if (_parsedCommand.size() == 1)
	{
		_DB->sendToClient(_fd, _fd, "411", " :No recipient given (" + _parsedCommand[0] + ")", SERVER);
		return true;
	}
	if (_parsedCommand.size() == 2)
	{
		_DB->sendToClient(_fd, _fd, "412", " :No text to send", SERVER);
		return true;
	}
	int targetFd;
	if ((_parsedCommand[1].front() != '#' && ((targetFd = findNick(_parsedCommand[1])) == -1 ||
											  Database::getInstance()->getAccount(targetFd).isActive()) == false) ||
		(_parsedCommand[1].front() == '#' && findChannel(_parsedCommand[1]) == -1))
	{
		_DB->sendToClient(_fd, _fd, "401", _parsedCommand[1] + " :No such nick/channel", SERVER);
		return true;
	}
	Channel& curChannel = Database::getInstance()->getChannel(findChannel(_parsedCommand[1]));
	if (_parsedCommand[1].front() == '#' && curChannel.isMemberExists(_fd) == false)
	{
		_DB->sendToClient(_fd, _fd, "404", _parsedCommand[1] + " :Cannot send to channel", SERVER);
		return true;
	}
	return false;
}