//This file defines the main entry point into our borders fragment program.
//The borders fragment program sets the boundary conditions for a given border.
//We determine which border from the uniform variable offset.
uniform sampler2D interiorMap;
uniform vec2 offset; //determines which border we are on.
uniform float scale; //-1 one velocity, 1 for pressure.


void boundary(in vec2 coords, in vec2 offset,out vec4 bv,
	      in float scale, in sampler2D x);

void main()
{
  boundary(gl_TexCoord[0].st,offset,gl_FragColor,scale,interiorMap);
}
