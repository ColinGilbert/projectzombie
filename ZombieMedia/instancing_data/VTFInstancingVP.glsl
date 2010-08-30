#extension GL_EXT_gpu_shader4 : enable

uniform mat4 viewProjMatrix;
uniform sampler2D matrixTexture;


void main()
{
  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_TexCoord[1] = gl_MultiTexCoord1;
  gl_TexCoord[2] = gl_MultiTexCoord2;

  mat4 worldMatrix;
  worldMatrix[0] = texture2D(matrixTexture, gl_TexCoord[1].xy);
  worldMatrix[1] = texture2D(matrixTexture, gl_TexCoord[1].zw);
  worldMatrix[2] = texture2D(matrixTexture, gl_TexCoord[2].xy);
  worldMatrix[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
  vec4 worldPos;
  worldPos.xyz  =  (worldMatrix * gl_Vertex).xyz;
  worldPos.w = 1.0f;
  gl_Position = viewProjMatrix * worldPos;
}
