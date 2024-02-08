#pragma once

#include "Command.hpp"

class Nick : public Command
{
	private:
		bool handleException();
	public:
		Nick(std::map<int, UserAccount>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand);
		~Nick();

		virtual void execute();
};
