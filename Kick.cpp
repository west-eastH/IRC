#include "Kick.hpp"

Kick::Kick(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(clients, channels, fd, parsedCommand) {}

Kick::~Kick() {}

void Kick::execute()
{
	int chIdx = -1;
	int targetFd;
	std::string msg;
	if (_curUser.isActive() == false)
		throw std::runtime_error("You need to login first");
	if (_parsedCommand.size() < 3 || _parsedCommand.size() > 4)
		throw std::runtime_error("Wrong Kick args!");
	if (_curUser.channels.find(_parsedCommand[1]) == _curUser.channels.end()
		|| _curUser.channels[_parsedCommand[1]] == false)
		throw std::runtime_error("You are not channel operator!!");
	if ((targetFd = findNick(_parsedCommand[2])) == -1 || _clients[targetFd].channels.find(_parsedCommand[1]) == _clients[targetFd].channels.end())
		throw std::runtime_error("They aren't on that channel!!");
	if (_curUser.getNickName() == _clients[targetFd].getNickName())
		throw std::runtime_error("Can not kick yourself!!");
	_clients[targetFd].channels.erase(_parsedCommand[1]);
	chIdx = findChannel(_parsedCommand[1]);
	_channels[chIdx].kickMember(targetFd);
	msg = "You got kicked out from " + _parsedCommand[1];
	if (_parsedCommand.size() == 4)
		msg += " (" + _parsedCommand[3] + ")";
	msg += "\n";
	send(targetFd, msg.c_str(), msg.length(), 0);
}
