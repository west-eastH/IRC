#pragma once
#include "Server.hpp"
#include "Command.hpp"
#include <netdb.h>

class Ping : public Command
{
public:
	Ping(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand);
	~Ping();
	void execute();
};
