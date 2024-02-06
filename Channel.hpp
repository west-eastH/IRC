
#pragma once

#include "UserInfo.hpp"
#include <sys/socket.h>
#include <vector>

class Channel {
	private:
		std::string	_mode;

		int		 	_limit;
		int			_userCount;
		std::string	_name;
		std::string	_key;
		std::string	_topic;

	public:
		std::map<int, UserInfo*>	_members;
		std::vector<std::string>	inviteMembers; 
		Channel(std::string name, std::string key);
		~Channel();

		const std::string& getName() const;
		const std::string& getKey() const;
		const std::string& getTopic() const;
		const std::string& getMode() const;

		int getLimit() const;
		int getUserCount() const;
		const std::string getMembers();
		void	joinChannel(int fd, UserInfo& user);
		int		partChannel(int fd);
		int		chopMember(const std::string& nick, bool op);
		void	setMode(const std::string mode);
		void	setTopic(const std::string topic);
		void	setLimit(const int limit);
		bool	checkMode(const std::string& mode);
		void	setKey(const std::string key);
};
