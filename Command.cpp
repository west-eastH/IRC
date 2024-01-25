
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