#include "../shared/network.hpp"
#include <vector>
#include <iostream>

int main(int argc, char *argv[])
{
  network::client client("127.0.0.1", 8080);
  std::string message = "Hello, Server!";
  std::vector<uint8_t> data(message.begin(), message.end());
  packet::packet pkt;
  pkt.header.type = packet::type::MESSAGE;
  pkt.header.size = static_cast<uint32_t>(data.size());
  pkt.data = data;
  client.send(pkt);
  pkt = client.receive();
  std::string received_message(pkt.data.begin(), pkt.data.end());
  std::cout << "Received message: " << received_message << std::endl;
  return 0;
}
