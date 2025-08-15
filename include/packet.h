#pragma once

#include "pch.h"

#define MAX_FIELDS 16
#define MAX_STRING_SIZE 256

// Field macro used to define a field in a packet struct
#define PACKET_FIELD(TYPE, NAME) \
   TYPE NAME;

// Define fields for the PING packet
#define PING_FIELDS \
  PACKET_FIELD(int, id)

// Define fields for the MOVE packet
#define MOVE_FIELDS \
  PACKET_FIELD(int, id) \
  PACKET_FIELD(float, x) \
  PACKET_FIELD(float, y) \

// Define fields for the ADD_PLAYER packet
#define ADD_PLAYER_FIELDS \
  PACKET_FIELD(int, id) \
  PACKET_FIELD(float, x) \
  PACKET_FIELD(float, y) \

// Define fields for the REMOVE_PLAYER packet
#define REMOVE_PLAYER_FIELDS \
  PACKET_FIELD(int, id) \
 
// All packet types are defined here
#define PACKET_TYPE_ITER(_F) \
  _F(PING,   ping,   0, PING_FIELDS) \
  _F(MOVE, move, 1, MOVE_FIELDS) \
  _F(ADD_PLAYER, add_player, 2) \
  _F(REMOVE_PLAYER, remove_player, 3) \

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
