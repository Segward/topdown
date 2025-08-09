#include "network/network.hpp"

int main()
{
    network net;
    net.start("127.0.0.1", 8080);
    return 0;
}