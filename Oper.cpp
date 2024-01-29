#include "Oper.hpp"

Oper::Oper(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand, std::string id, std::string password)
	: Command(clients, channels, fd, parsedCommand), _rootId(id), _rootPw(password) {}

Oper::~Oper() {}

void Oper::execute()
{
	if (_curUser.isActive() != true)
		throw std::runtime_error("Activate first!");
	if (_parsedCommand.size() != 3)
		throw std::runtime_error(_parsedCommand[0] + " : Not enough parameters");
	if (_parsedCommand[1] != _rootId || _parsedCommand[2] != _rootPw)
		throw std::runtime_error("incorrect root information!!!");
	if (_curUser.isRoot())
		throw std::runtime_error("You are already root!!!");
	_curUser.authorize();
	//putString(_fd, "You are now root!!!\n");
}
