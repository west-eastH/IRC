#pragma once

#include <iostream>
#include <map>

class UserInfo{
public:

private:
    int _fd;
    std::string _nickName;
    std::string _realName;
    std::string _userName;
    bool _auth;
	bool _pass;
    bool _active;

    std::string _sendBuffer;

public:
    UserInfo();
    virtual ~UserInfo();

    std::map<std::string, bool> channels;

    const std::string &getNickName() const;
    const std::string &getRealName() const;
    const std::string &getUserName() const;
    bool isAuth() const;
    bool isActive() const;
    const std::string &getSendBuffer() const;

    void setFd(int fd);
    void setNickName(const std::string &nickName);
    void setRealName(const std::string &realName);
    void setUserName(const std::string &userName);
    void setAuth(bool auth);
    void setActive(bool active);
	void setPass(bool pass);
    void setSendBuffer(const std::string &sendBuffer);

};

