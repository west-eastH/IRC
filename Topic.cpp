#include "Topic.hpp"

Topic::Topic(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(clients, channels, fd, parsedCommand) {}


Topic::~Topic()
{
}

bool Topic::exceptionTopic()
{
	if (_curUser.isActive() == false)
	{
		sendToClient(_fd, "", "You need to login first", SERVER);
		return true;
	}
	if (_parsedCommand.size() < 2 || _parsedCommand.size() > 3)
	{
		sendToClient(_fd, "461", " :Not enough parameters", SERVER);
		return true;
	}
	if (_curUser.channels.find(_parsedCommand[1]) == _curUser.channels.end())
	{
		sendToClient(_fd, "442", _parsedCommand[1] + " :You're not on that channel" , SERVER);
		return true;
	}
	return false;
}
bool Topic::printTopic(int chIdx)
{
	if (_parsedCommand.size() == 2)
	{
		if (_channels[chIdx].getTopic().length() == 0)
			sendToClient(_fd, "331", _parsedCommand[1] + " :No topic is set", SERVER);
		else
			sendToClient(_fd, "332", _parsedCommand[1] + " :" +  _channels[chIdx].getTopic(), SERVER);
		return true;
	}
	return false;
}

bool Topic::checkAuth(int chIdx)
{
	if (_channels[chIdx].checkMode("t") && _curUser.channels[_parsedCommand[1]] == false)
	{
		sendToClient(_fd, "482", _parsedCommand[1] + " :You're not channel operator", SERVER);
		return true;
	}
	return false;
}

void Topic::execute()
{
		//	 ok 442  -  ERR_NOTONCHANNEL  -  <channel> :You're not on that channel
        //   331  -  RPL_NOTOPIC  -  <channel> :No topic is set
		//   332  -  RPL_TOPIC  -  <channel> :<topic>
        //   ok 482  -  ERR_CHANOPRIVSNEEDED  -  <channel> :You're not channel operator
	int chIdx = -1;
	std::string msg;

	if (exceptionTopic())
		return ;
	
	chIdx = findChannel(_parsedCommand[1]);
	if (printTopic(chIdx))
		return ;

	if (checkAuth(chIdx))
		return ;

	_channels[chIdx].setTopic(_parsedCommand[2]);
	//_channels[chIdx].announce("The channel's topic has been changed to '" + _parsedCommand[2] + "'.\n");
}
