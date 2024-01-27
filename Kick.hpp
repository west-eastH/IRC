#pragma once
#include "Server.hpp"
#include "Command.hpp"

class Kick : public Command
{
	public:
		Kick(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand);
		~Kick();
		void execute();
};
