#pragma once

#include "pch.h"

#define MAX_FIELDS 16
#define MAX_STRING_SIZE 256

// Field macro used to define a field in a packet struct
#define PACKET_FIELD(TYPE, NAME) \
   TYPE NAME;

// Define fields for the PING packet
// There is only one field and its the player id
#define PING_FIELDS \
  PACKET_FIELD(int, id)

// Define fields for the MOVE packet
// The id field is from the client
// The x and y fields are the fields of the clients player field
#define MOVE_FIELDS \
  PACKET_FIELD(int, id) \
  PACKET_FIELD(float, x) \
  PACKET_FIELD(float, y) \
 
// All packet types are defined here
#define PACKET_TYPE_ITER(_F) \
  _F(PING,   ping,   0, PING_FIELDS) \
  _F(MOVE, move, 1, MOVE_FIELDS)

// Define the packet types
#define PACKET_STRUCT(UC, LC, I, FIELDS) \
  typedef struct { FIELDS } LC##_t;

PACKET_TYPE_ITER(PACKET_STRUCT)

// Define packet enumeration
#define PACKET_ENUM_MEMBER(UC, LC, I, FIELDS) \
PACKET_TYPE_##UC = I,

typedef enum {
  PACKET_TYPE_ITER(PACKET_ENUM_MEMBER)
} packet_type_e;

// Define packet union
#define PACKET_UNION_MEMBER(UC, LC, I, FIELDS) \
  LC##_t LC;

typedef struct {
  packet_type_e type;
  union {
    PACKET_TYPE_ITER(PACKET_UNION_MEMBER)
  };
} packet_t;
