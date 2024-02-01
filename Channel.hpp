
#pragma once

#include "UserInfo.hpp"
#include <sys/socket.h>

class Channel {
private:
	std::string	_mode;

    int		 	_limit;
    int			_userCount;
    std::string	_name;
    std::string	_key;
    std::string	_topic;
	std::map<int, UserInfo*>	_members;

public:
    Channel(std::string name, std::string key);
    ~Channel();

	const std::string& getName() const;
	const std::string& getKey() const;
	const std::string& getTopic() const;
	const std::string& getMode() const;
	int getLimit() const;
	int getUserCount() const;
	void	joinChannel(int fd, UserInfo& user);
	void	kickMember(int fd);
	int		chopMember(const std::string& nick, bool op);
	void	announce(const std::string msg);

	void	setMode(const std::string mode);
	void	setTopic(const std::string topic);
	void	setLimit(const int limit);
	bool	checkMode(const std::string& mode);
};
