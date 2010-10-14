uniform sampler2D terrainTexture;
varying vec3 DiffuseColor;
varying float LightIntensity;
void main()
{
  vec4 lightColor = texture2D(terrainTexture, gl_TexCoord[0].xy);
  lightColor.xyz *= DiffuseColor;
  gl_FragColor = lightColor * LightIntensity;
  //gl_FragColor = lightColor * LightIntensity + 0.0;
}
