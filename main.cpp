#include "Server.hpp"

int main(int ac, char *av[])
{
	if (ac != 3)
		return 1;
    Server server(av);
    server.start();
    return 0;
}