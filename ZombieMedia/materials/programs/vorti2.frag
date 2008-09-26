uniform sampler2D vort;
uniform sampler2D u;
uniform float dt;
void vortForce(in vec2 coords,
	       out vec2 uNew,
	       in float dt,
	       in sampler2D vort,
	       in sampler2D u);

void main()
{
  vec2 uNew;
  vortForce(gl_TexCoord[0].st,uNew,dt,vort,u);
  gl_FragColor = vec4(uNew,0.0,0.0);
}
