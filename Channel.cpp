
#include "Channel.hpp"

Channel::Channel(std::string name, std::string key) : _mode(0), _limit(2), _userCount(1), _name(name), _key(key) {
	_mode = 0;
}

Channel::Channel(int mode, const std::string &name, const std::string &key, const std::string &topic, int limit) 
	: _mode(mode), _limit(limit), _userCount(1), _name(name), _key(key), _topic(topic) {}

Channel::~Channel() {}

const std::string &Channel::getName() const 
{
	return _name;
}

const std::string &Channel::getKey() const
{
	return _key;
}

void	Channel::joinChannel(int fd, UserInfo& user)
{
	if (_userCount > _limit)
	{
		char msg;
		msg = _userCount + '0';
		send(fd, &msg, 1, 0);
		throw std::runtime_error("this channel is full!");
	}
	_members[fd] = user;
	_userCount++;
	announce(user.getNickName() + " join " + _name + "Channel!\n");
}

void	Channel::kickMember(int fd)
{
	std::map<int, UserInfo>::iterator it = _members.find(fd);
	if (it != _members.end())
		throw std::runtime_error("No exist member in channel!");
	_members.erase(fd);
	_userCount--;
}

void	Channel::announce(const std::string msg)
{
	std::map<int, UserInfo>::iterator it;
	for (it = _members.begin(); it != _members.end(); ++it)
		send(it->first, msg.c_str(), msg.length(), 0);
}