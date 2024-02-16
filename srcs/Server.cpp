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
#include "Privmsg.hpp"
#include "Part.hpp"
#include "Quit.hpp"

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
	_DB = Database::getInstance();
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
    std::cout << "server started" << std::endl;
    while (true)
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
				try
				{
					if (currEvent->ident == _socketFd)
						connectClient(changeList);
					else if (_DB->isUserExists(currEvent->ident) && cmds.empty())
						cmds = parsingCommand(*currEvent);
				}
				catch(const std::exception& e)
				{
					std::cerr << e.what() << '\n';
				}
            }
            else if (currEvent->filter == EVFILT_WRITE && !cmds.empty() && currEvent->ident == cmds.front()->getFd())
			{
				try
				{
					for (size_t i = 0; i < cmds.size(); i++)
						if (cmds[i])
							cmds[i]->execute();
				}
				catch(const std::exception& e)
				{
					std::cerr << e.what() << std::endl;
				}
				for (size_t i = 0; i < cmds.size(); i++)
					if (cmds[i])
						delete cmds[i];
				cmds.clear();
				if (_DB->isUserExists(currEvent->ident) == false)
					continue ;
				_DB->getAccount(currEvent->ident).sendBuffer.clear();
			}
        }  
    }
}

void Server::connectClient( std::vector<struct kevent> &changeList)
{
    uintptr_t clientSocket;
    if ((clientSocket = accept(_socketFd, NULL, NULL)) == -1) 
        throw std::runtime_error("client connect error");
    fcntl(clientSocket, F_SETFL, O_NONBLOCK);

    changeEvents(changeList, clientSocket, EVFILT_READ);
    changeEvents(changeList, clientSocket, EVFILT_WRITE);
	_DB->addAccount(clientSocket);
}

std::vector<Command*> Server::parsingCommand(struct kevent& currEvent)
{
	int n;
    char buf[512];
	std::vector<Command*> cmds;
	std::vector<std::string> tokenizedBuffer;
	std::string& sendBuffer = _DB->getAccount(currEvent.ident).sendBuffer;

	while (true)
	{
		n = recv(currEvent.ident, buf, sizeof(buf) - 1, 0);
		if (n < 0)
			throw std::runtime_error("receive error");
		if (n == 0)
		{
			disconnectClient(currEvent.ident);
			cmds.clear();
			return cmds;
		}
		buf[n] = '\0';
		sendBuffer += buf;
		if (std::strlen(buf) < 511)
			break ;
	}
	if (sendBuffer.find("\r\n") != std::string::npos)
	{
		try
		{
			tokenizedBuffer = splitBuff(sendBuffer);
			for (size_t i = 0; i < tokenizedBuffer.size(); i++)
			{
				std::vector<std::string> token = splitSpace(tokenizedBuffer[i]);
				if (token.size() != 0)
					cmds.push_back(createCommand(currEvent.ident, token));
			}
		}
		catch(int e)
		{
			sendBuffer.clear();
			cmds.clear();
			return cmds;
		}
	}
	return cmds;
}

void Server::disconnectClient(int clientFd)
{
    std::cout << clientFd << " : disconnect client" << std::endl;
    close(clientFd);
	_DB->deleteAccount(clientFd);
}

std::vector<std::string> Server::splitBuff(std::string& sendBuffer)
{
	std::vector<std::string> buff;
	size_t pos = sendBuffer.find("\r\n");

	while (pos != std::string::npos)
	{
		std::string cmdTemp;
		cmdTemp = sendBuffer.substr(0, pos);
		buff.push_back(cmdTemp);
    	sendBuffer.erase(0, pos + 2);
		pos = sendBuffer.find("\r\n");
	}
	if (sendBuffer.size() != 0)
		buff.push_back(sendBuffer);
	return buff;
}

std::vector<std::string> Server::splitSpace(std::string& st)
{
	size_t pos = st.find(" ");
	std::vector<std::string> vec;
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
	return vec;
}

Command* Server::createCommand(uintptr_t fd, std::vector<std::string>& buff)
{
	Command*	cmd	= NULL;

	if (buff.begin()->compare("PASS") == 0)
		cmd = new Pass(fd, buff, _password);
	else if (buff.begin()->compare("NICK") == 0)
		cmd = new Nick(fd, buff);
	else if (buff.begin()->compare("USER") == 0)
		cmd = new User(fd, buff);
	else if (buff.begin()->compare("PING") == 0)
		cmd = new Ping(fd, buff);
	else if (buff.begin()->compare("JOIN") == 0)
		cmd = new Join(fd, buff);
	else if (buff.begin()->compare("KICK") == 0)
		cmd = new Kick(fd, buff);
	else if (buff.begin()->compare("INVITE") == 0)
		cmd = new Invite(fd, buff);
	else if (buff.begin()->compare("TOPIC") == 0)
		cmd = new Topic(fd, buff);
	else if (buff.begin()->compare("PRIVMSG") == 0)
		cmd = new Privmsg(fd, buff);
	else if (buff.begin()->compare("OPER") == 0)
		cmd = new Oper(fd, buff, _rootId, _rootPw);
	else if (buff.begin()->compare("MODE") == 0)
		cmd = new Mode(fd, buff);
	else if (buff.begin()->compare("PART") == 0)
		cmd = new Part(fd, buff);
	else if (buff.begin()->compare("QUIT") == 0)
		cmd = new Quit(fd, buff);
	else
		throw(1);
	return cmd;
}
