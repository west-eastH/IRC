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
#include <vector>

#include <sstream>

#include "UserAccount.hpp"
#include "Channel.hpp"
#include "Database.hpp"


class Command;
class Server{
    private:
        std::string			_serverName;
        const std::string	_password;
		const std::string	_rootId;
		const std::string	_rootPw;
        int					_port;
        uintptr_t			_socketFd;
		Database*			_DB;

        void create(void);
        bool isValidPort(char* port);
        void changeEvents(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter);

		void		connectClient(std::vector<struct kevent>& changeList);
        void		disconnectClient(int clientFd);
		//void		splitBuff(uintptr_t fd, std::vector<std::string>& buff);
		std::vector<std::string> splitBuff(std::string& sendBuffer);
		Command*	createCommand(uintptr_t fd, std::vector<std::string>& buff);
        std::vector<Command*> parsingCommand(struct kevent& currEvent);
		std::vector<std::string> splitSpace(std::string& st);

    public:
        Server(char* port, char* password);
        ~Server();

        void	start(void);

        //std::map<int, UserAccount>	clients;
		//std::vector<Channel>	channels;
};