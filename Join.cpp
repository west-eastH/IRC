#include "Join.hpp"

Join::Join(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(clients, channels, fd, parsedCommand) {}

Join::~Join() {}

void Join::execute()
{
	int chIdx = -1;

	if (_curUser.isActive() == false)
		throw std::runtime_error("You need to login first");
	if (_parsedCommand.size() < 2 || _parsedCommand.size() > 3)
		throw std::runtime_error("Wrong Nick args!");
	if (_parsedCommand[1][0] != '#')
		throw std::runtime_error("Channel name have to start with '#'");
	if (_parsedCommand.size() == 2)
		_parsedCommand.push_back("");
	if ((findChannel(_parsedCommand[1])) == -1)
		_channels.push_back(Channel(_parsedCommand[1], _parsedCommand[2]));
	chIdx = findChannel(_parsedCommand[1]);
	if (_channels[chIdx].getKey() != _parsedCommand[2])
		throw std::runtime_error("Wrong password!");
	_channels[chIdx].joinChannel(_fd, _curUser);
}
