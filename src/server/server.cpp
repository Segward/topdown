#include "../shared/network.hpp"

int main(int argc, char *argv[])
{
    network::server srv(8080);

    while (true)
    {
        srv.accept();
        
    }

    return 0;
}