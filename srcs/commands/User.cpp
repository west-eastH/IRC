#include "User.hpp"

User::User(uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(fd, parsedCommand) {}

User::~User() {}

bool User::handleException()
{
	Database* DB = Database::getInstance();

	if (DB->getAccount(_fd).isPass() == false)
	{
		sendToClient(_fd, "", _parsedCommand[0] + " :You need to pass first", SERVER);
		return true;
	}
	if (DB->getAccount(_fd).isActive() == 1)
	{
		sendToClient(_fd, "462", _parsedCommand[0] + " :Unauthorized command (already registered)", SERVER);
		return true;
	}
	if (_parsedCommand.size() < 5)
	{
		sendToClient(_fd, "461", " :Not enough parameters", SERVER);
		return true;
	}
	if (_parsedCommand[4][0] != ':')
	{
		sendToClient(_fd, "", _parsedCommand[0] + " :Wrong realname arg!", SERVER);
		return true;
	}
	return false;
}

void User::execute()
{
	Database* DB = Database::getInstance();

	if (handleException())
		return ;
	DB->getAccount(_fd).setUserName(_parsedCommand[1]);
	DB->getAccount(_fd).setHostName(_parsedCommand[2]);
	DB->getAccount(_fd).setServerName(_parsedCommand[3]);
	DB->getAccount(_fd).setRealName(makeRealname());
	DB->getAccount(_fd).activate();
	std::string postfix = DB->getAccount(_fd).getNickName() + "!" + DB->getAccount(_fd).getUserName() + "@" + DB->getAccount(_fd).getServerName();
	sendToClient(_fd, "001", "Welcome to the Internet Relay Network " + postfix, SERVER);
	sendToClient(_fd, "002", "Your host is " + DB->getAccount(_fd).getServerName() + ", running version 0.0.1", SERVER);
	sendToClient(_fd, "003", "This server was created 24/02/02", SERVER);
	sendToClient(_fd, "004", DB->getAccount(_fd).getServerName() + " 0.0.1 ", SERVER);
}

std::string User::makeRealname()
{
	std::string realname;

	for (int i = 4; i < (int)_parsedCommand.size(); i++)
		realname += _parsedCommand[i] + " ";
	realname.erase(std::find(realname.begin(), realname.end(), ':'));
	realname.erase(realname.find_last_not_of(" ") + 1);

	return realname;
}