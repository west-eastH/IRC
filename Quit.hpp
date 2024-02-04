#pragma once

#include "Command.hpp"

class Quit : public Command
{
	private:
		bool exceptionQuit();
	public:
		Quit(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand);
		~Quit();

		virtual void execute();
};