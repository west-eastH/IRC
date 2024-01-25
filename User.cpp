#include "User.hpp"

User::User(std::map<int, UserInfo> &clients, uintptr_t fd, std::vector<std::string> temp_split)
	: Command(clients, fd, temp_split)
{
	 std::cout << "User constructor called" << std::endl;
}

User::~User()
{
}

void User::execute()
{
	if (_curUser.isPass() == 0 || _curUser.isActive() == 1)
		throw std::runtime_error("You need to pass first or already actived");

	if (_parsedCommand.size() < 5)
		throw std::invalid_argument("Wrong User args!");
	// <user> <mode> <unused> <realname>
	if (_parsedCommand[4][0] != ':')
		throw std::runtime_error("Wrong realname args!");
	_curUser.setUserName(_parsedCommand[1]);
	_parsedCommand[2] == "0" ? _curUser.deauthorize() : _curUser.authorize();

	std::string realname;
	for (int i = 4; i < (int)_parsedCommand.size(); i++)
		realname += _parsedCommand[i] + " ";
	realname.erase(std::find(realname.begin(), realname.end(), ':'));
	realname.erase(realname.find_last_not_of(" ") + 1);
	_curUser.setRealName(realname);
	_curUser.activate();
	putString(_fd, _curUser.getNickName() + " logined!!!!!\n");
}
