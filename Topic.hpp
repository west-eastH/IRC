#pragma once

#include "Command.hpp"

class Topic : public Command
{
	public:
		Topic(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> temp_split);
		~Topic();

		void execute();
};