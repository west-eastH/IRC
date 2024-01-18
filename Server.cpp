#include "Server.hpp"

Server::Server(std::string port, std::string password)
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


~Server()
{
	
}

int getPort() const 
{
	return _port;
}

const std::string& getPassword() const
{
	return _password;
}
