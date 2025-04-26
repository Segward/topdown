#include <raylib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  int screenWidth = 800;  
  int screenHeight = 600;
  
  InitWindow(screenWidth, screenHeight, "Fantasy RPG");
  SetTargetFPS(60);
 
  Vector2 playerPosition = {screenWidth / 2, screenHeight / 2};
  Vector2 playerVelocity = {0, 0}; 

  while (!WindowShouldClose()) {
    if (IsKeyDown(KEY_W)) playerVelocity.y = -5;
    else if (IsKeyDown(KEY_S)) playerVelocity.y = 5;
    else playerVelocity.y = 0;

    if (IsKeyDown(KEY_A)) playerVelocity.x = -5;
    else if (IsKeyDown(KEY_D)) playerVelocity.x = 5;
    else playerVelocity.x = 0;

    playerPosition.x += playerVelocity.x;
    playerPosition.y += playerVelocity.y;

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawCircleV(playerPosition, 20, BLUE);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}

