#include "../shared/network.hpp"

int main(int argc, char *argv[])
{
    network::server srv(8080, 8081);
    srv.start();
    return 0;
}