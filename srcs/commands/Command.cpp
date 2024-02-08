#include <cstring>
#include "Command.hpp"

Command::Command(uintptr_t fd, std::vector<std::string> parsedCommand) 
	:_fd(fd), _parsedCommand(parsedCommand) {}

Command::~Command() {}

int Command::findNick(const std::string& nick) const
{
	Database* DB = Database::getInstance();
	return DB->search(nick, NICK);
}

int	Command::findChannel(const std::string& name) const
{
	Database* DB = Database::getInstance();
	return DB->search(name, CHANNEL);
}

void Command::sendToClient(int clientFd, std::string cmd, std::string params, bool flag)
{
	std::string prefix;
	std::string success;
	UserAccount sender = Database::getInstance()->getAccount(clientFd);

	if (flag == CLIENT)
	{
		prefix = sender.getNickName() + "!" + sender.getUserName() + "@" + sender.getServerName();
		success = ":" + prefix + " " + cmd + params + "\r\n";
	}
	else
	{
		prefix = sender.getServerName();
		success = ":" + prefix + " " + cmd + " " + sender.getNickName() + " " + params + "\r\n";
	}
    const char *msg = success.c_str();
	int result = send(clientFd, msg, std::strlen(msg), 0);
    if (result == -1)
        throw new std::runtime_error("Error: send failed");
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
		msg += _parsedCommand[i] + " ";
	return msg;
}

uintptr_t Command::getFd() const
{
	return _fd;
}
