#include "Command.hpp"

Command::Command(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand) 
	:  _parsedCommand(parsedCommand), _clients(clients), _channels(channels), _curUser(clients[fd]), _fd(fd) {}

Command::~Command() {}

void Command::putString(const int fd, const std::string& str) const
{
	write(fd, str.c_str(), str.length());
}

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

void Command::sendToClient(int clientFd, std::string cmd, std::string params, bool flag)
{
	std::string prefix;
	if (flag == 0) // prefix -> USER
		prefix = _curUser.getNickName() + "!" + _curUser.getUserName() + "@" + _curUser.getServerName();
	else // prefix -> SERVER
		prefix = _curUser.getServerName();

	std::string success = ":" + prefix + " " + cmd + " " + params + "\r\n";
    const char *msg = success.c_str();
    int result = send(clientFd, const_cast<char *>(msg), std::strlen(msg), 0);
    
    if (result == -1)
        throw new std::runtime_error("Error: send failed");
}

void Command::responseToClient(std::string num, std::string cmd, std::string params)
{
	std::string response = num + " " + cmd + " :" + params + "\r\n";
    const char *msg = response.c_str();
    int result = send(_fd, const_cast<char *>(msg), std::strlen(msg), 0);
    
    if (result == -1)
        throw new std::runtime_error("Error: send failed");
}


void Command::errorToClient(std::string errCode, std::string errCmd, std::string msg)
{
	std::string errMsg = errCode + " " + errCmd + " :" + msg + "\r\n";
    const char *resultMsg = errMsg.c_str();
    int result = send(_fd, const_cast<char *>(resultMsg), std::strlen(resultMsg), 0);
    
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
/* 
Command::CommandError::CommandError(std::string errCode, std::string errCmd, std::string msg) : _msg(errCode + " " + errCmd + " :" + msg + "\r\n") {}

Command::CommandError::~CommandError() throw() {}

const char* Command::CommandError::what() const throw()
{
	return _msg.c_str();
}
 */