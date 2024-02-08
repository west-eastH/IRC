#pragma once

#include "Command.hpp"

class Pass : public Command
{
	private:
		std::string _password;
		bool	handleException();
	public:
		Pass(std::map<int, UserAccount>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand, std::string password);
		~Pass();
		void	execute();
};
