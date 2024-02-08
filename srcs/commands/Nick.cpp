#include "Nick.hpp"

Nick::Nick(uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(fd, parsedCommand) {}

Nick::~Nick() {}

bool Nick::handleException()
{
	Database* DB = Database::getInstance();

	if (DB->getAccount(_fd).isPass() == false)
	{
		sendToClient(_fd, "", _parsedCommand[0] + " :You need to pass first", SERVER);
		return true;
	}
	if (_parsedCommand.size() != 2)
	{
		sendToClient(_fd, "431", " :No nickname given", SERVER);
		return true;
	}
	if (isPrintable(_parsedCommand[1]) == false || _parsedCommand[1].length() > 9)
	{
		sendToClient(_fd, "432", _parsedCommand[1] + " :Erroneous nickname", SERVER);
		return true;
	}
	if (findNick(_parsedCommand[1]) != -1)
	{
		sendToClient(_fd, "433", _parsedCommand[1] + " :Nickname is already in use", SERVER);
		return true;
	}
	return false;
}

void Nick::execute()
{
	Database* DB = Database::getInstance();

	if (handleException())
		return ;
	sendToClient(_fd, _parsedCommand[0], " :" + _parsedCommand[1], CLIENT);
	DB->getAccount(_fd).setNickName(_parsedCommand[1]);
}
