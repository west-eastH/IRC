#pragma once

#include "Command.hpp"

class Mode : public Command
{
private:
	bool checkMode(Channel& channel, const std::string& mode);
public:
	Mode(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand);
	~Mode();

	void execute();
};
