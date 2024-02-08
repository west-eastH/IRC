#pragma once
#include <iostream>
#include "UserAccount.hpp"
#include "Channel.hpp"

#define NICK 0
#define CHANNEL 1

class Database
{
private:
	Database();
	static Database*					_instance;
	std::vector<Channel>				_channels;
	std::map<uintptr_t, UserAccount>	_accounts;
	
public:
	~Database();
	static Database* getInstance();
	void	addAccount(uintptr_t fd);
	void	addChannel(const std::string& name, const std::string& key);
	UserAccount&	getAccount(uintptr_t fd);
	Channel&	getChannel(int idx);
	void	deleteAccount(uintptr_t fd);
	bool	isUserExists(uintptr_t fd);
	int		search(const std::string& target, int type);
};
