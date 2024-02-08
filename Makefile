#-------------------------------------------

BLACK       =   "\033[0;30m"
GRAY        =   "\033[1;30m"
RED         =   "\033[1;31m"
GREEN       =   "\033[0;32m"
YELLOW      =   "\033[1;33m"
PURPLE      =   "\033[0;35m"
CYAN        =   "\033[1;36m"
WHITE       =   "\033[1;37m"
EOC         =   "\033[0;0m"
LINE_CLEAR  =   "\x1b[1A\x1b[M"

#-------------------------------------------
NAME = ircserv
CXX	= c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
HEADER = ./includes
HEADER_CMDS = ./includes/commands/

SRCS_NAME =	main.cpp \
		Server.cpp \
		UserAccount.cpp \
		Database.cpp \
		Channel.cpp 
SRCS_PREFIX = ./srcs/
SRCS = $(addprefix $(SRCS_PREFIX), $(SRCS_NAME))

CMDS_NAME =	Pass.cpp \
			Command.cpp \
			Nick.cpp \
			User.cpp \
			Ping.cpp \
			WhoIs.cpp \
			Join.cpp \
			Kick.cpp \
			Invite.cpp \
			Topic.cpp \
			Privmsg.cpp \
			Mode.cpp \
			Oper.cpp \
			Part.cpp \
			Quit.cpp 
CMDS_PREFIX = ./srcs/commands/
SRCS += $(addprefix $(CMDS_PREFIX), $(CMDS_NAME))

OBJS = $(SRCS:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	@$(CXX) $(CXXFLAGS) -o $(NAME) $^
	@echo $(GREEN)"\n===================================================\n"$(EOC)
	@echo $(YELLOW)"                    IRC IS READY"$(EOC)
	@echo $(GREEN)"\n===================================================\n"$(EOC)

%.o : %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(HEADER) -I$(HEADER_CMDS)

clean :
	rm -f $(OBJS)

fclean : clean
	rm -f $(NAME)

re :
	$(MAKE) fclean
	$(MAKE) all

.PHONY : all bonus clean fclean re
