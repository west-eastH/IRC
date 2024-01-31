
#include "Channel.hpp"

Channel::Channel(std::string name, std::string key) : _tMode(false), _iMode(false), _kMode(false), _limit(10), _userCount(0), _name(name), _key(key) {

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

const char Channel::getMode() const																						
{
	return _mode;
}

void	Channel::joinChannel(int fd, UserInfo& user)
{
	if (_members.find(fd) != _members.end())
		throw std::runtime_error("You are already in this channel!");
	if (_userCount >= _limit)
		throw std::runtime_error("this channel is full!");
	_members[fd] = &user;
	_userCount++;
	announce(user.getNickName() + " join " + _name + "Channel!\n");
}

void	Channel::kickMember(int fd)
{
	std::map<int, UserInfo*>::iterator it = _members.find(fd);
	if (it == _members.end())
		throw std::runtime_error("No exist member in channel!");
	_members.erase(fd);
	_userCount--;
}

void	Channel::chopMember(const std::string& name, bool op)
{
	
}

void	Channel::announce(const std::string msg)
{
	std::map<int, UserInfo*>::iterator it;
	for (it = _members.begin(); it != _members.end(); ++it)
		send(it->first, msg.c_str(), msg.length(), 0);
}

void	Channel::setTopic(const std::string topic)
{
	_topic = topic;
}

void Channel::setMode(const char mode)
{
	_mode = mode;
}

const std::string& Channel::getTopic() const
{
	return _topic;
}

// void	Channel::onTMode()
// {
// 	_tMode = true;
// }

// void	Channel::onIMode()
// {
// 	_iMode = true;
// }

// void	Channel::onKMode()
// {
// 	_kMode = true;
// }


// void	Channel::offTMode()
// {
// 	_tMode = false;
// }

// void	Channel::offIMode()
// {
// 	_iMode = false;
// }

// void	Channel::offKMode()
// {
// 	_kMode = false;
// }
