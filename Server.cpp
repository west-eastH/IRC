#include "Server.hpp"
//Command 객체로 이동해야함
std::vector<std::string> Server::split(std::string input, char delimiter) {
    std::vector<std::string> answer;
    std::stringstream ss(input);
    std::string temp;
 
    while (getline(ss, temp, delimiter)) {
        answer.push_back(temp);
    }
 
    return answer;
}

bool Server::isValid(char *port)
{
    int tempPort;
    tempPort = std::stoi(port);
    if (tempPort < 0 || tempPort > 65535)
        return false;
    _port = tempPort;
    return true;
}

Server::Server(char **av) : _serverName("Reboot"), _password(av[2])
{
    if (!isValid(av[1]))
        exit(1);
    create();
}

void Server::create(void)
{
    struct sockaddr_in serverAddr;

    if ((_socketFd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        exit(1);
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(_port);
    if (bind(_socketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
        exit(1);
    if (listen(_socketFd, 5) == -1)
        exit(1);
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
    clients[client_socket].setSendBuffer("");
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
        clients[curr_event->ident].setSendBuffer(clients[curr_event->ident].getSendBuffer() + buf);
		std::vector<std::string> temp_split = this->split(clients[curr_event->ident].getSendBuffer(), ' ');
		if (clients[curr_event->ident].isActive() == 0)
		{
			if (temp_split.begin()->compare("PASS") == 0)
				std::cout << "test" << std::endl; //Command 객체 checkPass()
			else
				std::cout << "test2" << std::endl; //Pass먼저 해야한다 아니면 실제 irc에서 에러출력 보고 적용
		}
		else
		{
			if (temp_split.begin()->compare("PASS") == 0)
				//이제 각자 어쩌고 저쩌고 파싱
			else if (temp_split.begin()->compare("JOIN") == 0)
				//이제 각자 어쩌고 저쩌고 파싱
			//etc..
		}
		//std::cout << "received data from " << curr_event->ident << ": " << clients[curr_event->ident].getSendBuffer() << std::endl;
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
    std::map<int, User>::iterator it = clients.find(curr_event->ident);
    if (it != clients.end())
    {
        if (clients[curr_event->ident].getSendBuffer() != "")
        {
            int n;
            if ((n = write(curr_event->ident, clients[curr_event->ident].getSendBuffer().c_str(), clients[curr_event->ident].getSendBuffer().size()) == -1))
            {
                disconnect_client(curr_event->ident);
            }
            else
                clients[curr_event->ident].setSendBuffer("");
        }
    }
}
