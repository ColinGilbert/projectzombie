//This program defines a fragment program that sets sources.
uniform vec4 sources;

void main()
{
  gl_FragColor = sources;
  //gl_FragColor = vec4(-0.01,0.0,0.0,0.0);
}
