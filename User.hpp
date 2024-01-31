#pragma once

#include "Command.hpp"

class User : public Command
{
	private:
		bool exceptionUser();
	public:
		User(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> temp_split);
		~User();

		void execute();
};