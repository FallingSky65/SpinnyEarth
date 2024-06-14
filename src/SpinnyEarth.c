#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

// Convert degrees to radians
float radians(float degrees) {
  return degrees*M_PI/180.0f;
}

int main(int argc, char **argv) {

  if (argc != 5) {
    printf("Incorrect # of arguments. Requires 4 (Screen width, Screen height, RenderFPS, Duration(s))\n");
    return 1;
  } 

  // Screen dimensions
  const int SW = atoi(argv[1]); // Screen width
  const int SH = atoi(argv[2]); // Screen height

  const int RENDERFPS = atoi(argv[3]); // FPS of the final render
  const int DURATION = atoi(argv[4]);  // Duration of the final render in seconds
  int frameCount = 0;       // Ticker

  // Set false to disable saving the frames. Boosts FPS dramatically, but makes the program kind of useless
  const bool SAVEFRAMES = true;

  // Disable raylib info logging, bye bye to terminal spam 
  SetTraceLogLevel(LOG_ERROR);

  // Initialize window
  InitWindow(SW, SH, "SpinnyEarth by fs65");

  // Initialize camera and its parameters
  Camera camera = { 0 };
  camera.position = (Vector3){ 0.0f, 0.0f, 4.0f };
  camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
  camera.up = (Vector3){ 0.0f, -1.0f, 0.0f };
  camera.fovy = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  // Create and rotate the sphere model
  Model sphereModel = LoadModelFromMesh(GenMeshSphere(1.0f, 128, 128));
  sphereModel.transform = MatrixMultiply(sphereModel.transform, MatrixRotateX(radians(90.0f)));
  Matrix sphereTransformDefault = sphereModel.transform;

  // Load and reorient the texture as image
  Image img = LoadImage("src/resources/EarthTexClouds.png");
  //Image img = LoadImage("src/resources/texel_checker.png"); // Use to verify texture orientation is correct
  ImageFlipHorizontal(&img);
  ImageRotateCW(&img);

  // Create RenderTexture2D, and draws the texture image onto the RenderTexture2D
  // Using a RenderTexture2D to store the texture allows for updating the texture throughout the animation
  RenderTexture2D sphereRT = LoadRenderTexture(img.width, img.height);
  Texture2D imgTexture = LoadTextureFromImage(img);
  BeginTextureMode(sphereRT);
    ClearBackground(RAYWHITE);
    DrawTexture(imgTexture, 0, 0, WHITE);
  EndTextureMode();
  UnloadTexture(imgTexture);
  UnloadImage(img);

  // Sets model texture
  sphereModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = sphereRT.texture;
  
  // Loads shaders
  Shader shader = LoadShader("src/resources/shaders/vert.glsl", "src/resources/shaders/frag.glsl");
  shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(shader, "matModel");
  shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

  // Sets the ambient lighting for the shader
  int ambientLoc = GetShaderLocation(shader, "ambient");
  SetShaderValue(shader, ambientLoc, (float[4]){ 0.2f, 0.2f, 0.2f, 1.0f }, SHADER_UNIFORM_VEC4);
  
  // Sets model shader
  sphereModel.materials[0].shader = shader;

  // Creates a directional light in the scene
  CreateLight(LIGHT_DIRECTIONAL, (Vector3){ 1.0f, 1.0f, 1.0f }, Vector3Zero(), WHITE, shader);

  // Trackes the "time" based on when the frame is in the final animation
  // measured in seconds
  float time = 0.0f;

  // Used for optimized saving of the frames
  // Each frame is drawn onto the canvas, then the canvas is drawn on the screen
  RenderTexture2D canvas = LoadRenderTexture(SW,SH);

  // Comment out this remove the FPS cap
  SetTargetFPS(RENDERFPS);

  // Runs for each frame of the animation
  while (!WindowShouldClose() && frameCount <= DURATION*RENDERFPS) {

    // Update frameCount and time
    frameCount++;
    time = (float)frameCount/RENDERFPS;

    // Uncomment to make camera orbit
    //UpdateCamera(&camera, CAMERA_ORBITAL);
    
    // Rotate the model, s.t. the model has one rotation per the entire animation
    sphereModel.transform = MatrixMultiply(sphereTransformDefault, MatrixRotateY(time*radians(360.0f/DURATION)));

    // Updates camera position in the shader
    SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], &camera.position.x, SHADER_UNIFORM_VEC3);

    // Drawing to canvas
    BeginTextureMode(canvas);

      ClearBackground(RAYWHITE);

      BeginMode3D(camera);

        DrawModel(sphereModel, Vector3Zero(), 1.0f, WHITE);    

      EndMode3D();

    EndTextureMode();

    // Drawing to screen
    BeginDrawing();

      ClearBackground(RAYWHITE);

      DrawTexture(canvas.texture, 0, 0, WHITE);

      DrawFPS(10, 10);
      
      SetWindowTitle(TextFormat("xoot | %i/%i Frames", frameCount-1, DURATION*RENDERFPS));

    EndDrawing();

    // Saves the frame as an image
    if (SAVEFRAMES && frameCount > 1) {
      //TakeScreenshot(TextFormat("Frame%03i.png", frameCount-1)); // Slower implementation of image saving, more reliable, use for debugging image saving
      Image screenshot = LoadImageFromTexture(canvas.texture);
      ExportImage(screenshot, TextFormat("frames/Frame%03i.png", frameCount-1));
      UnloadImage(screenshot);
    }
  }

  // Frees memory
  UnloadModel(sphereModel);
  UnloadShader(shader);
  UnloadRenderTexture(canvas);
  UnloadRenderTexture(sphereRT);

  CloseWindow();

  return 0;
}
