#include "../shared/network/packet.h"
#include <stdio.h>
#include <string.h>

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

    for (int i = 0; i < ptr - buffer; i++) {
        printf("%02x ", buffer[i]);
    }

    printf("\n");
    return 0;
}
