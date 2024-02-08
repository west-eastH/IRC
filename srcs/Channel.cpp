
#include "Channel.hpp"
#include "Command.hpp"

Channel::Channel(std::string name, std::string key)
	: _limit(10), _userCount(0), _name(name), _key(key), _topic(""), _mode("t")
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
	Database* DB = Database::getInstance();
	std::map< uintptr_t, std::pair<bool, bool> >::iterator it;
	std::string users;

	for (it = _members.begin(); it != _members.end(); ++it)
	{
		if (it->second.second == false)
			continue;
		if (it->second.first == true)
			users += "@";
		users += DB->getAccount(it->first).getNickName() + " ";
	}
	users.erase(users.find_last_not_of(" ") + 1);
	return users;
}

void Channel::joinChannel(uintptr_t fd, bool oper, bool status)
{
	_members[fd] = std::make_pair(oper, status);
	Database* DB = Database::getInstance();
	DB->getAccount(fd).addChannel(DB->search(_name, CHANNEL));
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
	std::map< uintptr_t, std::pair<bool, bool> >::iterator it;
	for (it = _members.begin(); it != _members.end(); ++it)
	{
		if (Database::getInstance()->getAccount(it->first).getNickName() == nick)
		{
			if (op == _members[it->first].first)
				return 0;
			_members[it->first].first = op;
			return 1;
		}
	}
	return -1;
}

bool	Channel::isMemberExists(uintptr_t fd) 
{
	return (_members.find(fd) != _members.end()) && _members[fd].second;
}

bool	Channel::isAdmin(uintptr_t fd)
{
	return _members[fd].first;
}

void	Channel::announce(std::string cmd, std::string params)
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
		int result = send(it->first, msg, std::strlen(msg), 0);
		if (result == -1)
			throw new std::runtime_error("Error: send failed");
	}
}