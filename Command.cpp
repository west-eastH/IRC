
#include "Command.hpp"

Command::Command(std::map<int, UserInfo>& clients, uintptr_t fd, std::vector<std::string> temp_split) 
	: _parsedCommand(temp_split), _clients(clients), _fd(fd), _curUser(clients[fd])
{
} 

Command::~Command()
{
}
