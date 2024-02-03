
#include "Channel.hpp"
#include "Command.hpp"

Channel::Channel(std::string name, std::string key) : _mode("t"), _limit(10), _userCount(0), _name(name), _key(key), _topic("") {}

Channel::~Channel() {}

const std::string &Channel::getName() const 
{
	return _name;
}

const std::string &Channel::getKey() const
{
	return _key;
}

const std::string& Channel::getMode() const
{
	return _mode;
}

int Channel::getLimit() const
{
	return _limit;
}

int Channel::getUserCount() const
{
	return _userCount;
}

const std::string Channel::getMembers() 
{
	std::map<int, UserInfo *>::iterator it;
	std::string users;
	for (it = _members.begin(); it != _members.end(); ++it)
	{
		if (it != _members.begin())
			users += " ";
		if (it->second->channels[_name] == true)
			users += "@";
		users += it->second->getNickName();
	}
	return users;
	// TODO :: getMembers()
}

void	Channel::joinChannel(int fd, UserInfo& user)
{
	// if (_members.find(fd) != _members.end())
	// 	throw std::runtime_error("You are already in this channel!");
	
	//if (_userCount >= _limit)
	//	throw std::runtime_error("this channel is full!");
	_members[fd] = &user;
	_userCount++;
	
/*
	
	announce(user.getNickName() + " join " + _name + " Channel!\r\n");*/
}

void	Channel::kickMember(int fd)
{
	// std::map<int, UserInfo*>::iterator it = _members.find(fd);
	_members.erase(fd);
	_userCount--;
}
/*
void	Channel::announce(const std::string msg)
{
	std::string prefix;
	std::string success;

	prefix = _curUser.getServerName();
	success = ":" + prefix + " " + _name + " " + params + "\r\n";
	std::map<int, UserInfo*>::iterator it;
	for (it = _members.begin(); it != _members.end(); ++it)
		send(it->first, msg.c_str(), msg.length(), 0);
}
*/
void	Channel::setTopic(const std::string topic)
{
	_topic = topic;
}

bool Channel::checkMode(const std::string &mode)
{
	if (_mode.find(mode) != std::string::npos)
		return (true);
	return (false);
}

void Channel::setMode(const std::string mode)
{
	_mode = mode;
}

void Channel::setLimit(const int limit)
{
	_limit = limit;
}

void Channel::setKey(const std::string key)
{
	if (key == "")
		_key.clear();
	else
		_key = key;
}

const std::string& Channel::getTopic() const
{
	return _topic;
}

int	Channel::chopMember(const std::string& nick, bool op) 
{
	std::map<int, UserInfo*>::iterator it;
	for (it = _members.begin(); it != _members.end(); ++it)
	{
		if (it->second->getNickName() == nick)
		{
			if (op == it->second->channels[_name])
				return 0;
			it->second->channels[_name] = op;
			return 1;
		}	// return it->first;
	}
	return -1;
}