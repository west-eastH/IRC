#pragma once
#include "Server.hpp"

class Command
{
	protected:
		uintptr_t					_fd;
		std::vector<std::string>	_parsedCommand;
	public:

		Command(uintptr_t fd, std::vector<std::string> parsedCommand);
		virtual ~Command();
		
		int findNick(const std::string& nick) const;
		int	findChannel(const std::string& name) const;
		void sendToClient(int clientFd, std::string cmd, std::string params, bool flag);
		bool isPrintable(const std::string& str);
		virtual void execute() = 0;
		std::string makeMsg(size_t idx);
};
