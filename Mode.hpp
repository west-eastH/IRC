#pragma once

#include "Command.hpp"

class Mode : public Command
{
private:
	bool exceptionMode();
	bool checkMode(const std::string& mode);
	void chmod(Channel& channel, const std::string& mode);
public:
	Mode(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand);
	~Mode();

	void execute();
};
