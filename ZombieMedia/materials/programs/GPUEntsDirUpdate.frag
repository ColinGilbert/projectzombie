#extension GL_EXT_gpu_shader4 : enable

uniform int key;
uniform sampler2D input; //input texture
uniform float dt; //time
uvec4 whiteNoise(in uvec4 input, in unsigned int key);

vec4 convertToR0_R1(in uvec4 input);

//This program updates the director texture of GPUEntities.
//we randomly pick a direction.
//Precondition:
//We assume that input.xy contains the directional vector that we want to update. 
//Where x is the e1 axis and y e2 axis.
void main()
{
	vec4 state = texture2D(input,gl_TexCoord[0].st);
	
	if(state.z > state.w)
	{
		state.z = 0.0-dt;
		//next throw dice to determine whether we want to change
		uvec4 coord;
		unsigned int uintkey = unsigned int(key);
		coord.x = gl_TexCoord[0].x*20000.0;
		coord.y = gl_TexCoord[0].y*20000.0;
		coord.z = uintkey;
		coord.w = uintkey;
		
		uvec4 noise = whiteNoise(coord,uintkey);
		vec4 rand = convertToR0_R1(noise);
		
		state.w = rand.w*15.00+1.0; //change sometime in 1 to 5 sec
		
		if(rand.x < rand.y) //do our probability thing
		{
			const float twopi = 6.283185;
			rand.z = rand.z*twopi; //theta
			state.x = cos(rand.z);
			state.y = sin(rand.z);
		}
	}
	state.z += dt;
	gl_FragColor = state;
}