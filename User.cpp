#include "User.hpp"

const std::string &User::getNickName() const {
    return _nickName;
}

const std::string &User::getRealName() const {
    return _realName;
}

const std::string &User::getUserName() const {
    return _userName;
}

bool User::isAuth() const {
    return _auth;
}

bool User::isActive() const {
    return _active;
}

const std::string &User::getSendBuffer() const {
    return _sendBuffer;
}

User::User() : _nickName(""), _realName(""), _auth(false), _active(false)
{
    std::cout << "User constructor called" << std::endl;
}

User::~User() {

}

void User::setFd(int fd) {
    User::fd = fd;
}

void User::setNickName(const std::string &nickName) {
    _nickName = nickName;
}

void User::setRealName(const std::string &realName) {
    _realName = realName;
}

void User::setUserName(const std::string &userName) {
    _userName = userName;
}

void User::setAuth(bool auth) {
    _auth = auth;
}

void User::setActive(bool active) {
    _active = active;
}

void User::setSendBuffer(const std::string &sendBuffer) {
    User::_sendBuffer = sendBuffer;
}
