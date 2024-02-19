#pragma once
#include "Server.hpp"

typedef unsigned long uintptr_t;

class Command
{
	protected:
		uintptr_t					_fd;
		Database*					_DB;
		std::vector<std::string>	_parsedCommand;
	public:

		Command(uintptr_t fd, std::vector<std::string> parsedCommand);
		virtual ~Command();
		uintptr_t		getFd() const;
		int 			findNick(const std::string& nick) const;
		int				findChannel(const std::string& name) const;
		bool 			isPrintable(const std::string& str);
		virtual void 	execute() = 0;
		std::string 	makeMessage(size_t idx);
};
