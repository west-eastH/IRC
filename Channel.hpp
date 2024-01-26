
#pragma once

#include "UserInfo.hpp"
#include <sys/socket.h>

class Channel {
private:
	int			_mode;
    int		 	_limit;
    int			_userCount;
    std::string	_name;
    std::string	_key;
    std::string	_topic;
	std::map<int, UserInfo*>	_members;

public:
    Channel(std::string name, std::string key);
    Channel(int mode, const std::string &name, const std::string &key, const std::string &topic, int limit);
    ~Channel();

	const std::string &getName() const;
	const std::string &getKey() const;
	void	joinChannel(int fd, UserInfo& user);
	void	kickMember(int fd);
	void	announce(const std::string msg);
};
