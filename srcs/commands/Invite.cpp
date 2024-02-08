#include "Invite.hpp"

Invite::Invite(uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(fd, parsedCommand) {}

Invite::~Invite() {}

void Invite::execute()
{
	int chIdx = -1;
	int targetFd;
	std::string msg;
	Database *DB = Database::getInstance();

	if (handleException())
		return ;
	targetFd = findNick(_parsedCommand[1]);
	chIdx = findChannel(_parsedCommand[2]);
	DB->getChannel(chIdx).joinChannel(_fd, MEMBER, OFFLINE);
	sendToClient(_fd, "341", _parsedCommand[1] + " " + _parsedCommand[2], SERVER);
	sendToClient(targetFd, _parsedCommand[0], " " + _parsedCommand[1] + " :" + _parsedCommand[2], CLIENT);
}
bool Invite::handleException()
{
	int targetFd;

	UserAccount& curUser = Database::getInstance()->getAccount(_fd);
	if (curUser.isActive() == false)
	{
		sendToClient(_fd, "", _parsedCommand[0] + " :You need to pass first", SERVER);
		return true;		
	}
	if (_parsedCommand.size() != 3)
	{
		sendToClient(_fd, "461", " :Not enough parameters", SERVER);
		return true;
	}
	Channel& curChannel = Database::getInstance()->getChannel(findChannel(_parsedCommand[2]));
	if (curChannel.isMemberExists(_fd) == false || curChannel.isAdmin(_fd) == false)
	{
		sendToClient(_fd, "482", _parsedCommand[2] + " :You're not channel operator", SERVER);
		return true;
	}
	if ((targetFd = findNick(_parsedCommand[1])) == -1 || Database::getInstance()->getAccount(targetFd).isActive() == false)
	{
		sendToClient(_fd, "401", _parsedCommand[1] + " :No such nick/channel", SERVER);
		return true;
	}
	if (curChannel.isMemberExists(targetFd) == true)
	{
		sendToClient(_fd, "443", _parsedCommand[1] + " " + _parsedCommand[2] + " :is already on channel", SERVER);
		return true;	
	}
	return false;
}