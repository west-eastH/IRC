#pragma once

#include "Command.hpp"

class Privmsg : public Command
{
	private:
		bool exceptionPrivmsg();
	public:
		Privmsg(std::map<int, UserAccount>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand);
		~Privmsg();

		virtual void execute();
};