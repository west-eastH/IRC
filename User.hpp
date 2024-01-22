#ifndef IRC_USER_HPP
#define IRC_USER_HPP

#include <iostream>
#include <map>

class User{
public:

private:
    int fd;
    std::string _nickName;
    std::string _realName;
    std::string _userName;
    bool _auth;
    bool _active;

    std::string _sendBuffer;

public:
    User();
    virtual ~User();

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
    void setSendBuffer(const std::string &sendBuffer);

};

#endif //IRC_USER_HPP
