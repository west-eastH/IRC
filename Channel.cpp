
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
}

void	Channel::joinChannel(int fd, UserInfo& user)
{
	_members[fd] = &user;
	_userCount++;
}

int	Channel::partChannel(int fd)
{
	_members.erase(fd);
	_userCount--;
	return _userCount;
}

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
		}
	}
	return -1;
}