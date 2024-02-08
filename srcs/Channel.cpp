
#include "Channel.hpp"
#include "Command.hpp"

Channel::Channel(std::string name, std::string key) 
	: _mode("t"), _limit(10), _userCount(0), _name(name), _key(key), _topic("")
{
	if (key.length())
		_mode += "k";
}

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

const std::string Channel::generateFormattedMemberNames() 
{
	std::map<int, UserAccount *>::iterator it;
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

void	Channel::joinChannel(int fd, bool oper, bool status)
{
	_members[fd] = std::make_pair<oper, status>;
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
	std::map<int, UserAccount*>::iterator it;
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

bool	isMemberExists(uintptr_t fd) const
{
	return _members.find(_fd) != _members.end();
}

void	announce(std::string cmd, std::string params)
{
	std::string prefix;
	std::string success;
	std::map< uintptr_t, std::pair<bool, bool> >::iterator it;

	for (it = _members.begin(); it != _members.end(); ++it)
	{
		UserAccount sender = Database::getInstance()->getAccount(it->first);

		prefix = sender.getNickName() + "!" + sender.getUserName() + "@" + sender.getServerName();
		success = ":" + prefix + " " + cmd + " " + params + "\r\n";
		const char *msg = success.c_str();
		int result = send(clientFd, msg, std::strlen(msg), 0);
		if (result == -1)
			throw new std::runtime_error("Error: send failed");
	}
}