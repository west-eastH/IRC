#include "Join.hpp"

Join::Join(uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(fd, parsedCommand) {}

Join::~Join() {}

bool Join::handleException()
{
	Database* DB = Database::getInstance();

	if (DB->getAccount(_fd).isActive() == false)
	{
		sendToClient(_fd, "", _parsedCommand[0] + " :You need to pass first", SERVER);
		return true;
	}
	if (_parsedCommand.size() < 2 || _parsedCommand.size() > 3)
	{
		sendToClient(_fd, "461", " :Not enough parameters", SERVER);
		return true;
	}
	if (_parsedCommand[1][0] != '#')
	{
		sendToClient(_fd, "", _parsedCommand[0] + " :channel name have to start with '#'", SERVER);
		return true;
	}
	return false;
}


bool Join::checkChMode(int chIdx)
{
	Channel& curChannel = Database::getInstance()->getChannel(chIdx);

	if (curChannel.checkMode("k") && curChannel.getKey() != _parsedCommand[2])
	{
		sendToClient(_fd, "475", _parsedCommand[1] + " :Cannot join channel (+k)", SERVER);
		return (true);
	}
	else if (curChannel.checkMode("l") && curChannel.getLimit() == curChannel.getUserCount())
	{
		sendToClient(_fd, "471", _parsedCommand[1] + " :Cannot join channel (+l)", SERVER);
		return (true);
	}
	else if (curChannel.checkMode("i") && curChannel.isMemberExists(_fd) == false)
	{
		sendToClient(_fd, "473", _parsedCommand[1] + " :Cannot join channel (+i)", SERVER);
		return (true);
	}
	return (false);
}

void Join::execute()
{
	int chIdx = -1;
	Database* DB = Database::getInstance();
	bool oper = DB->getAccount(_fd).isRoot() ? OPER : MEMBER;

	if (handleException())
		return ;
	if (findChannel(_parsedCommand[1]) == -1)
	{
		const std::string key = _parsedCommand.size() == 2 ? "" : _parsedCommand[2];
		DB->addChannel(_parsedCommand[1], key);
		oper = true;
	}
	chIdx = findChannel(_parsedCommand[1]);
	if (DB->getAccount(_fd).isRoot() == false && checkChMode(chIdx))
		return ;
	DB->getChannel(chIdx).joinChannel(_fd, oper, ONLINE);
	DB->getChannel(chIdx).announce(_fd, _parsedCommand[0], _parsedCommand[1], false);
	sendToClient(_fd, "332", _parsedCommand[1] + " :" + DB->getChannel(chIdx).getTopic(), SERVER);
	std::string members = DB->getChannel(chIdx).generateFormattedMemberNames();
	sendToClient(_fd, "353", "= " + _parsedCommand[1] + " :" + members, SERVER);
	sendToClient(_fd, "366", _parsedCommand[1] + " :End of /NAMES list", SERVER);
}
