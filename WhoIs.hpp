#pragma once

#include "Command.hpp"

class WhoIs : public Command
{
private:
	bool exceptionWhoIs();

public:
	WhoIs(std::map<int, UserInfo> &clients, std::vector<Channel> &channels, uintptr_t fd, std::vector<std::string> parsedCommand);
	~WhoIs();

	virtual void execute();
};