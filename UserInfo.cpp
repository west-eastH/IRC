#include "UserInfo.hpp"

UserInfo::UserInfo() : _pass(false), _active(false), _root(false), _realName("name"), _userName("user")
{
	static int	_nickIdx = 0;
	std::stringstream ss;
    ss << "default" << _nickIdx;
	_nickIdx++;
    std::string nick = ss.str();
    _nickName = nick;
    std::cout << "UserInfo constructor called" << std::endl;
}

UserInfo::~UserInfo() {}

const std::string &UserInfo::getNickName(void) const
{
    return _nickName;
}

const std::string &UserInfo::getRealName(void) const
{
    return _realName;
}

const std::string &UserInfo::getUserName(void) const
{
    return _userName;
}

const std::string &UserInfo::getHostName(void) const
{
	return _hostName;
}

const std::string &UserInfo::getServerName(void) const
{
	return _serverName;
}

bool UserInfo::isActive(void) const
{
    return _active;
}

bool UserInfo::isPass(void) const
{
    return _pass;
}

bool UserInfo::isRoot(void) const
{
	return _root;
}

void UserInfo::setNickName(const std::string &nickName)
{
    _nickName = nickName;
}

void UserInfo::setRealName(const std::string &realName)
{
    _realName = realName;
}

void UserInfo::setUserName(const std::string &userName)
{
    _userName = userName;
}

void UserInfo::setHostName(const std::string &hostName)
{
	_hostName = hostName;
}

void UserInfo::setServerName(const std::string &serverName)
{
	_serverName = serverName;
}

void UserInfo::activate(void)
{
	_active = true;
}

void UserInfo::deactivate(void)
{
	_active = false;
}

void UserInfo::allowAccess(void)
{
	_pass = true;
} 

void UserInfo::denyAccess(void)
{
	_pass = false;
}

void UserInfo::authorize()
{
	_root = true;
}

void UserInfo::deauthorize()
{
	_root = false;
}
