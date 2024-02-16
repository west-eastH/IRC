#include <cstring>
#include "Command.hpp"

Command::Command(uintptr_t fd, std::vector<std::string> parsedCommand) 
	:_fd(fd), _DB(Database::getInstance()), _parsedCommand(parsedCommand) {}

Command::~Command() {}

int Command::findNick(const std::string& nick) const
{
	return _DB->search(nick, NICK);
}

int	Command::findChannel(const std::string& name) const
{
	return _DB->search(name, CHANNEL);
}

bool Command::isPrintable(const std::string& str)
{
	for (size_t i = 0; i < str.length(); i++)
		if (!std::isprint(str[i]))
			return false;
	return true;
}

std::string Command::makeMessage(size_t idx)
{
	std::string msg;
	for (size_t i = idx; i < _parsedCommand.size(); i++)
	{
		if (i != _parsedCommand.size() - 1)
			msg += _parsedCommand[i] + " ";
		else
			msg += _parsedCommand[i];
	}
	return msg;
}

uintptr_t Command::getFd() const
{
	return _fd;
}
