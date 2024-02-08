#include "Ping.hpp"

Ping::Ping(uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(fd, parsedCommand) {}

Ping::~Ping() {}

void Ping::execute()
{
	if (handleException())
		return ;
	pong();
}

bool Ping::handleException()
{
	Database* DB = Database::getInstance();

	struct hostent *host_info;
	if (_parsedCommand.size() != 2)
	{
		sendToClient(_fd, "409", _parsedCommand[0] + " :No origin specified", SERVER);
		return true;
	}
	host_info = gethostbyname(_parsedCommand[1].c_str());
	if (std::strcmp(DB->getAccount(_fd).getServerName().c_str(), host_info->h_name))
	{
		sendToClient(_fd, "402",  _parsedCommand[0] + " :" + _parsedCommand[1] + " :No such serverd", SERVER);
		return true;
	}
	return false;
}

void Ping::pong()
{
	Database* DB = Database::getInstance();

	std::string result = "PONG " + DB->getAccount(_fd).getServerName() + "\r\n";
	 int n = send(_fd, result.c_str(), result.length(), 0);
	 std::cout << result << std::endl;
    if (n == -1)
        throw new std::runtime_error("Error: send failed");
}
