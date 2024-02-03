#include "Server.hpp"
#include "Pass.hpp"
#include "Nick.hpp"
#include "User.hpp"
#include "Join.hpp"
#include "Kick.hpp"
#include "Invite.hpp"
#include "Topic.hpp"
#include "Oper.hpp"
#include "Mode.hpp"
#include "Ping.hpp"
#include "WhoIs.hpp"
#include "Privmsg.hpp"

Server::Server(char* port, char* password) : _serverName("localhost"), _password(password), _rootId("qwer"), _rootPw("1234")
{
    if (!isValidPort(port))
        throw std::invalid_argument("Wrong port!");
    create();
}

Server::~Server() {}

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
	std::vector<Command*> cmds;
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
                else if (clients.find(currEvent->ident) != clients.end() && cmds.empty())
                    cmds = parsingCommand(*currEvent);
            }
            else if (currEvent->filter == EVFILT_WRITE && !cmds.empty() && currEvent->ident == cmds.front()->_fd)
			{
				try
				{
					// std::cout << "fd = " << cmds.front()->_fd << std::endl;
					for (size_t i = 0; i < cmds.size(); i++)
					{
						if (cmds[i])
							cmds[i]->execute();
					}
				}
				catch(const std::exception& e)
				{
					std::cerr << e.what() <<std::endl;
				}
				clients[cmds[0]->_fd].sendBuffer.clear();
				for (size_t i = 0; i < cmds.size(); i++)
				{
					if (cmds[i])
						delete cmds[i];
				}
				cmds.clear();
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

std::vector<Command*> Server::parsingCommand(struct kevent& currEvent)
{
	std::vector<Command*> cmds;
    char buf[1024];
    int n = recv(currEvent.ident, buf, sizeof(buf), 0);
	std::vector<std::string> tokenizedBuffer;

	if (n < 0)
        throw std::runtime_error("receive error");
    if (n == 0)
    {
        disconnectClient(currEvent.ident);
		cmds.clear();
		return cmds;
    }
	buf[n] = '\0';
	clients[currEvent.ident].sendBuffer += buf;
	try
	{
		//커맨드 단위 벡터 만들기
		//단일 커맨드를 공백기준으로 잘라서
		splitBuff(currEvent.ident, tokenizedBuffer);
		for (size_t i = 0; i < tokenizedBuffer.size(); i++)
		{
			std::vector<std::string> token = splitSpace(tokenizedBuffer[i]);
			if (token.size() != 0)
				cmds.push_back(createCommand(currEvent.ident, token));
		}
	}
	catch(int e)
	{
		clients[currEvent.ident].sendBuffer.clear();
		cmds.clear();
		return cmds;
	}
	return cmds;
}

void Server::disconnectClient(int client_fd)
{
    std::cout << client_fd << " : disconnect client" << std::endl;
    close(client_fd);
    clients.erase(client_fd);
}

void Server::splitBuff(uintptr_t fd, std::vector<std::string>& buff)
{
	size_t pos = clients[fd].sendBuffer.find("\r\n");
	while (pos != std::string::npos)
	{
		std::string cmdTemp;
		cmdTemp = clients[fd].sendBuffer.substr(0, pos);
		buff.push_back(cmdTemp);
    	clients[fd].sendBuffer.erase(0, pos + 2);
		pos = clients[fd].sendBuffer.find("\r\n");
		// if (buff.size() == 0)
		// 	throw(1);
	}
	if (clients[fd].sendBuffer.size() != 0)
	{
		// std::cout << "파싱 꼬레" <<  clients[fd].sendBuffer << std::endl;
		buff.push_back(clients[fd].sendBuffer);
	}
}

std::vector<std::string> Server::splitSpace(std::string& st)
{
	size_t pos = st.find(" ");
	std::vector<std::string> vec;
	std::cout << st << std::endl;
	while (pos != std::string::npos)
	{
		std::string cmdTemp;
		cmdTemp = st.substr(0, pos);
		if (cmdTemp.length() != 0)
			vec.push_back(cmdTemp);
    	st.erase(0, pos + 1);
		pos = st.find(" ");
	}
	if (st.size() != 0)
		vec.push_back(st);
	// for (size_t i = 0; i < vec.size(); i++)
	// {
	// 	std::cout << vec[i] << std::endl;
	// }
	return vec;
}

Command* Server::createCommand(uintptr_t fd, std::vector<std::string>& buff)
{
	Command*	cmd	= NULL;

// 여기서 buff에 명령어 리스트들이 들어있어서 반복문 돌면서 해야하는데 어떻게 해야할까

	// 그냥 split(buff.begin())->compare("PASS") 요렇게!!!
	if (buff.begin()->compare("PASS") == 0)
		cmd = new Pass(clients, channels, fd, buff, _password);
	else if (buff.begin()->compare("NICK") == 0)
		cmd = new Nick(clients, channels, fd, buff);
	else if (buff.begin()->compare("USER") == 0)
		cmd = new User(clients, channels, fd, buff);
	else if (buff.begin()->compare("PING") == 0)
		cmd = new Ping(clients, channels, fd, buff);
	else if (buff.begin()->compare("WHOIS") == 0)
		cmd = new WhoIs(clients, channels, fd, buff);
	else if (buff.begin()->compare("JOIN") == 0)
		cmd = new Join(clients, channels, fd, buff);
	else if (buff.begin()->compare("KICK") == 0)
		cmd = new Kick(clients, channels, fd, buff);
	else if (buff.begin()->compare("INVITE") == 0)
		cmd = new Invite(clients, channels, fd, buff);
	else if (buff.begin()->compare("TOPIC") == 0)
		cmd = new Topic(clients, channels, fd, buff);
	else if (buff.begin()->compare("PRIVMSG") == 0)
		cmd = new Privmsg(clients, channels, fd, buff);
	else if (buff.begin()->compare("OPER") == 0)
		cmd = new Oper(clients, channels, fd, buff, _rootId, _rootPw);
	else if (buff.begin()->compare("MODE") == 0)
		cmd = new Mode(clients, channels, fd, buff);
	//else if (buff.begin()->compare("LIST") == 0)
	//	cmd = new List();
	//else if (buff.begin()->compare("QUIT") == 0)
	//	cmd = new Quit();
	//else if (buff.begin()->compare("PART") == 0)
	//	cmd = new Part();
	else
		throw(1);
	return cmd;
}

//NICK phan
//USER phan phan 0 :pilho
//WHOIS phan
//PING irc.libera.chat
