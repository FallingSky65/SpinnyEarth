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

  vec3 color = vec3(uv.x, uv.y, 0.0);

  if (length(uv) <= 0.7) {
    color = vec3(0.0, 0.0, 1.0);
  }

  finalColor = vec4(color, 1.0);
}
