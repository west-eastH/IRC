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
	if (_parsedCommand.size() < 3)
	{
		std::cout << "=======asdf======== : "  << _parsedCommand.size() << std::endl;
		errorToClient("461", _parsedCommand[0], "Not enough parameters");
		return true;
	}
	if (_parsedCommand[1].front() != '#')
		return true;
	if ((_curUser.channels.find(_parsedCommand[1]) == _curUser.channels.end()
		|| _curUser.channels[_parsedCommand[1]] == false))
	{
		errorToClient("482", _parsedCommand[0], "You are not channel operator!!");
		return true;
	}
	return false;		
}

void Mode::execute()
{
	if (exceptionMode())
		return ;
	if (checkMode(_parsedCommand[2]) == false)
		return ;
	if (checkParams(_parsedCommand[2]) == false)
		return ;
	// if (checkMode(_channels[findChannel(_parsedCommand[1])], _parsedCommand[2]) == false)
	// 	throw std::runtime_error("Invaild mode flag!!");
	chmod(_channels[findChannel(_parsedCommand[1])], _parsedCommand[2]);
}

bool Mode::checkMode(const std::string& mode)
{
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

bool Mode::checkParams(const std::string& mode)
{
	int opCode;
	int requiredParamsCount = 0;
	int commandParamsSize = _parsedCommand.size() - 3;

	for (size_t i = 0; i < mode.length(); i++)
	{
		if (mode[i] == '+' || mode[i] == '-')
			opCode = mode[i];
		else
		{
			if ((opCode == '+' && (mode[i] == 'k' || mode[i] == 'l' || mode[i] == 'o')) || \
				(opCode == '-' && (mode[i] == 'k' || mode[i] == 'o')))
				requiredParamsCount++;
		}
	}
	if (commandParamsSize != requiredParamsCount)
		return false;
	return true;
}

void Mode::chmod(Channel& channel, const std::string& mode)
{
	//============================================TODO===============================================
	int opCode = '+';
	static bool (Mode::*chmodFunc[5])(Channel& channel, const int opCode, const std::string& param) = {&Mode::changeModeI, &Mode::changeModeT, &Mode::changeModeK, &Mode::changeModeL, &Mode::changeModeO};
	const std::string modeList = "itklo";
	int paramIdx = 0;
	bool successChangeMode;
	for (size_t i = 0; i < mode.length(); i++)
	{
		if (mode[i] == '+' || mode[i] == '-')
			opCode = mode[i];
		else
		{
			if ((opCode == '+' && (mode[i] == 'k' || mode[i] == 'l' || mode[i] == 'o')) || \
				(opCode == '-' && (mode[i] == 'k' || mode[i] == 'o')))
				successChangeMode = (this->*chmodFunc[modeList.find(mode[i])])(channel, opCode, _parsedCommand[3 + paramIdx++]);
			else
				successChangeMode = (this->*chmodFunc[modeList.find(mode[i])])(channel, opCode, "");
			if (successChangeMode)
			{
				if (mode[i] == 'o')
					responseToClient("325", _parsedCommand[1], _parsedCommand[2 + paramIdx]);
				else
					responseToClient("324", _parsedCommand[1] + " " + _parsedCommand[2], "");
			}
		}
	}
}

bool Mode::changeModeI(Channel& channel, const int opCode, const std::string& param)
{
	size_t pos = channel.getMode().find('i');
	(void) param;
	if (opCode == '+')
	{
		if (pos != std::string::npos)
			return false;
		std::string currMode = channel.getMode();
		currMode += "i";
		channel.setMode(currMode);
	}
	else
	{
		if (pos == std::string::npos)
			return false;
		std::string currMode = channel.getMode();
		currMode.erase(pos, 1);
		channel.setMode(currMode);
	}
	return true;
}

bool Mode::changeModeT(Channel& channel, const int opCode, const std::string& param)
{
	size_t pos = channel.getMode().find('t');
	(void) param;
	if (opCode == '+')
	{
		if (pos != std::string::npos)
			return false;
		std::string currMode = channel.getMode();
		currMode += "t";
		channel.setMode(currMode);
	}
	else
	{
		if (pos == std::string::npos)
			return false;
		std::string currMode = channel.getMode();
		currMode.erase(pos, 1);
		channel.setMode(currMode);
	}
	return true;
}

bool Mode::changeModeK(Channel& channel, const int opCode, const std::string& param)
{
	size_t pos = channel.getMode().find('k');
	if (opCode == '+')
	{
		if (pos != std::string::npos)
		{
			errorToClient("467", _parsedCommand[1], "Channel key already set");
			return false;
		}
		std::string currMode = channel.getMode();
		currMode += "k";
		channel.setMode(currMode);
		channel.setKey(param);
	}
	else
	{
		if (pos == std::string::npos || channel.getKey() != param)
			return false;
		std::string currMode = channel.getMode();
		currMode.erase(pos, 1);
		channel.setMode(currMode);
		channel.setKey("");
	}
	return true;
}

bool Mode::changeModeL(Channel& channel, const int opCode, const std::string& param)
{
	size_t pos = channel.getMode().find('l');
	if (opCode == '+')
	{
		if (pos != std::string::npos)
			return false;
		for (size_t i = 0; i < param.length(); i++)
			if (std::isdigit(param[i]) == 0)
				return false;
		int limit = std::atoi(param.c_str());
		if (channel.getUserCount() > limit || limit < 1)
			return false;
		channel.setLimit(limit);
		std::string currMode = channel.getMode();
		currMode += "l";
		channel.setMode(currMode);
	}
	else
	{
		if (pos == std::string::npos)
			return false;
		std::string currMode = channel.getMode();
		currMode.erase(pos, 1);
		channel.setMode(currMode);
	}
	return true;
}

bool Mode::changeModeO(Channel& channel, const int opCode, const std::string& param)
{
	bool modeO = opCode == '+' ? true : false;
	int res = channel.chopMember(param, modeO);
	if (res < 0)
	{
		errorToClient("441", param + " " + _parsedCommand[1], "They aren't on that channel");
		return false;
	}
	if (res == 0)
		return false;
	return true;
}
