#include "../shared/network.hpp"

int main(int argc, char *argv[])
{
    network::server srv(8080);
    srv.start();
    return 0;
}