#pragma once

#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/event.h>
#include <map>
#include <string>

class Server{
    private:
        const static std::string _password;
        std::string _serverName;
        int _port;
        int _socketFd;
        void create(void);
        Server();
        bool isValid(int ac, char **av);
        void change_events(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata);

    public:
        // std::map<int, UserInfo> users;
        // std::map<std::string, Channel> channels;
        // char clientBuffer[SOMAXCONN][BUFSIZ];
        // char sendBuffer[SOMAXCONN][BUFSIZ];
        std::map<int, std::string> clients;
        ~Server();
        Server(int ac, char **av);
        void start(void);
        void connect_client( std::vector<struct kevent> &changeList);
        void parsing_command(struct kevent* curr_event);
        void disconnect_client(int client_fd);
        void execute_command(struct kevent* curr_event);
}; 