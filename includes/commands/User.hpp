#pragma once

#include "Command.hpp"

class User : public Command
{
	private:
		bool handleException();
		std::string makeRealname();
	public:
		User(std::map<int, UserAccount>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> temp_split);
		~User();

		void execute();
};