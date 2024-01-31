#pragma once
#include "Server.hpp"
#include "Command.hpp"

class Join : public Command
{
	private:
		bool exceptionJoin();
	public:
		Join(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand);
		~Join();
		void execute();
};
