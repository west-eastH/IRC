#pragma once
#include "Server.hpp"
#include "Command.hpp"

class Join : public Command
{
	private:
		bool handleException();
		bool checkChMode(int chIdx);
	public:
		Join(std::map<int, UserAccount>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand);
		~Join();
		void execute();
};
