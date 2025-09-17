#ifndef PACKET_H
#define PACKET_H

#include "pch.h"

#define MAX_FIELDS 16
#define MAX_STRING_SIZE 256

#define PACKET_FIELD(TYPE, NAME) \
   TYPE NAME;

#define PING_FIELDS \
  PACKET_FIELD(int, id)

#define JOIN_FIELDS \
  PACKET_FIELD(int, id)

#define PACKET_TYPE_ITER(_F) \
  _F(PING, ping, 0, PING_FIELDS) \
  _F(JOIN, join, 1, JOIN_FIELDS) \

#define PACKET_STRUCT(UC, LC, I, FIELDS) \
  typedef struct { FIELDS } LC##_t;

PACKET_TYPE_ITER(PACKET_STRUCT)

#define PACKET_ENUM_MEMBER(UC, LC, I, FIELDS) \
PACKET_TYPE_##UC = I,

typedef enum {
  PACKET_TYPE_ITER(PACKET_ENUM_MEMBER)
} packet_type_e;

#define PACKET_UNION_MEMBER(UC, LC, I, FIELDS) \
  LC##_t LC;

#define PACKET_DIR_CC 0
#define PACKET_DIR_CS 1
#define PACKET_DIR_SS 2
#define PACKET_DIR_SC 3
#define PACKET_DIR_SB 4

typedef struct {
  packet_type_e type;
  int dir;
  union {
    PACKET_TYPE_ITER(PACKET_UNION_MEMBER)
  };
} packet_t;

#endif
