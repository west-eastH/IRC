#include "Quit.hpp"
bool Quit::exceptionQuit()
{
	if (_curUser.isPass() == false)
	{
		sendToClient(_curUser, _fd, "", _parsedCommand[0] + " :You need to pass first", SERVER);
		return true;
	}
	return false;
}

Quit::Quit(std::map<int, UserAccount> &clients, std::vector<Channel> &channels, uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(clients, channels, fd, parsedCommand) {}

Quit::~Quit() {}

void Quit::execute()
{
	if (exceptionQuit())
		return ;
	std::map<std::string, bool>::iterator iter;
	for (iter = _curUser.channels.begin(); iter != _curUser.channels.end(); ++iter)
	{
		int chIdx = findChannel(iter->first);
		std::map<int, UserAccount*>::iterator it;
		for (it = _channels[chIdx]._members.begin(); it != _channels[chIdx]._members.end(); ++it)
			sendToClient(_curUser, it->first, "PART", " " + iter->first, CLIENT);
		if (_channels[chIdx].partChannel(_fd) == 0)
			_channels.erase(_channels.begin() + chIdx);
	}
	close(_fd);
	_clients.erase(_fd);
}
