#include "Irc.hpp"

Irc::Irc(std::string port, std::string password)
{
	int temp_port;
	if (port.length() > 5 || port.length() == 0)
		throw 1;
	temp_port = std::atoi(port.c_str());
	if (temp_port < 0 || temp_port > 65535)
		throw 1;
	_port = temp_port;
	_password = password;
}

Irc(const Irc& origin)
{
	
}

Irc &operator=(const Irc& origin)
{
	
}

~Irc()
{
	
}

getPort()
{
	
}

getPassword()
{
	
}
