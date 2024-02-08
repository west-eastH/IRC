#include "Database.hpp"

Database* Database::_instance = NULL;

Database::Database() {}

Database::~Database() {}

Database* Database::getInstance()
{
	if (_instance == NULL)
		_instance = new Database();
	return _instance;
}

void Database::addAccount(uintptr_t fd)
{
	_accounts[fd].sendBuffer.clear();
}

void Database::addChannel(const std::string& name, const std::string& key)
{
	_channels.push_back(Channel(name, key));
}

UserAccount&	Database::getAccount(uintptr_t fd)
{
	return _accounts[fd];
}

void	Database::deleteAccount(uintptr_t fd)
{
	_accounts.erase(fd);
}

bool	Database::isUserExists(uintptr_t fd)
{
	return _accounts.find(fd) != _accounts.end();
}

int		Database::search(const std::string& target, int type)
{
	if (type == NICK)
	{
		std::map<uintptr_t ,UserAccount>::iterator it;
		for (it = _accounts.begin(); it != _accounts.end(); ++it)
			if (it->second.getNickName() == target)
				return it->first;
	}
	else if (type == CHANNEL)
	{
		for (size_t i = 0; i < _channels.size(); i++)
			if (_channels[i].getName() == target)
				return i;
	}
	return -1;
}

Channel&	getChannel(int idx)
{
	return _channels[idx];
}