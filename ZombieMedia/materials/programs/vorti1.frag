uniform sampler2D u;

void vorticity(in vec2 coords, //the coords
	       out float vort,
	       in sampler2D u);

void main()
{
  //float vort;
  vorticity(gl_TexCoord[0].st,vort,u);
  gl_FragColor = vec4(vort,0.0,0.0,0.0);
}
