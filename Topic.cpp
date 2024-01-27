#include "Topic.hpp"

Topic::Topic(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand, std::string password)
	: Command(clients, channels, fd, parsedCommand), _password(password) {}

Topic::~Topic()
{
}
void Topic::execute()
{
	int chIdx = -1;
	int targetFd;
	std::string msg;

	if (_curUser.isActive() == false)
		throw std::runtime_error("You need to login first");
	if (_parsedCommand.size() < 2 || _parsedCommand.size() > 3)
		throw std::runtime_error("Wrong topic args!");
	if (_curUser.channels.find(_parsedCommand[1]) == _curUser.channels.end())
		throw std::runtime_error("No exist User!!");
	else
	//topic
	/* if ((targetFd = findNick(_parsedCommand[2])) == -1 || _clients[targetFd].channels.find(_parsedCommand[1]) == _clients[targetFd].channels.end())
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
	send(targetFd, msg.c_str(), msg.length(), 0); */
}
