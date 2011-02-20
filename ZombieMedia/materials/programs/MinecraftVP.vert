//This program defines a test shader for doing "Minecraft" voxel shading.

#define SKY 1
#define FULLBAND 1

//uniform sampler2D terrainTexture;
uniform mat4 worldViewProjMatrix;

attribute vec4 vertex;
attribute vec4 uv0;


varying vec4 position;
varying vec4 worldPos;
varying vec4 textureAtlasOffset;
const float ATLAS_WIDTH = 4096.0;
const float TEX_WIDTH = 256.0;
const float eOffset = TEX_WIDTH / ATLAS_WIDTH;

void main(void)
{
  position = worldViewProjMatrix * vertex;
  worldPos = abs(position + vec4(0.5f, 0.5f, 0.5f, 0.5f));

  gl_Position = position;

  float idx = uv0.x * 256.0 - 1;
  float blocky = floor(idx / 16.0);
  float blockx = idx - blocky * 16.0;
  textureAtlasOffset = vec4(blockx + 0.25f, blocky + 0.25f, 0, 0) * eOffset; //translate to align center.
}
