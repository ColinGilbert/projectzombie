uniform sampler2D terrainTexture;
varying vec3 DiffuseColor;
varying float LightIntensity;
void main()
{
  vec4 lightColor = texture2D(terrainTexture, gl_TexCoord[0].st);
  lightColor *= vec4(DiffuseColor, 1.0);
  gl_FragColor = lightColor * LightIntensity;
  //gl_FragColor = lightColor * LightIntensity + 0.0;
}
