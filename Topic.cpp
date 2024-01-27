#include "Topic.hpp"

Topic::Topic(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(clients, channels, fd, parsedCommand) {}


Topic::~Topic()
{
}
void Topic::execute()
{
	int chIdx = -1;
	std::string msg;

	if (_curUser.isActive() == false)
		throw std::runtime_error("You need to login first");
	if (_parsedCommand.size() < 2 || _parsedCommand.size() > 3)
		throw std::runtime_error("Wrong topic args!");
	if (_curUser.channels.find(_parsedCommand[1]) == _curUser.channels.end())
		throw std::runtime_error("No exist User!!");
	chIdx = findChannel(_parsedCommand[1]);
	if (_parsedCommand.size() == 2)
	{
		msg = _parsedCommand[1] + "'s topic is [" + _channels[chIdx].getTopic() + "]\n";
		send(_fd, msg.c_str(), msg.length(), 0);
		return ;
	}
	if (_curUser.channels[_parsedCommand[1]] == false)
		throw std::runtime_error("You are not channel operator!!");
	_channels[chIdx].setTopic(_parsedCommand[2]);
	_channels[chIdx].announce("The channel's topic has been changed to '" + _parsedCommand[2] + "'.\n");
}
