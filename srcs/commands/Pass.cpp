#include "Pass.hpp"

Pass::Pass(uintptr_t fd, std::vector<std::string> parsedCommand, std::string password)
	: Command(fd, parsedCommand), _password(password) {}

Pass::~Pass() {}

void Pass::execute()
{
	Database* DB = Database::getInstance();

	if (handleException())
		return ;
	if (_parsedCommand[1] != _password)
		DB->getAccount(_fd).denyAccess();
	else
		DB->getAccount(_fd).allowAccess();
}

bool Pass::handleException()
{
	Database* DB = Database::getInstance();

	if (DB->getAccount(_fd).isActive() == true)
	{
		sendToClient(_fd, "462", " :Unauthorized command (already registered)", SERVER);
		return true;
	}
	if (_parsedCommand.size() != 2)
	{
		sendToClient(_fd, "461", " :Not enough parameters", SERVER);
		return true;
	}
	return false;
}
