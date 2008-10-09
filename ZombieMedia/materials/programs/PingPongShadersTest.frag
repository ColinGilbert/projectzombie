#extension GL_EXT_gpu_shader4 : enable

uniform int key;
uniform sampler2D input; //the input texture
uvec4 whiteNoise(in uvec4 input, in unsigned int key);

vec4 convertToR0_R1(in uvec4 input);

void main()
{
	//now check R channel to see if it's smaller than 0.99
	vec4 color = texture2D(input,gl_TexCoord[0].st);
	
	if(color.r < 0.99 || color.g < 0.99 || color.b < 0.99) //if less than 0.99, then we do the random thing.
	{
		uvec4 coord;
		unsigned int uintkey = unsigned int(key);
		coord.x = gl_TexCoord[0].x*20000.0;
		coord.y = gl_TexCoord[0].y*20000.0;
		coord.z = uintkey;
		coord.w = uintkey;
		
		uvec4 noise = whiteNoise(coord,uintkey);
		vec4 rand = convertToR0_R1(noise);
		color.w = 1.0;
		
		float prob = 0.1;
		float offset = 0.005;
		
		//probability that we increase color
		if(rand.x < prob)
		{
			color.r += offset;			
		}
		if(rand.y < prob)
		{
			color.g += offset;
		}
		if(rand.z < prob)
		{
			color.b += offset;
		}
		
	}
	gl_FragColor = color;
}