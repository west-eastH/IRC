
#include "Command.hpp"

Command::Command(std::map<int, UserInfo>& clients, uintptr_t fd, std::vector<std::string> temp_split) 
	: _parsedCommand(temp_split), _clients(clients), _fd(fd), _curUser(clients[fd])
{
} 

Command::~Command()
{
}

void Command::putString(int fd, std::string str) const
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
