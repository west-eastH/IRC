#pragma once

#include <iostream>
#include <map>
#include <sstream>

class UserInfo{
public:

private:
	bool		_pass;
    bool		_active;
	bool		_root;
    std::string	_nickName;
    std::string	_realName;
    std::string	_userName;
	std::string _hostName;

public:
    UserInfo();
    virtual ~UserInfo();

    std::string sendBuffer;
    std::map<std::string, bool> channels;

    const std::string &getNickName(void) const;
    const std::string &getRealName(void) const;
    const std::string &getUserName(void) const;
	const std::string &getHostName(void) const;

    void setNickName(const std::string &nickName);
    void setRealName(const std::string &realName);
    void setUserName(const std::string &userName);
	void setHostName(const std::string &hostName);

    bool isActive(void) const;
	bool isPass(void) const;
	bool isRoot(void) const;

	void activate(void);
	void deactivate(void);

	void allowAccess(void); 
	void denyAccess(void);

	void authorize(void);
	void deauthorize(void);
};
