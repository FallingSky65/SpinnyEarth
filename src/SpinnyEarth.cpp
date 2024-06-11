#include "raylib.h"

int main(void) {
  
  const int SW = 800;
  const int SH = 450;
  const float ASPECT_RATIO = (float)SW/SH;

  InitWindow(SW, SH, "xoot");

  Image imBlank = GenImageColor(SW, SH, BLANK);
  Texture2D texture = LoadTextureFromImage(imBlank);
  UnloadImage(imBlank);

  Shader shader = LoadShader(0,"src/resources/shaders/frag.glsl");

  float time = 0.0f;
  int timeLoc = GetShaderLocation(shader, "uTime");
  SetShaderValue(shader, timeLoc, &time, SHADER_UNIFORM_FLOAT);

  SetShaderValue(shader,
      GetShaderLocation(shader, "aRatio"),
      &ASPECT_RATIO, SHADER_UNIFORM_FLOAT);

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    time = (float)GetTime();
    SetShaderValue(shader, timeLoc, &time, SHADER_UNIFORM_FLOAT);

    BeginDrawing();
      ClearBackground(RAYWHITE);
      
      BeginShaderMode(shader);
        DrawTexture(texture, 0, 0, WHITE);
      EndShaderMode();

    EndDrawing();
  }

  UnloadShader(shader);

  CloseWindow();

  return 0;
}