#ifndef SPRITE_H
#define SPRITE_H

const char *vss = 
  "#version 330 core\n"
  "layout(location = 0) in vec2 aPos;\n"
  "uniform vec2 uPos;\n"
  "uniform vec2 uSize;\n"
  "uniform vec3 uColor;\n"
  "out vec3 fragColor;\n"
  "void main() {\n"
  "vec2 pos = uPos + aPos * uSize;\n"
  "gl_Position = vec4(pos, 0.0, 1.0);\n"
  "fragColor = uColor;\n"
  "}\0";

const char *fss = 
  "#version 330 core\n"
  "in vec3 fragColor;\n"
  "out vec4 FragColor;\n"
  "void main() {\n"
  "FragColor = vec4(fragColor, 1.0);\n"
  "}\0";

typedef struct {
  float x, y, w, h;
  unsigned int vao, vbo;
} sprite_t;

sprite_t *sprite_init(float x, float y, float w, float h);
void sprite_bind_quad(sprite_t *sprite);
void sprite_draw_quad(sprite_t *sprite, unsigned int shader);

#endif
