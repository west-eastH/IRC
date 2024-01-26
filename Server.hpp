#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/event.h>
#include <string>
#include <exception>

#include <sstream>

#include "UserInfo.hpp"
#include "Channel.hpp"

class Command;
class Server{
    private:
        std::string			_serverName;
        const std::string	_password;
        int					_port;
        uintptr_t			_socketFd;

        void create(void);
        bool isValidPort(char* port);
        void changeEvents(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter);

		void		connectClient(std::vector<struct kevent>& changeList);
        void		disconnectClient(int clientFd);
		void		splitBuff(uintptr_t fd, std::vector<std::string>& buff);
        void		executeCommand(struct kevent* currEvent, Command* cmd);
        Command*	parsingCommand(struct kevent& currEvent);
		Command*	createCommand(uintptr_t fd, std::vector<std::string>& buff);
		std::vector<std::string> split(std::string input, char delimiter);

    public:

        Server(char* port, char* password);
        ~Server();

        void	start(void);

        std::map<int, UserInfo>	clients;
		std::vector<Channel>	channels;
};