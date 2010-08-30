uniform vec4 lightAmbient;
uniform vec4 lightPosition;
uniform vec4 lightDiffuse;
uniform vec4 lightSpecular;
uniform vec4 lightAttenuation;
uniform float surfaceShininess;
uniform vec4 cameraPos;

void main()
{
  //gl_TexCoord[].st;
  gl_FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}
