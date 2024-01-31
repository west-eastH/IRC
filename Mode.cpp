#include "Mode.hpp"

// +i: 채널에 접근을 초대된 사용자에게만 허용합니다.
// +t: 채널의 주제를 오직 운영자만 설정할 수 있게 합니다.
// +k: 채널에 입장하기 위해 비밀번호가 필요하도록 합니다.
// l: Set/remove the user limit to channel
// · o: Give/take channel operator privilege

// 사용자 모드의 예는 다음과 같습니다:
// +o: 사용자에게 운영자 권한을 부여합니다.

Mode::Mode(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand) : Command(clients, channels, fd, parsedCommand) {}

Mode::~Mode() {}

bool Mode::exceptionMode()
{
	if (_curUser.isActive() != true)
	{
		errorToClient("", _parsedCommand[0], "Activate first!");
		return true;
	}
	if (_parsedCommand.size() < 3 || _parsedCommand.size() > 4)
	{
		errorToClient("461", _parsedCommand[0], "Not enough parameters");
		return true;
	}
	if (_parsedCommand[1].front() == '#' && (_curUser.channels.find(_parsedCommand[1]) == _curUser.channels.end()
		|| _curUser.channels[_parsedCommand[1]] == false))
	{
		errorToClient("482", _parsedCommand[0], "You are not channel operator!!");
		return true;
	}
	return false;		
}

void Mode::execute()
{
/* 
  324    RPL_CHANNELMODEIS
              "<channel> <mode> <mode params>"

       325    RPL_UNIQOPIS
              "<channel> <nickname>" */
	if (exceptionMode())
		return ;
	if (checkMode(_parsedCommand[2]) == false)
		return ;
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

bool Mode::checkMode(const std::string& mode)
{
	/* 	467    ERR_KEYSET
              "<channel> :Channel key already set"
		471    ERR_CHANNELISFULL
              "<channel> :Cannot join channel (+l)"
       472    ERR_UNKNOWNMODE
              "<char> :is unknown mode char to me for <channel>"
       473    ERR_INVITEONLYCHAN
              "<channel> :Cannot join channel (+i)"
		475    ERR_BADCHANNELKEY
			"<channel> :Cannot join channel (+k)"

       441    ERR_USERNOTINCHANNEL
              "<nick> <channel> :They aren't on that channel" */
	const std::string modeList = "itklo+-";
	for (size_t i = 0; i < mode.length(); i++)
	{
		if (modeList.find(mode[i]) == std::string::npos)
		{
			std::string errMsg = mode + "is unknown mode char to me for " + _parsedCommand[1];
			errorToClient("472", mode, errMsg);
			return false;
		}
	}
	return true;

}

void Mode::chmod(Channel& channel, const std::string& mode)
{
	int opCode = '+';
	char currMode = channel.getMode();

	// itklo
	const std::string modeList = "itklo";
	for (size_t i = 0; i < mode.length(); i++)
	{
		if (mode[i] == '+' || mode[i] == '-')
			opCode = mode[i];
		else
		{
			if (mode[i] == 'o')
				channel.
			size_t pos = (modeList.find(mode[i]) + 1);
			if (opCode == '+')
				currMode |= (1 << pos);
			else
				currMode &= ~(1 << pos);
		}
	}
	channel.setMode(currMode);
}
