uniform sampler2D resultMap;

void main()
{
  //gl_FragColor = 0.5 + 0.5*texture2D(resultMap,gl_TexCoord[0].st)/128.0;
  //gl_FragColor = 0.5 + 0.5*clamp(texture2D(resultMap,gl_TexCoord[0].st)/256.0,-1.0,1.0);
  gl_FragColor = texture2D(resultMap,gl_TexCoord[0].st);
}
