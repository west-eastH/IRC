#pragma once

#include "Command.hpp"

class Nick : public Command
{
	private:
		bool exceptionNick();
	public:
		Nick(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand);
		~Nick();

		virtual void execute();
};