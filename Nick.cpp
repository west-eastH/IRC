#include "Nick.hpp"

Nick::Nick(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand)
	: Command(clients, channels, fd, parsedCommand) {}

Nick::~Nick() {}

void Nick::execute()
{
	std::cout <<"im Nick!!!@!@!@!@!" << std::endl;
	// std::string warning = "461 PASS :Not enough parameters";
    //     sendToClient(_fd, warning);
	if (_curUser.isPass() == false)
		throw std::runtime_error("You need to pass first");
	if (_parsedCommand.size() != 2)
		throw std::runtime_error("Wrong Nick args!");
	if (findNick(_parsedCommand[1]) != -1)
		throw std::runtime_error("이미 사용중인 닉네임입니다.");
	std::string tmp = ":" + _curUser.getNickName() + "!" + _curUser.getUserName() + 
		"@localhost NICK " + _parsedCommand[1];
	sendToClient(_fd, tmp);
	_curUser.setNickName(_parsedCommand[1]);
}