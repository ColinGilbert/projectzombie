//This files defines the main entry point for our advection frag program.

uniform sampler2D velocityMap; //our velocity texture
uniform sampler2D quantityMap; //quanity that we want to advect.
uniform float dt; //our time
uniform float rdx; //reciprocal of dx, 
                   //reciprocal of distance between cells in the direction of x.
uniform float drate; //diffusion rate.

void advect(in vec2 coords, out vec4 qtynew,
	    in float dt, in float rdx,
	    in sampler2D vel, in sampler2D qty);

void main()
{
  //float tdt = 0.0001;
  advect(gl_TexCoord[0].st,gl_FragColor,
	 dt,rdx,velocityMap,quantityMap); //advect, see myflowsim.frag
  //gl_FragColor = vec4(vec3(texture2D(velocityMap,gl_TexCoord[0].st)),1.0);
  //gl_FragColor *= 1.0;
  gl_FragColor *= drate;
  gl_FragColor.a = 1.0;
}
