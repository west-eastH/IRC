#include "User.hpp"

User::User(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(clients, channels, fd, parsedCommand) 
{
	 std::cout << "User constructor called" << std::endl;
}

User::~User()
{
}

void User::execute()
{
	std::cout <<"im User!!!@!@!@!@!" << std::endl;
	if (/* _curUser.isPass() == 0 ||  */_curUser.isActive() == 1)
		throw std::runtime_error("You need to pass first or already actived");
	if (_parsedCommand.size() < 5)
		throw std::invalid_argument("Wrong User args!");
	// USER <user> <mode> <unused> <realname>
	if (_parsedCommand[4][0] != ':')
		throw std::runtime_error("Wrong realname args!");
	_curUser.setUserName(_parsedCommand[1]);

	std::string realname;
	for (int i = 4; i < (int)_parsedCommand.size(); i++)
		realname += _parsedCommand[i] + " ";
	realname.erase(std::find(realname.begin(), realname.end(), ':'));
	realname.erase(realname.find_last_not_of(" ") + 1);
	_curUser.setRealName(realname);
	_curUser.activate();
	sendToClient(_fd, "001 USER :Welcome to the Internet Relay Network");
}