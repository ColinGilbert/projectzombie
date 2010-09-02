#extension GL_EXT_gpu_shader4 : enable


attribute vec4 vertex;
attribute vec3 normal;
attribute vec4 uv0;
attribute vec4 uv1;
attribute vec4 uv2;
attribute vec3 tangent;



uniform mat4 viewProjMatrix;
uniform sampler2D matrixTexture;


void main()
{
  gl_TexCoord[0] = uv0;

  mat4 worldMatrix;
  worldMatrix[0] = texture2D(matrixTexture, uv1.xy);
  worldMatrix[1] = texture2D(matrixTexture, uv1.zw);
  worldMatrix[2] = texture2D(matrixTexture, uv2.xy);
  worldMatrix[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
  vec4 worldPos = vertex * worldMatrix;
  gl_Position = viewProjMatrix * worldPos;
}
