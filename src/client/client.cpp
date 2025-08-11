#include "../shared/network.hpp"

int main(int argc, char *argv[])
{
    network::client client("127.0.0.1", 8080);

    return 0;
}