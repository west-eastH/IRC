#pragma once
#include "Server.hpp"

class Command
{
	protected:
		std::vector<std::string>	_parsedCommand;
		std::map<int, UserInfo>&	_clients;
		std::vector<Channel>&		_channels;
		UserInfo&					_curUser;
	public:
		uintptr_t					_fd;

		Command(std::map<int, UserInfo>& clients, std::vector<Channel>& channels, uintptr_t fd, std::vector<std::string> parsedCommand);
		virtual ~Command();
		
		void putString(const int fd, const std::string& str) const;
		void throwError(const int fd, const std::string& str) const;
		int findNick(const std::string& nick) const;
		int	findChannel(const std::string& name) const;
		// void sendToClient(int clientFd, std::string warning);
		void sendToClient(int clientFd, std::string prefix, std::string cmd, std::string params);

		virtual void execute() = 0;
		class CommandError : public std::exception
		{
			private:
				const std::string _msg;
			public:
				CommandError(std::string errCode, std::string errCmd, std::string msg);
				~CommandError() throw();
				const char* what() const throw();
		};
};
