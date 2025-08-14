#pragma once

#include "pch.h"

#define MAX_FIELDS 16
#define MAX_STRING_SIZE 256

#define PACKET_TYPES_ITER(_F, ...) \
  _F(PING, ping, 1, ##__VA_ARGS__) \
  _F(CONNECT, connect, 2, ##__VA_ARGS__) \
  _F(PLAYER_MOVE, player_move, 3, ##__VA_ARGS__) \

#define DECL_TYPES_ENUM_MEMBER(UC, LC, I) \
  PACKET_TYPE_##UC = I,

#define DECL_TYPES_UNION_MEMBER(UC, LC, I) \
  LC##_t LC;

typedef struct { 
  char msg[MAX_STRING_SIZE]; 
  int playerId;

  #define PING_FIELDS(_F, PN) \
    _F(msg, PN) \
    _F(playerId, PN)

} ping_t;

typedef struct {
  char msg[MAX_STRING_SIZE]; 
  int playerId;

  #define CONNECT_FIELDS(_F, PN) \
    _F(msg, PN) \
    _F(playerId, PN)

} connect_t; 

typedef struct {
  char msg[MAX_STRING_SIZE];
  float x, y;
  int playerId;

  #define PLAYER_MOVE_FIELDS(_F, PN) \
    _F(msg, PN) \
    _F(x, PN) \
    _F(y, PN) \
    _F(playerId, PN)

} player_move_t;

typedef enum {
  PACKET_TYPES_ITER(DECL_TYPES_ENUM_MEMBER)
} packet_type_e;

typedef struct {
  union {
      PACKET_TYPES_ITER(DECL_TYPES_UNION_MEMBER)
  };
  packet_type_e type;
} packet_t;

typedef enum {
  FIELD_TYPE_INT,
  FIELD_TYPE_FLOAT,
  FIELD_TYPE_STRING,
} field_type_e;

typedef struct {
  field_type_e type;
  const char *name;
  int offset, size;
} field_desc_t;

typedef struct {
  field_desc_t fields[MAX_FIELDS];
} type_desc_t;

#define TYPE_TO_FIELD_TYPE(_T) _Generic(*((_T*) NULL), \
  int: FIELD_TYPE_INT, \
  float: FIELD_TYPE_FLOAT, \
  default: FIELD_TYPE_STRING \
)

#define TYPE_OF_FIELD(P, F) \
  __typeof__(((P*) NULL)->F)

#define IS_FIELD_TYPE_STRING(_T) \
  (sizeof(_T) > 1 &&  \
  __builtin_types_compatible_p(_T, char[MAX_STRING_SIZE]))

#define DO_FIELD_DESC(FN, PN) { \
  .type = IS_FIELD_TYPE_STRING( \
    TYPE_OF_FIELD(PN, FN)) ? FIELD_TYPE_STRING : \
    (TYPE_TO_FIELD_TYPE(TYPE_OF_FIELD(PN, FN))), \
  .name = #FN, \
  .size = sizeof(TYPE_OF_FIELD(PN, FN)), \
  .offset = offsetof(PN, FN), \
},

#define DO_TYPE_DESC(UC, LC, I, ...) \
  [I] = { .fields = { UC##_FIELDS(DO_FIELD_DESC, LC##_t) }},

const type_desc_t types[] = {
  PACKET_TYPES_ITER(DO_TYPE_DESC)
};

