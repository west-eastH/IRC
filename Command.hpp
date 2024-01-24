#pragma once
#include "Server.hpp"

class Command
{
	protected:
		std::vector<std::string>	_parsedCommand;
		std::map<int, UserInfo>&	_clients;
		uintptr_t					_fd;
		UserInfo&					_curUser;
	public:
		Command(std::map<int, UserInfo>& clients, uintptr_t fd, std::vector<std::string> temp_split);
		virtual ~Command();
		virtual void execute() = 0;
};
