#include "Ping.hpp"

Ping::Ping(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(clients, channels, fd, parsedCommand) {}

Ping::~Ping() {}

void Ping::execute()
{
	// int chIdx = -1;
	struct hostent *host_info;

	if (_parsedCommand.size() != 2)
		throw std::runtime_error("409 PING :No origin specified");
	host_info = gethostbyname(_parsedCommand[1].c_str());
	if (std::strcmp(_curUser.getHostName().c_str(), host_info->h_name))
		throw std::runtime_error("402 PING :localhost :No such serverd");
	std::string result = "PONG " + _curUser.getHostName();
	send(_fd, result.c_str(), result.length(), 0);
	// _curUser.channels[_parsedCommand[1]] = oper;
	// _channels[chIdx].joinChannel(_fd, _curUser);
}
