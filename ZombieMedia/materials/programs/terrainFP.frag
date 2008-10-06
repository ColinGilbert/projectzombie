#extension GL_EXT_gpu_shader4 : enable


uvec4 whitenoise(int uvec4 input, in unsigned int key);

void main()
{
	uvec4 coord;
	coord.x = gl_TexCoord[0].x;
	coord.y = gl_TexCoord[1].y;
	coord.z = 0u;
	coord.w = 0u;
	const unsigned int key = 1u;
	const unsigned int MAXFT = 4294967295u;
	uvec4 noise = whiteNoise(coord.st,key);
	vec4 color = float(noise)/float(MAXFT);
	gl_FragColor = color;
	//gl_FragColor = texture2D(terrainTexture,gl_TexCoord[0].st);
}