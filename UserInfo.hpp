#pragma once

#include <iostream>
#include <map>
#include <sstream>

class UserInfo{
public:

private:
    bool		_auth;
	bool		_pass;
    bool		_active;
    std::string	_nickName;
    std::string	_realName;
    std::string	_userName;

public:
    UserInfo();
    virtual ~UserInfo();

    std::string sendBuffer;
    std::map<std::string, bool> channels;

    const std::string &getNickName(void) const;
    const std::string &getRealName(void) const;
    const std::string &getUserName(void) const;

    void setNickName(const std::string &nickName);
    void setRealName(const std::string &realName);
    void setUserName(const std::string &userName);

    bool isAuth(void) const;
    bool isActive(void) const;
	bool isPass(void) const;

	void activate(void);
	void deactivate(void);

	void allowAccess(void); 
	void denyAccess(void);

	void authorize(void);
	void deauthorize(void);
};
