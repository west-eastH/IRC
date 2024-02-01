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
	/* TODO : INVITE MODE */
	//ERR_INVITEONLYCHAN
	
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

// 		471    ERR_CHANNELISFULL
//               "<channel> :Cannot join channel (+l)"
//      473    ERR_INVITEONLYCHAN
//               "<channel> :Cannot join channel (+i)"
// 		475    ERR_BADCHANNELKEY
// 			"<channel> :Cannot join channel (+k)"
// 		332    RPL_TOPIC
//               "<channel> :<topic>"



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
	if (_channels[chIdx].getKey() != _parsedCommand[2])
		errorToClient("475", _parsedCommand[0], "Cannot join channel (+k)");
		throw std::runtime_error("Wrong password!");
	_curUser.channels[_parsedCommand[1]] = oper;
	_channels[chIdx].joinChannel(_fd, _curUser);
}
