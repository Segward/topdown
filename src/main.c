#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

typedef struct {
  float x, y;
  float width, height;
  unsigned int vao, vbo;
} sprite_t;

unsigned int texture_load(const char *path) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);

    unsigned char *data = stbi_load(path, &width, &height, &channels, 4);
    if (!data) {
        printf("Failed to load texture: %s\n", path);
        return 0;
    }

    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    return tex;
}

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
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f
};

  glGenVertexArrays(1, &sprite->vao);
  glGenBuffers(1, &sprite->vbo);

  glBindVertexArray(sprite->vao);
  glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const char *vertexShaderSource = " \
#version 330 core\n \
layout(location = 0) in vec2 aPos;\n \
layout(location = 1) in vec2 aUV;\n \
uniform vec2 uPos;\n \
uniform vec2 uSize;\n \
uniform vec2 uCam;\n \
uniform float uZoom;\n \
out vec2 vUV;\n \
void main() {\n \
  vec2 pos = (uPos + aPos * uSize - uCam) * uZoom;\n \
  gl_Position = vec4(pos, 0.0, 1.0);\n \
  vUV = aUV;\n \
}\0";

const char *fragmentShaderSource = " \
#version 330 core\n \
in vec2 vUV;\n \
uniform sampler2D uTex;\n \
out vec4 FragColor;\n \
void main() {\n \
  FragColor = texture(uTex, vUV);\n \
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

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glfwSwapInterval(0);

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

  unsigned int tex = texture_load("assets/texture.png");
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glUniform1i(glGetUniformLocation(shader, "uTex"), 0);

  sprite_t *sprite = sprite_init(0, 0, 0.25f, 0.25f);
  sprite_bind_quad(sprite);

  sprite_t *box = sprite_init(1, 0.2, 0.5f, 0.5f);
  sprite_bind_quad(box);

  float zoom = 1;
  float lastFrame = 0.0f;
  float speed = 1.0f;

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    float moveX = 0.0f;
    float moveY = 0.0f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      moveY += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      moveY -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      moveX += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      moveX -= 1.0f;

    float length = sqrtf(moveX * moveX + moveY * moveY);
    if (length > 0.0f) {
      moveX /= length;
      moveY /= length;
    }

    sprite->x += moveX * speed * deltaTime;
    sprite->y += moveY * speed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_I) && zoom < 2.0f)
      zoom += speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_O) && zoom > 0.25f)
      zoom -= speed * deltaTime;

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
