#include "Invite.hpp"

Invite::Invite(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(clients, channels, fd, parsedCommand) {}

Invite::~Invite() {}

void Invite::execute()
{
	int chIdx = -1;
	int targetFd;
	std::string msg;

	if (_curUser.isActive() == false)
		throw std::runtime_error("You need to login first");
	if (_parsedCommand.size() != 3)
		throw std::runtime_error("Wrong Invite args!");
	if (_curUser.channels.find(_parsedCommand[1]) == _curUser.channels.end()
		|| _curUser.channels[_parsedCommand[1]] == false)
		throw std::runtime_error("You are not channel operator!!");
	if ((targetFd = findNick(_parsedCommand[2])) == -1 || !_clients[targetFd].isActive())
		throw std::runtime_error("No exist User!!");
	if (_curUser.getNickName() == _clients[targetFd].getNickName())
		throw std::runtime_error("Can not invite yourself!!");
	if (_clients[targetFd].channels.find(_parsedCommand[1]) != _clients[targetFd].channels.end())
		throw std::runtime_error("They already exist on that channel!!");

	_clients[targetFd].channels[_parsedCommand[1]] = false;
	chIdx = findChannel(_parsedCommand[1]);
	_channels[chIdx].joinChannel(targetFd, _clients[targetFd]);
	msg = "You are invited from " + _parsedCommand[1] + "\n";
	send(targetFd, msg.c_str(), msg.length(), 0);
}
