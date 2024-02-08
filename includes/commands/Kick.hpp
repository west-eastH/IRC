#pragma once
#include "Server.hpp"
#include "Command.hpp"

class Kick : public Command
{
	private:
		bool handleException();
	public:
		Kick(std::map<int, UserAccount>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand);
		~Kick();
		void execute();
};
