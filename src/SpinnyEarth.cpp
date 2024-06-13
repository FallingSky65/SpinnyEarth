#include "raylib.h"
#include <cstdlib>
#include <iostream>

int main(void) {
  
  const int SW = 800;
  const int SH = 800;
  const float ASPECT_RATIO = (float)SW/SH;

  const int RENDERFPS = 30;
  const int DURATION = 10;
  int frameCount = 0;

  //SetConfigFlags(!FLAG_WINDOW_HIGHDPI);
  InitWindow(SW, SH, "xoot");

  //std::cout << GetWindowScaleDPI().x << std::endl;

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

  RenderTexture2D canvas = LoadRenderTexture(SW,SH);

  SetTargetFPS(600);

  while (!WindowShouldClose() && frameCount <= DURATION*RENDERFPS) {
    frameCount++;
    time = (float)frameCount/RENDERFPS;
    SetShaderValue(shader, timeLoc, &time, SHADER_UNIFORM_FLOAT);

    //BeginDrawing();
    BeginTextureMode(canvas);

      ClearBackground(RAYWHITE);
      
      BeginShaderMode(shader);
        SetShaderValueTexture(shader,
            GetShaderLocation(shader, "texture1"),
            earthTex);
        //DrawRectangle(0, 0, SW, SH, WHITE);
        DrawTexture(texture, 0, 0, WHITE);
      EndShaderMode();

    EndTextureMode();

    BeginDrawing();

      ClearBackground(RAYWHITE);

      DrawTexture(canvas.texture, 0, 0, WHITE);
      
      SetWindowTitle(TextFormat("xoot | FPS: %i | %i/%i Frames", GetFPS(), frameCount-1, DURATION*RENDERFPS));

    EndDrawing();

    if (frameCount > 1) {
      //TakeScreenshot(TextFormat("Frame%03i.png", frameCount-1));
      Image screenshot = LoadImageFromTexture(canvas.texture);
      ExportImage(screenshot, TextFormat("Frame%03i.png", frameCount-1));
      UnloadImage(screenshot);
    }
  }
  UnloadTexture(earthTex);
  UnloadShader(shader);
  UnloadRenderTexture(canvas);

  CloseWindow();

  return 0;
}
