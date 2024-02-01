#pragma once

#include "Command.hpp"

class Mode : public Command
{
private:
	bool exceptionMode();
	bool checkMode(const std::string& mode);
	void chmod(Channel& channel, const std::string& mode);

	bool changeModeI(Channel& channel, const int opCode);

	bool changeModeT(Channel& channel, const int opCode);

	bool changeModeK(Channel& channel, const int opCode);

	bool changeModeL(Channel& channel, const int opCode);

	bool changeModeO(Channel& channel, const int opCode);

public:
	Mode(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand);
	~Mode();

	void execute();
};
