#include "Join.hpp"

bool Join::exceptionJoin()
{
	if (_curUser.isActive() == false)
	{
		errorToClient("", _parsedCommand[0], "You need to pass first");
		return true;
	}
	if (_parsedCommand.size() < 2 || _parsedCommand.size() > 3)
	{
		errorToClient("461", _parsedCommand[0], "Not enough parameters");
		return true;
	}
	if (_parsedCommand[1][0] != '#')
	{
		errorToClient("", _parsedCommand[0], "channel name have to start with '#'");
		return true;
	}
	return false;
}

Join::Join(std::map<int, UserInfo> &clients, std::vector<Channel> &channels, uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(clients, channels, fd, parsedCommand) {}

Join::~Join() {}

bool Join::checkChMode(int chIdx)
{
	if (_channels[chIdx].checkMode("k") && _channels[chIdx].getKey() != _parsedCommand[2])
	{
		errorToClient("475", _parsedCommand[1], "Cannot join channel (+k)");
		return (true);
	}
	else if (_channels[chIdx].checkMode("l") && _channels[chIdx].getLimit() == _channels[chIdx].getUserCount())
	{
		errorToClient("471", _parsedCommand[1], "Cannot join channel (+l)");
		return (true);
	}
	else if (_channels[chIdx].checkMode("i"))
	{
		errorToClient("473", _parsedCommand[1], "Cannot join channel (+i)");
		return (true);
	}
	return (false);
}

void Join::execute()
{
	int chIdx = -1;
	bool oper = false;
	if (exceptionJoin())
		return ;
	if (_parsedCommand.size() == 2)
		_parsedCommand.push_back("");
	if ((findChannel(_parsedCommand[1])) == -1)
	{
		_channels.push_back(Channel(_parsedCommand[1], _parsedCommand[2]));
		oper = true;
	}
	chIdx = findChannel(_parsedCommand[1]);
	if (checkChMode(chIdx))
		return ;
	_curUser.channels[_parsedCommand[1]] = oper;
	_channels[chIdx].joinChannel(_fd, _curUser);
}
