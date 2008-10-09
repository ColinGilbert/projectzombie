#extension GL_EXT_gpu_shader4 : enable

uniform int key;
uniform sampler2D input; //input texture
uvec4 whiteNoise(in uvec4 input, in unsigned int key);

vec4 convertToR0_R1(in uvec4 input);

//This program updates the director texture of GPUEntities.
//we randomly pick a direction.
//Precondition:
//We assume that input.xy contains the directional vector that we want to update. 
//Where x is the e1 axis and y e2 axis.
void main()
{
	vec4 dir = texture2D(input,gl_TexCoord[0].st);
	//next throw dice to determine whether we want to change
	uvec4 coord;
	unsigned int uintkey = unsigned int(key);
	coord.x = gl_TexCoord[0].x*20000.0;
	coord.y = gl_TexCoord[0].y*20000.0;
	coord.z = uintkey;
	coord.w = uintkey;
	
	uvec4 noise = whiteNoise(coord,uintkey);
	vec4 rand = convertToR0_R1(noise);
	
	if(rand.x < rand.y) //do our probability thing
	{
		const float twopi = 6.283185;
		rand.z = rand.z*twopi; //theta
		dir.x = cos(rand.z);
		dir.y = sin(rand.z);
	}
	
	gl_FragColor = dir;
}