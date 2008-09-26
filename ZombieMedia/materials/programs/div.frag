//This program defines a fragment that computes the divergence of a vector field w.
uniform sampler2D vectorField;
uniform float halfrdx;

void divergence(in vec2 coords,
		out vec4 div,
		in float halfrdx,
		in sampler2D w);

void main()
{
  divergence(gl_TexCoord[0].st,gl_FragColor,halfrdx,vectorField);
}
