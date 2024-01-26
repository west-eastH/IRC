#include "Server.hpp"
#include "Pass.hpp"
#include "Nick.hpp"
#include "User.hpp"
#include "Join.hpp"

Server::Server(char* port, char* password) : _serverName("Reboot"), _password(password)
{
    if (!isValidPort(port))
        throw std::invalid_argument("Wrong port!");
    create();
}

Server::~Server() {}

std::vector<std::string> Server::split(std::string input, char delimiter)
{
    std::vector<std::string> answer;
    std::stringstream ss(input);
    std::string temp;
 
    while (getline(ss, temp, delimiter))
        answer.push_back(temp); 
    return answer;
}

bool Server::isValidPort(char *port)
{
    int tempPort;
    tempPort = std::stoi(port);
    if (tempPort < 0 || tempPort > 65535)
        return false;
    _port = tempPort;
    return true;
}

void Server::create(void)
{
    struct sockaddr_in serverAddr;

    if ((_socketFd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        throw std::runtime_error("server socket error");
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(_port);
    if (bind(_socketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
        throw std::runtime_error("server bind error");
    if (listen(_socketFd, 10) == -1)
        throw std::runtime_error("server listen error");
    fcntl(_socketFd, F_SETFL, O_NONBLOCK);
}


void Server::changeEvents(std::vector<struct kevent>& changeList, uintptr_t ident, int16_t filter)
{
    struct kevent tempEvent;

    EV_SET(&tempEvent, ident, filter, EV_ADD | EV_ENABLE, 0, 0, NULL);
    changeList.push_back(tempEvent);
}

void Server::start(void) 
{
    int kq = 0;
    int newEvents = 0;
	Command* cmd = NULL;
    struct kevent	eventList[8];
    struct kevent*	currEvent = NULL;
    std::vector<struct kevent> changeList;

    if ((kq = kqueue()) == -1)
        throw std::runtime_error("kqueue error");

    changeEvents(changeList, _socketFd, EVFILT_READ);
    std::cout << "echo server started" << std::endl;

    while (1)
    {
        newEvents = kevent(kq, &changeList[0], changeList.size(), eventList, 8, NULL);
        if (newEvents == -1)
            throw std::runtime_error("event error");
        changeList.clear();
        for (int i = 0; i < newEvents; i++)
        {
            currEvent = &eventList[i];
            if (currEvent->flags & EV_ERROR)
            {
                if (currEvent->flags == _socketFd)
                    throw std::runtime_error("event error");
                else
                {
                    std::cerr << "client socket error" << std::endl;
                    disconnectClient(currEvent->ident);
                }
            }
            else if (currEvent->filter == EVFILT_READ)
            {
                if (currEvent->ident == _socketFd)
                    connectClient(changeList);
                else if (clients.find(currEvent->ident) != clients.end() && !cmd)
                    cmd = parsingCommand(*currEvent);
            }
            else if (currEvent->filter == EVFILT_WRITE && cmd != NULL && currEvent->ident == cmd->_fd)
			{
				try
				{
                	cmd->execute();
				}
				catch(const std::exception& e)
				{
					std::string errMsg = e.what();
					errMsg += "\n";
					send(cmd->_fd, errMsg.c_str(), errMsg.length(), 0);
				}
				
				clients[cmd->_fd].sendBuffer.clear();
				delete cmd;
				cmd = NULL;
			}
        }
        
    }
}

void Server::connectClient( std::vector<struct kevent> &changeList)
{
    int client_socket;
    if ((client_socket = accept(_socketFd, NULL, NULL)) == -1) 
        throw std::runtime_error("client connect error");
    fcntl(client_socket, F_SETFL, O_NONBLOCK);

    changeEvents(changeList, client_socket, EVFILT_READ);
    changeEvents(changeList, client_socket, EVFILT_WRITE);
    clients[client_socket].sendBuffer.clear();
}

Command* Server::parsingCommand(struct kevent& currEvent)
{
	Command* cmd = NULL;
    char buf[1024];
    int n = recv(currEvent.ident, buf, sizeof(buf), 0);
	std::vector<std::string> tokenizedBuffer;

	if (n < 0)
        throw std::runtime_error("receive error");
    if (n == 0)
    {
        disconnectClient(currEvent.ident);
		return NULL;
    }
	buf[n] = '\0';
	clients[currEvent.ident].sendBuffer += buf;
	
	try
	{
		splitBuff(currEvent.ident, tokenizedBuffer);
		cmd = createCommand(currEvent.ident, tokenizedBuffer);
		std::cout << buf << std::endl;
	}
	catch(int e)
	{
		clients[currEvent.ident].sendBuffer.clear();
		return NULL;
	}
	return cmd;
}

void Server::disconnectClient(int client_fd)
{
    std::cout << client_fd << " : disconnect client" << std::endl;
    close(client_fd);
    clients.erase(client_fd);
}

void Server::splitBuff(uintptr_t fd, std::vector<std::string>& buff)
{
	size_t		pos = clients[fd].sendBuffer.find("\r\n");
	if (pos != std::string::npos)
		clients[fd].sendBuffer.erase(pos);
	buff = this->split(clients[fd].sendBuffer, ' ');
	if (buff.size() == 0)
		throw(1);
}

Command* Server::createCommand(uintptr_t fd, std::vector<std::string>& buff)
{
	Command*	cmd	= NULL;
	
	if (buff.begin()->compare("PASS") == 0)
		cmd = new Pass(clients, channels, fd, buff, _password);
	else if (buff.begin()->compare("NICK") == 0)
		cmd = new Nick(clients, channels, fd, buff);
	else if (buff.begin()->compare("USER") == 0)
		cmd = new User(clients, channels, fd, buff);
	else if (buff.begin()->compare("JOIN") == 0)
		cmd = new Join(clients, channels, fd, buff);
	//else if (buff.begin()->compare("INVITE") == 0)
	//	cmd = new Invite();
	//else if (buff.begin()->compare("QUIT") == 0)
	//	cmd = new Quit();
	//else if (buff.begin()->compare("PART") == 0)
	//	cmd = new Part();
	//else if (buff.begin()->compare("PRIVMSG") == 0)
	//	cmd = new Privmsg();
	//else if (buff.begin()->compare("KICK") == 0)
	//	cmd = new Kick();
	//else if (buff.begin()->compare("OPER") == 0)
	//	cmd = new Oper();
	//else if (buff.begin()->compare("MODE") == 0)
	//	cmd = new Mode();
	//else if (buff.begin()->compare("TOPIC") == 0)
	//	cmd = new Topic();
	else
		throw(1);
	return cmd;
}