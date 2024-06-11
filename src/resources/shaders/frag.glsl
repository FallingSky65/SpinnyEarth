#version 330

// Input vertex attributes
in vec2 fragTexCoord;
in vec4 fragColor;

// Output fragment color 
out vec4 finalColor;

// Uniforms
uniform float uTime = 0.0;
uniform float aRatio = 1.0;

void main() {
  vec2 fragPos = fragTexCoord;
  vec2 uv = fragPos*vec2(2.0, 2.0) - vec2(1.0, 1.0);
  uv *= vec2(1.0, -1.0) * vec2(aRatio, 1.0);

  // raysphere
  vec3 spherePos = vec3(0.0, 0.0, -2.0);
  float radius = 1.0;
  vec3 rayDir = vec3(uv, -1.0);
  vec3 rayOrig = vec3(0.0);
  rayDir /= length(rayDir);

  float a = dot(rayDir, rayDir);
  float b = 2*dot(rayDir, rayOrig - spherePos);
  float c = dot(spherePos, spherePos) + dot(rayOrig, rayOrig) - 2*dot(spherePos,rayOrig) - radius * radius;

  float discriminant = b*b - 4*a*c;

  vec3 color = vec3(uv.x, uv.y, 0.0);
  if (discriminant >= 0.0) {
    float t = (-b - sqrt(discriminant))/(2*a);
    if (t >= 0.005) {
      vec3 intersectionPoint = rayOrig + t*rayDir;
      vec3 normal = (intersectionPoint - spherePos)/radius;

      float latitude = degrees(asin(normal.y));
      float longitude = degrees(atan(normal.x,normal.z)) + 15*uTime;

      if (mod(floor(latitude/15.0) + floor(longitude/15.0), 2) == 0.0) {
        color = vec3(1.0);
      } else {
        color = vec3(1.0, 0.0, 0.0);
      }


      //color = normal;
    }
  }

  finalColor = vec4(color, 1.0);
}
