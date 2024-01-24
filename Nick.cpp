#include "Nick.hpp"

Nick::Nick(std::map<int, UserInfo> &clients, uintptr_t fd, std::vector<std::string> temp_split)
	: Command(clients, fd, temp_split)
{
	 std::cout << "Nick constructor called" << std::endl;
}

Nick::~Nick()
{
}

void Nick::execute()
{
	if (_curUser.isPass() == 0)
		return ;

	if (_parsedCommand.size() != 2)
		write(_fd, "Wrong Nick args.\n", 17);
	else
	{
		_curUser.setNickName(_parsedCommand[1]);
		std::string msg = "Set nickname : " + _curUser.getNickName() + " ok\n";
		write(_fd, msg.c_str(), msg.length());
	}
}