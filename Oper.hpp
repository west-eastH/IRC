#pragma once
#include "Server.hpp"
#include "Command.hpp"

class Oper : public Command
{
	private:
		std::string	_rootId;
		std::string	_rootPw;
	public:
		Oper(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand, std::string id, std::string pw);
		~Oper();
		void execute();
};
