#extension GL_EXT_gpu_shader4 : enable

uniform int key;

uvec4 whiteNoise(in uvec4 input, in unsigned int key);

vec4 convertToR0_R1(in uvec4 input);


void main()
{
	uvec4 coord;
	
	coord.x = gl_TexCoord[0].x*1024.0;
	coord.y = gl_TexCoord[0].y*1024.0;
	coord.z = 0u;
	coord.w = 0u;
	uvec4 noise = whiteNoise(coord,unsigned int(key));
	vec4 color = convertToR0_R1(noise);
	color.w = 1.0;
	
	//vec4 color;
	//color.x = float(noise.x)/float(MAXFT);
	//color.y = float(noise.y)/float(MAXFT);
	//color.z = float(noise.z)/float(MAXFT);
	//color.w = 1.0;
	gl_FragColor = color;
	//vec4 fac = vec4(0.2125,0.7154,0.0721,0.0);
	//conver to gray scale;
	//float y = dot(fac,color);
	//gl_FragColor.r = y;
	//gl_FragColor.g = y;
	//gl_FragColor.b = y;
	//gl_FragColor.a = 1.0;
}