#include "Invite.hpp"

Invite::Invite(uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(fd, parsedCommand) {}

Invite::~Invite() {}

void Invite::execute()
{
	int chIdx = -1;
	int targetFd;
	std::string msg;

	if (handleException())
		return ;
	targetFd = findNick(_parsedCommand[1]);
	chIdx = findChannel(_parsedCommand[2]);
	_DB->getChannel(chIdx).join(targetFd, MEMBER, OFFLINE);
	_DB->sendToClient(_fd, _fd, "341", _parsedCommand[1] + " " + _parsedCommand[2], SERVER);
	_DB->sendToClient(_fd, targetFd, _parsedCommand[0], " " + _parsedCommand[1] + " :" + _parsedCommand[2], CLIENT);

// 	127.000.000.001.58206-127.000.000.001.06667: INVITE phan #new

// 127.000.000.001.06667-127.000.000.001.58206: :irc.local 341 nickname phan :#new

// 127.000.000.001.06667-127.000.000.001.38766: :nickname!codespace@127.0.0.1 INVITE phan :#new
}
bool Invite::handleException()
{
	int targetFd;
	
	UserAccount& curUser = _DB->getAccount(_fd);
	if (curUser.isActive() == false)
	{
		_DB->sendToClient(_fd, _fd, "", _parsedCommand[0] + " :You need to pass first", SERVER);
		return true;		
	}
	if (_parsedCommand.size() != 3)
	{
		_DB->sendToClient(_fd, _fd, "461", " :Not enough parameters", SERVER);
		return true;
	}
	if ((findChannel(_parsedCommand[2]) == -1) || (targetFd = findNick(_parsedCommand[1])) == -1 || Database::getInstance()->getAccount(targetFd).isActive() == false)
	{
		_DB->sendToClient(_fd, _fd, "401", _parsedCommand[1] + " :No such nick/channel", SERVER);
		return true;
	}
	Channel& curChannel = _DB->getChannel(findChannel(_parsedCommand[2]));
	if (curChannel.isMemberExists(_fd) == false || curChannel.isAdmin(_fd) == false)
	{
		_DB->sendToClient(_fd, _fd, "482", _parsedCommand[2] + " :You're not channel operator", SERVER);
		return true;
	}
	if (curChannel.isMemberExists(targetFd) == true)
	{
		_DB->sendToClient(_fd, _fd, "443", _parsedCommand[1] + " " + _parsedCommand[2] + " :is already on channel", SERVER);
		return true;	
	}
	return false;
}