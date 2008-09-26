//This is the fragment program that computes the gradient of a scalar field, then subtract the gradient field
//from a velocity field. u = w - grad p. Where u is divergence free.
uniform float halfrdx;
uniform sampler2D p; //the pressure field
uniform sampler2D w; //the saclar field
uniform int pIndex; //the index into the texture which indicates which component stores the pressure.

void gradsub(in vec2 coords, //the texture coords
	     out vec4 uNew, //the new velocity
	     in float halfrdx, //0.5 / gridscale
	     in sampler2D p, //pressure scalar field
	     in sampler2D w, //velocity
	     in int pIndex); //index which indicates where pressure scalar field is stored in the component of p texture.

void main()
{
  gradsub(gl_TexCoord[0].st,gl_FragColor,halfrdx,p,w,pIndex);
}
