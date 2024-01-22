#include "Server.hpp"
const std::string Server::_password = "imrc";
bool Server::isValid(int ac, char **av)
{
    int tempPort;
    if (ac != 3 || av[2] != _password)
        return false;
    tempPort = std::stoi(av[1]);
    if (tempPort < 0 || tempPort > 65535)
        return false;
    _port = tempPort;
    return true;
}

Server::Server(int ac, char **av) : _serverName("Reboot")
{
    if (!isValid(ac, av))
        exit(1);
    create();
}

void Server::create(void)
{
    struct sockaddr_in serverAddr;

    if ((_socketFd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        exit(1); // error
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(_port);
    if (bind(_socketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
        exit(1); // error
    if (listen(_socketFd, 5) == -1)
        exit(1); // error
    fcntl(_socketFd, F_SETFL, O_NONBLOCK);
}

Server::~Server()
{

}


void Server::change_events(std::vector<struct kevent>& changeList, uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
    struct kevent tempEvent;

    EV_SET(&tempEvent, ident, filter, flags, fflags, data, udata);
    changeList.push_back(tempEvent);
}

void Server::start(void) 
{
    int kq;
    if ((kq = kqueue()) == -1)
        exit(1); // error

    std::vector<struct kevent> changeList;
    struct kevent event_list[8];

    change_events(changeList, _socketFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    std::cout << "echo server started" << std::endl;

    int new_events;
    struct kevent* curr_event;

    while (1)
    {
        new_events = kevent(kq, &changeList[0], changeList.size(), event_list, 8, NULL);
        if (new_events == -1)
            exit (1);
        changeList.clear();
        
        for (int i = 0; i < new_events; i++)
        {
            curr_event = &event_list[i];
            if (curr_event->flags & EV_ERROR)
            {
                if (curr_event -> flags == _socketFd)
                    exit (1);
                else
                {
                    std::cerr << "client socket error" << std::endl;
                    disconnect_client(curr_event->ident);
                }
            }
            else if (curr_event->filter == EVFILT_READ)
            {
                if (curr_event->ident == _socketFd)
                    connect_client(changeList);
                else if (clients.find(curr_event->ident) != clients.end())
                    parsing_command(curr_event);
            }
            else if (curr_event->filter == EVFILT_WRITE)
                execute_command(curr_event);
        }
        
    }
}

void Server::connect_client( std::vector<struct kevent> &changeList)
{
    int client_socket;
    if ((client_socket = accept(_socketFd, NULL, NULL)) == -1) 
        exit(1);
    fcntl(client_socket, F_SETFL, O_NONBLOCK);

    change_events(changeList, client_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    change_events(changeList, client_socket, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
    clients[client_socket] = "";
}

void Server::parsing_command(struct kevent* curr_event)
{
    char buf[1024];
    int n = read(curr_event->ident, buf, sizeof(buf));

    if (n<=0)
    {
        if (n < 0)
            exit(1);
        disconnect_client(curr_event->ident);
    }
    else
    {
        buf[n] = '\0';
        clients[curr_event->ident] += buf;
        std::cout << "received data from " << curr_event->ident << ": " << clients[curr_event->ident] << std::endl;
    }
}

void Server::disconnect_client(int client_fd)
{
    std::cout << "asdasd" << std::endl;
    close(client_fd);
    clients.erase(client_fd);
}

void Server::execute_command(struct kevent *curr_event)
{
    std::map<int, std::string>::iterator it = clients.find(curr_event->ident);
    if (it != clients.end())
    {
        if (clients[curr_event->ident] != "")
        {
            int n;
            if ((n = write(curr_event->ident, clients[curr_event->ident].c_str(), clients[curr_event->ident].size()) == -1))
            {
                disconnect_client(curr_event->ident);
            }
            else
                clients[curr_event->ident].clear();
        }
    }
}
