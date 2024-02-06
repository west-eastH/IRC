#include "Join.hpp"

bool Join::exceptionJoin()
{
	if (_curUser.isActive() == false)
	{
		sendToClient(_curUser, _fd, "", _parsedCommand[0] + " :You need to pass first", SERVER);
		return true;
	}
	if (_parsedCommand.size() < 2 || _parsedCommand.size() > 3)
	{
		sendToClient(_curUser, _fd, "461", " :Not enough parameters", SERVER);
		return true;
	}
	if (_parsedCommand[1][0] != '#')
	{
		sendToClient(_curUser, _fd, "", _parsedCommand[0] + " :channel name have to start with '#'", SERVER);
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
		sendToClient(_curUser, _fd, "475", _parsedCommand[1] + " :Cannot join channel (+k)", SERVER);
		return (true);
	}
	else if (_channels[chIdx].checkMode("l") && _channels[chIdx].getLimit() == _channels[chIdx].getUserCount())
	{
		sendToClient(_curUser, _fd, "471", _parsedCommand[1] + " :Cannot join channel (+l)", SERVER);
		return (true);
	}
	else if (_channels[chIdx].checkMode("i"))
	{
		for (size_t i = 0; i < _channels[chIdx].inviteMembers.size(); i++)
			if (_channels[chIdx].inviteMembers[i] == _curUser.getNickName())
				return false;
		sendToClient(_curUser, _fd, "473", _parsedCommand[1] + " :Cannot join channel (+i)", SERVER);
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
		if (_parsedCommand[2].length())
		{
			std::string currMode = _channels.back().getMode();
			currMode += "k";
			_channels.back().setMode(currMode);
			_channels.back().setKey(_parsedCommand[2]);
		}
		oper = true;
	}
	chIdx = findChannel(_parsedCommand[1]);
	if (!_curUser.isRoot() && checkChMode(chIdx))
		return ;
	if (_curUser.isRoot() || _curUser.channels[_parsedCommand[1]])
		oper = true;
	_curUser.channels[_parsedCommand[1]] = oper;
	std::map<int, UserInfo*>::iterator it;
	_channels[chIdx].joinChannel(_fd, _curUser);
	for (it = _channels[chIdx]._members.begin(); it != _channels[chIdx]._members.end(); ++it)
		sendToClient(_curUser, it->first, _parsedCommand[0], " " + _parsedCommand[1], CLIENT);
	sendToClient(_curUser, _fd, "332", _parsedCommand[1] + " :" +  _channels[chIdx].getTopic(), SERVER);
	std::string members = _channels[chIdx].getMembers();
	sendToClient(_curUser, _fd, "353", "= " + _parsedCommand[1] + " :" + members, SERVER);
	sendToClient(_curUser, _fd, "366", _parsedCommand[1] + " :End of /NAMES list", SERVER);
}
