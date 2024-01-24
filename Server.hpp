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

#include <sstream>

#include "UserInfo.hpp"
class Command;
class Server{
    private:
        std::string _serverName;
        const std::string _password;
        int _port;
        uintptr_t _socketFd;
        void create(void);
        Server();
        bool isValid(char *port);
        void change_events(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata);

    public:
        std::map<int, UserInfo> clients;
        ~Server();
        Server(char **av);
        void start(void);
        void connect_client( std::vector<struct kevent> &changeList);
        Command* parsing_command(struct kevent* curr_event);
        void disconnect_client(int client_fd);
        void execute_command(struct kevent* curr_event, Command* cmd);
		std::vector<std::string> split(std::string input, char delimiter);
		Command* createCommand(uintptr_t fd);
};