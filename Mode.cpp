#include "Mode.hpp"

// +t: 채널의 주제를 오직 운영자만 설정할 수 있게 합니다.
// +i: 채널에 접근을 초대된 사용자에게만 허용합니다.
// +k: 채널에 입장하기 위해 비밀번호가 필요하도록 합니다.

// 사용자 모드의 예는 다음과 같습니다:
// +o: 사용자에게 운영자 권한을 부여합니다.
// l: Set/remove the user limit to channel

Mode::Mode(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand) : Command(clients, channels, fd, parsedCommand) {}

Mode::~Mode() {}

void Mode::execute()
{
	if (_curUser.isActive() != true)
		throw std::runtime_error("Activate first!");
	if (_parsedCommand.size() != 3)
		throw std::runtime_error(_parsedCommand[0] + " : Not enough parameters");
	if (_curUser.channels.find(_parsedCommand[1]) == _curUser.channels.end()
		|| _curUser.channels[_parsedCommand[1]] == false)
		throw std::runtime_error("You are not channel operator!!");
	// if (checkMode(_channels[findChannel(_parsedCommand[1])], _parsedCommand[2]) == false)
	// 	throw std::runtime_error("Invaild mode flag!!");
}

// bool Mode::checkMode(Channel& channel, const std::string& mode)
// {
// 	const std::string modes = "tik";

// 	// if (mode.length() != 2)
// 	// 	return false;
// 	if (mode[0] != '+' && mode[0] != '-')
// 		return false;
// 	if (modes.find(mode[1]) == std::string::npos)
// 		return false;
// 	// if (mode[0] == '+' && mode[1] == 't')
// 	// 	channel.onTMode();
// 	// else if (mode[0] == '+' && mode[1] == 'i')
// 	// 	channel.onIMode();
// 	// else if (mode[0] == '+' && mode[1] == 'k')
// 	// 	channel.onKMode();
// 	// else if (mode[0] == '-' && mode[1] == 't')
// 	// 	channel.offTMode();
// 	// else if (mode[0] == '-' && mode[1] == 'i')
// 	// 	channel.offIMode();
// 	// else
// 	// 	channel.offKMode();
// 	return true;
// }


