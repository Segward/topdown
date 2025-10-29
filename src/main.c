#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef struct {
  float x, y;
  float width, height;
  unsigned int vao, vbo;
} sprite_t;

sprite_t *sprite_init(float x, float y, float width, float height) {
  sprite_t *sprite = malloc(sizeof(sprite_t));
  sprite->x = x;
  sprite->y = y;
  sprite->width = width;
  sprite->height = height;
  return sprite;
}

void sprite_bind_quad(sprite_t *sprite) {
  float vertices[] = {
    0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f
  };

  glGenVertexArrays(1, &sprite->vao);
  glGenBuffers(1, &sprite->vbo);

  glBindVertexArray(sprite->vao);
  glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const char *vertexShaderSource = " \
#version 330 core\n \
layout(location = 0) in vec2 aPos;\n \
uniform vec2 uPos;\n \
uniform vec2 uSize;\n \
uniform vec2 uCam;\n \
uniform float uZoom;\n \
out vec3 fragColor;\n \
void main() {\n \
    vec2 pos = (uPos + aPos * uSize - uCam) * uZoom;\n \
    gl_Position = vec4(pos, 0.0, 1.0);\n \
    fragColor = vec3(1.0, 1.0, 1.0);\n \
}\0";

const char *fragmentShaderSource = " \
#version 330 core\n \
in vec3 fragColor;\n \
out vec4 FragColor;\n \
void main() {\n \
    FragColor = vec4(fragColor, 1.0);\n \
}\0";

void sprite_draw(sprite_t *sprite, unsigned int shader, float camX, float camY, float zoom) {
  glUseProgram(shader);
  glUniform2f(glGetUniformLocation(shader, "uPos"), 
              sprite->x - sprite->width / 2.0f, 
              sprite->y - sprite->height / 2.0f);
  glUniform2f(glGetUniformLocation(shader, "uSize"), sprite->width, sprite->height);
  glUniform2f(glGetUniformLocation(shader, "uCam"), camX, camY);
  glUniform1f(glGetUniformLocation(shader, "uZoom"), zoom);
  glBindVertexArray(sprite->vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

int main() {
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "Topdown", NULL, NULL);
  if (!window)
    goto failed;

  glfwMakeContextCurrent(window);
  if (!gladLoadGL())
    goto failed;

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  unsigned int shader = glCreateProgram();
  glAttachShader(shader, vertexShader);
  glAttachShader(shader, fragmentShader);
  glLinkProgram(shader);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  sprite_t *sprite = sprite_init(0, 0, 0.25f, 0.25f);
  sprite_bind_quad(sprite);

  sprite_t *box = sprite_init(1, 0.2, 0.5f, 0.5f);
  sprite_bind_quad(box);

  float zoom = 1;

  while (!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_I) && zoom < 2.0f)
      zoom += 0.01f;

    if (glfwGetKey(window, GLFW_KEY_O) && zoom > 0.25f)
      zoom -= 0.01f;

    if (glfwGetKey(window, GLFW_KEY_W))
      sprite->y += 0.01f;

    if (glfwGetKey(window, GLFW_KEY_A))
      sprite->x -= 0.01f;

    if (glfwGetKey(window, GLFW_KEY_S))
      sprite->y -= 0.01f;

    if (glfwGetKey(window, GLFW_KEY_D))
      sprite->x += 0.01f;

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    sprite_draw(sprite, shader, sprite->x, sprite->y, zoom);
    sprite_draw(box, shader, sprite->x, sprite->y, zoom);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &sprite->vao);
  glDeleteBuffers(1, &sprite->vbo);
  glDeleteProgram(shader);

done:
  if (window)
    glfwDestroyWindow(window);

  glfwTerminate();
  return 0;

failed:
  glfwTerminate();
  return -1;
}
