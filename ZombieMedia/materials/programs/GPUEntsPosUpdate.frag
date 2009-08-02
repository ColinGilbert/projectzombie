#extension GL_EXT_gpu_shader4 : enable

uniform sampler2D _input;
uniform sampler2D dir;
uniform float dt; //change in time
uniform int key;
uniform vec3 camPos;
const float EPSILON = 2.4414e-4;
const float STOPRANGE = 6.0; //stop

uvec4 whiteNoise(in uvec4 _input, in unsigned int key);

vec4 convertToR0_R1(in uvec4 _input);

void main()
{

	
	//update position using old euler's method
	vec2 tc = gl_TexCoord[0].st;
	//tc.t = 1.0 - tc.t;
	vec4 pos = texture2D(_input,tc);
	vec4 dir = texture2D(dir,tc);
	
	uvec4 coord;
	unsigned int uintkey = unsigned int(key);
	coord.x = uint(gl_TexCoord[0].x*20000.0);
	coord.y = uint(gl_TexCoord[0].y*20000.0);
	coord.z = uintkey;
	coord.w = uintkey;
	
	uvec4 noise = whiteNoise(coord,uintkey);
	vec4 rand = convertToR0_R1(noise);
	//rand.x = rand.x*0.03; //speed
	rand.x = rand.x*0.5;
	vec3 camPosG = camPos;
	camPosG.y = 0.0;
	vec3 vecToPlayer = camPosG - pos.xyz;
	float dis = length(vecToPlayer);
	if(dis > STOPRANGE)
	{
		dis = STOPRANGE/dis;
		dis = max(dis,EPSILON); //for now, we need to limit the zombie range later.
	}	
	else
		dis = 1.0;
	pos.xz = pos.xz + dt*rand.x*dir.xy;	
	//pos.xz = pos.xz + dt*rand.x*(1.0 - dis)*dir.xy; //old euler's method
	//pos.xz = pos.xz + dt*rand.x*vecToPlayer.xz;
	gl_FragColor = pos;
}
