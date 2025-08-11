#include "../shared/network.hpp"
#include "../shared/packet.hpp"
#include <string>
#include <vector>

int main(int argc, char *argv[])
{
  network::client client("127.0.0.1", 8080);
  client.connect();

  std::string message = "Hello Server";
  std::vector<uint8_t> data(message.begin(), message.end());

  packet::packet pkt;
  pkt.header.type = packet::type::MESSAGE;
  pkt.header.size = data.size();
  pkt.data = std::move(data);
  client.send(pkt);

  return 0;
}
