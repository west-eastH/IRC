#pragma once
#include "Server.hpp"
#include "Command.hpp"

class Part : public Command
{
	public:
		Part(std::map<int, UserAccount>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand);
		~Part();
		void execute();
		bool exceptionPart();
};
