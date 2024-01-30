#include "Server.hpp"

int main(int ac, char *av[])
{
	try
	{
		if (ac != 3)
			throw std::invalid_argument("Invalid argument!");
    	Server server(av[1], av[2]);
    	server.start();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
    return 0;
}
