#pragma once

#include "Command.hpp"

class Topic : public Command
{
	private:
		bool exceptionTopic();
		bool printTopic();
		bool checkAuth();
	public:
		Topic(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand);
		~Topic();

		void execute();
};