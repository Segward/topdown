#pragma once

#include <stddef.h>

#define PACKET_TYPES_ITER_SIMPLE(_F) \
  _F(PING, ping, 1) \
  _F(PONG, pong, 2)

#define PACKET_TYPES_ITER_FIELDS(_F, ...) \
  _F(PING, ping, 1, __VA_ARGS__) \
  _F(PONG, pong, 2, __VA_ARGS__)

#define MAX_FIELDS 16
#define MAX_STRING_SIZE 256

typedef struct ping { int x; } ping_t;
typedef struct pong { char y[MAX_STRING_SIZE]; } pong_t;

#define PING_FIELDS(_F, PN) _F(x, PN)
#define PONG_FIELDS(_F, PN) _F(y, PN)
#define DECL_TYPES_ENUM_MEMBER(UC, LC, I) PACKET_TYPE_##UC = I,

typedef enum {
  PACKET_TYPES_ITER_SIMPLE(DECL_TYPES_ENUM_MEMBER)
} packet_type_e;

#define DECL_TYPES_UNION_MEMBER(UC, LC, I) LC##_t LC;

typedef struct {
  union {
      PACKET_TYPES_ITER_SIMPLE(DECL_TYPES_UNION_MEMBER)
  };
  packet_type_e type;
} packet_t;

typedef enum {
  FIELD_TYPE_INT,
  FIELD_TYPE_FLOAT,
  FIELD_TYPE_STRING,
} field_type_e;

#define TYPE_TO_FIELD_TYPE(_T) _Generic(*((_T*) NULL), \
  int: FIELD_TYPE_INT, \
  float: FIELD_TYPE_FLOAT, \
  default: FIELD_TYPE_STRING \
)

typedef struct {
  field_type_e type;
  const char *name;
  int offset, size;
} field_desc_t;

typedef struct {
  field_desc_t fields[MAX_FIELDS];
} type_desc_t;

#define TYPE_OF_FIELD(P, F) __typeof__(((P*) NULL)->F)

#define IS_FIELD_TYPE_STRING(_T) \
  (sizeof(_T) > 1 && __builtin_types_compatible_p(_T, char[MAX_STRING_SIZE]))

#define DO_FIELD_DESC(FN, PN) { \
  .type = IS_FIELD_TYPE_STRING(TYPE_OF_FIELD(PN, FN)) ? FIELD_TYPE_STRING : \
    (TYPE_TO_FIELD_TYPE(TYPE_OF_FIELD(PN, FN))), \
  .name = #FN, \
  .size = sizeof(TYPE_OF_FIELD(PN, FN)), \
  .offset = offsetof(PN, FN), \
},

#define DO_TYPE_DESC(UC, LC, I, ...) \
  [I] = { \
      .fields = { \
          UC##_FIELDS(DO_FIELD_DESC, LC##_t) \
      } \
  },

const type_desc_t types[] = {
  PACKET_TYPES_ITER_FIELDS(DO_TYPE_DESC)
};

