#include "../shared/network/packet.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  packet_t packet;
  packet.type = PACKET_TYPE_PING;
  packet.ping.x = 42;

  unsigned char buffer[256];
  unsigned char *ptr = buffer;
  const type_desc_t *desc = &types[packet.type];

  for (int i = 0; i < MAX_FIELDS; i++) {
    field_desc_t field = desc->fields[i];
    if (field.size == 0) break;

    memcpy(ptr, (unsigned char *)&packet + field.offset, field.size);
    ptr += field.size;
  }

  ping_t *ping = (ping_t *)buffer;
  printf("X: %d\n", ping->x);

  packet_t packet2;
  packet2.type = PACKET_TYPE_PONG;
  packet2.pong.y = malloc(strlen("Hello, World!") + 1);
  strcpy(packet2.pong.y, "Hello, World!");


  bzero(buffer, sizeof(buffer));
  ptr = buffer;
  desc = &types[packet2.type];

  for (int i = 0; i < MAX_FIELDS; i++) {
    field_desc_t field = desc->fields[i];
    if (field.size == 0) break;

    memcpy(ptr, (unsigned char *)&packet2 + field.offset, field.size);
    ptr += field.size;
  }

  pong_t *pong = (pong_t *)buffer;  
  printf("Y: %s\n", pong->y);

  return 0;
}
