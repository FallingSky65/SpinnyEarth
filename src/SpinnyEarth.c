#include <math.h>

#include "raylib.h"
#include "raymath.h"

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

float radians(float degrees) {
  return degrees*M_PI/180.0f;
}


int main(void) {

  const int SW = 800;
  const int SH = 800;

  const int RENDERFPS = 30;
  const int DURATION = 10;
  int frameCount = 0;

  const bool SAVEFRAMES = true;

  InitWindow(SW, SH, "xoot");

  Camera camera = { 0 };
  camera.position = (Vector3){ 0.0f, 0.0f, 4.0f };
  camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
  camera.up = (Vector3){ 0.0f, -1.0f, 0.0f };
  camera.fovy = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  Model sphereModel = LoadModelFromMesh(GenMeshSphere(1.0f, 32, 32));
  sphereModel.transform = MatrixMultiply(sphereModel.transform, MatrixRotateX(radians(90.0f)));
  Matrix sphereTransformDefault = sphereModel.transform;

  Image img = LoadImage("src/resources/EarthTex.png");
  //Image img = LoadImage("src/resources/texel_checker.png"); // Use to verify texture orientation is correct
  ImageFlipHorizontal(&img);
  ImageRotateCW(&img);
  RenderTexture2D sphereRT = LoadRenderTexture(img.width, img.height);

  Texture2D imgTexture = LoadTextureFromImage(img);
  BeginTextureMode(sphereRT);
    ClearBackground(RAYWHITE);
    DrawTexture(imgTexture, 0, 0, WHITE);
  EndTextureMode();
  UnloadTexture(imgTexture);
  UnloadImage(img);

  sphereModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = sphereRT.texture;
  
  Shader shader = LoadShader("src/resources/shaders/vert.glsl", "src/resources/shaders/frag.glsl");
  shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(shader, "matModel");
  shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

  int ambientLoc = GetShaderLocation(shader, "ambient");
  SetShaderValue(shader, ambientLoc, (float[4]){ 0.2f, 0.2f, 0.2f, 1.0f }, SHADER_UNIFORM_VEC4);
  
  sphereModel.materials[0].shader = shader;

  CreateLight(LIGHT_DIRECTIONAL, (Vector3){ 1.0f, 1.0f, 1.0f }, Vector3Zero(), WHITE, shader);


  float time = 0.0f;

  RenderTexture2D canvas = LoadRenderTexture(SW,SH);

  // Comment out this remove the FPS cap
  SetTargetFPS(RENDERFPS);

  while (!WindowShouldClose() && frameCount <= DURATION*RENDERFPS) {
    frameCount++;
    time = (float)frameCount/RENDERFPS;

    // Uncomment to make camera orbit
    //UpdateCamera(&camera, CAMERA_ORBITAL);
    
    sphereModel.transform = MatrixMultiply(sphereTransformDefault, MatrixRotateY(time*radians(36.0f)));

    SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], &camera.position.x, SHADER_UNIFORM_VEC3);

    BeginTextureMode(canvas);

      ClearBackground(RAYWHITE);

      BeginMode3D(camera);

        DrawModel(sphereModel, Vector3Zero(), 1.0f, WHITE);    

      EndMode3D();

    EndTextureMode();

    BeginDrawing();

      ClearBackground(RAYWHITE);

      DrawTexture(canvas.texture, 0, 0, WHITE);

      DrawFPS(10, 10);
      
      SetWindowTitle(TextFormat("xoot | %i/%i Frames", frameCount-1, DURATION*RENDERFPS));

    EndDrawing();

    if (SAVEFRAMES && frameCount > 1) {
      //TakeScreenshot(TextFormat("Frame%03i.png", frameCount-1)); // Slower implementation of image saving, more reliable, use for debugging image saving
      Image screenshot = LoadImageFromTexture(canvas.texture);
      ExportImage(screenshot, TextFormat("Frame%03i.png", frameCount-1));
      UnloadImage(screenshot);
    }
  }

  UnloadModel(sphereModel);
  UnloadShader(shader);
  UnloadRenderTexture(canvas);
  UnloadRenderTexture(sphereRT);

  CloseWindow();

  return 0;
}
