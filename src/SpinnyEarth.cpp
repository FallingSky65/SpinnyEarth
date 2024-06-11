#include "raylib.h"

int main(void) {
  
  const int SW = 800;
  const int SH = 800;
  const float ASPECT_RATIO = (float)SW/SH;

  const int RENDERFPS = 30;
  const int DURATION = 10;
  int frameCount = 0;


  SetConfigFlags(FLAG_MSAA_4X_HINT);
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

  Image earthImg = LoadImage("src/resources/EarthTex.png");
  Texture2D earthTex = LoadTextureFromImage(earthImg);
  UnloadImage(earthImg);
  SetShaderValueTexture(shader, GetShaderLocation(shader, "texture1"), earthTex);

  SetTargetFPS(600);

  while (!WindowShouldClose() && frameCount < DURATION*RENDERFPS) {
    frameCount++;
    time = (float)frameCount/RENDERFPS;
    SetShaderValue(shader, timeLoc, &time, SHADER_UNIFORM_FLOAT);

    BeginDrawing();
      ClearBackground(RAYWHITE);
      
      BeginShaderMode(shader);
        SetShaderValueTexture(shader,
            GetShaderLocation(shader, "texture1"),
            earthTex);
        DrawTexture(texture, 0, 0, WHITE);
      EndShaderMode();
      
      //DrawFPS(10, 10);

    EndDrawing();

    TakeScreenshot(TextFormat("Frame%03i.png", frameCount));
  }
  UnloadTexture(earthTex);
  UnloadShader(shader);

  CloseWindow();

  return 0;
}
