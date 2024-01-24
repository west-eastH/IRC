#include "UserInfo.hpp"

const std::string &UserInfo::getNickName() const {
    return _nickName;
}

const std::string &UserInfo::getRealName() const {
    return _realName;
}

const std::string &UserInfo::getUserName() const {
    return _userName;
}

bool UserInfo::isAuth() const {
    return _auth;
}

bool UserInfo::isActive() const {
    return _active;
}

const std::string &UserInfo::getSendBuffer() const {
    return _sendBuffer;
}

UserInfo::UserInfo() : _nickName(""), _realName(""), _auth(false), _active(false)
{
    std::cout << "UserInfo constructor called" << std::endl;
}

UserInfo::~UserInfo() {

}

void UserInfo::setFd(int fd) {
    _fd = fd;
}

void UserInfo::setNickName(const std::string &nickName) {
    _nickName = nickName;
}

void UserInfo::setRealName(const std::string &realName) {
    _realName = realName;
}

void UserInfo::setUserName(const std::string &userName) {
    _userName = userName;
}

void UserInfo::setAuth(bool auth) {
    _auth = auth;
}

void UserInfo::setActive(bool active) {
    _active = active;
}

void UserInfo::setPass(bool pass) {
    _pass = pass;
}

void UserInfo::setSendBuffer(const std::string &sendBuffer) {
    _sendBuffer = sendBuffer;
}
