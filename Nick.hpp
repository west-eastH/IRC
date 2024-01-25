#pragma once

#include "Command.hpp"

class Nick : public Command
{
	public:
		Nick(std::map<int, UserInfo> &clients, uintptr_t fd, std::vector<std::string> temp_split);
		~Nick();

		virtual void execute();
};