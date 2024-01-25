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
		putString(_fd, "Wrong Nick args.\n");
	else
	{
		if (findNick(_parsedCommand[1]) != -1)
		{
			putString(_fd, "이미 사용중인 닉네임입니다.\n");
			return ;
		}
		_curUser.setNickName(_parsedCommand[1]);
		std::string msg = "Set nickname : " + _curUser.getNickName() + " ok\n";
		putString(_fd, msg.c_str());
	}
}