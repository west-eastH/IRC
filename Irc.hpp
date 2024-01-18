#pragma once
#include <iostream>

class Irc
{
private:
	int _port;
	std::string _password;
public:
	Irc();
	Irc(std::string port, std::string password);
	Irc(const Irc& origin);
	Irc &operator=(const Irc& origin);
	~Irc();
	getPort();
	getPassword();
};
