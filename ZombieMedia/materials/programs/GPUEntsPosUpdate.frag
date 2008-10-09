
uniform sampler2D input;
uniform sampler2D dir;
uniform float dt; //change in time
void main()
{
	//update position using old euler's method
	vec4 pos = texture2D(input,gl_TexCoord[0].st);
	vec4 dir = texture2D(dir,gl_TexCoord[0].st);
	
	pos.xz = pos.xz + dt*dir.xy; //old euler's method
	
	gl_FragColor = pos;
}