#include <cstring>
#include "Command.hpp"

Command::Command(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand) 
	:  _parsedCommand(parsedCommand), _clients(clients), _channels(channels), _curUser(clients[fd]), _fd(fd) {}

Command::~Command() {}

int Command::findNick(const std::string& nick) const
{
	std::map<int ,UserInfo>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second.getNickName() == nick)
			return it->first;
	}
	return -1;
}

int	Command::findChannel(const std::string& name) const
{
	for (int i = 0; i < static_cast<int>(_channels.size()); i++)
	{
		if (_channels[i].getName() == name)
			return i;
	}
	return -1;
}

void Command::sendToClient(UserInfo& sender, int clientFd, std::string cmd, std::string params, bool flag)
{
	std::string prefix;
	std::string success;
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

bool Command::isPrintable(const std::string str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (!std::isprint(str[i]))
			return false;
	}
	return true;
}

std::string Command::makeMsg(size_t idx)
{
	std::string msg;
	for (size_t i = idx; i < _parsedCommand.size(); i++)
		msg += _parsedCommand[i] + " ";
	return msg;
}
