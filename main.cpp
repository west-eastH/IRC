#include "Server.hpp"

int main(int ac, char *av[])
{
    Server server(ac, av);
    server.start();
    return 0;
}