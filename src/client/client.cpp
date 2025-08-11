#include "../shared/network.hpp"
#include "../shared/packet.hpp"
#include <string>
#include <vector>
#include <iostream>

int main(int argc, char *argv[])
{
  network::client client("127.0.0.1", 8080);

  while (true)
  {
    std::string message;
    std::cout << "Enter message to send (or 'exit' to quit): ";
    std::getline(std::cin, message);
    if (message == "exit")
      break;

    std::vector<uint8_t> data(message.begin(), message.end());

    packet::packet pkt;
    pkt.header.type = packet::type::MESSAGE;
    pkt.header.size = data.size();
    pkt.data = std::move(data);
    client.send(pkt);
  }

  return 0;
}
