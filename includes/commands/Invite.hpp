#pragma once
#include "Server.hpp"
#include "Command.hpp"

class Invite : public Command
{
	public:
		Invite(std::map<int, UserAccount>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand);
		~Invite();
		void execute();
		bool exceptionInvite();
};
