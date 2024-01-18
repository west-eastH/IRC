#pragma once
#include <iostream>
#define  BUFF_SIZE   1024
class Server
{
private:
	int   _port;
	int   _serverFd;
	int   _clientFd;
	int   _clientAddrSize;
	std::string _password;
	std::string _serverName;
	struct sockaddr_in   _serverAddr;
	struct sockaddr_in   _clientAddr;
	Server(const Server& origin);
	Server& operator=(const Server& origin);

public:
	Server(std::string port, std::string password);
	~Server();
	
	char   buffRcv[BUFF_SIZE];
	char   buffSnd[BUFF_SIZE];

	int getPort() const;
	const std::string& getPassword() const;
};

/**
 * Server:
 * 	- accept()
 * 
 * User;
 *  - int:: socketFd
 *  - int:: permils (ex: 744)
 *  - string:: hostName
 *  - string:: serverName
 *  - string:: realName
 *  - string:: userName
 * 	- string:: nickName (max: 9)
 *  + login()
 *  + chat()
 * 
 * Channel;
 * 
 * Relay;
 * 
 * Command
 * 
*/
