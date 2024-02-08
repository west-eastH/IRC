
#pragma once

#include "UserAccount.hpp"
#include <sys/socket.h>
#include <vector>
#include <utility>

#define ONLINE true
#define OFFLINE false
#define OPER true
#define MEMBER false

class Channel {
	private:
		int		 	_limit;
		int			_userCount;
		std::string	_name;
		std::string	_key;
		std::string	_topic;
		std::string	_mode;
		std::map< uintptr_t, std::pair<bool, bool> > _members; //fd, oper, online
	public:
		Channel(std::string name, std::string key);
		~Channel();

		const std::string& getName() const;
		const std::string& getKey() const;
		const std::string& getTopic() const;
		const std::string& getMode() const;

		int		getLimit() const;
		int		getUserCount() const;
		int		partChannel(int fd);
		int		chopMember(const std::string& nick, bool op);
		bool	isMemberExists(uintptr_t fd);
		bool	isAdmin(uintptr_t fd);
		bool	checkMode(const std::string& mode);
		void	joinChannel(uintptr_t fd, bool oper, bool status);
		void	setMode(const std::string mode);
		void	setTopic(const std::string topic);
		void	setLimit(const int limit);
		void	setKey(const std::string key);
		void	announce(std::string cmd, std::string params);
		const std::string generateFormattedMemberNames();
};
