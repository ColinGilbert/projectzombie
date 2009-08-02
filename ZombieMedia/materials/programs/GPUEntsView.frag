uniform sampler2D imposterTex;
uniform sampler2D positionTex;
const float TWOPI = 6.283185;
const float PIHALF = 1.5707963;
const float PI = 3.1415926;
const vec3 specColor = vec3(1.0,0.1,0.1);
void main()
{
    //gl_TexCoord[1].t = 1.0 - gl_TexCoord[1].t;
	const int IMPOSTER = 0;
	//the normal here is based on sphereical coordinates.
	float theta = gl_TexCoord[IMPOSTER].p;
	float phi = gl_TexCoord[IMPOSTER].q*PI;
	
	//vec3 pos = texture2D(positionTex,gl_TexCoord[1].st).xyz;
	theta = theta*TWOPI - PI;
	//phi = (1.0-phi)*PI - PIHALF;
	vec3 normal;
	normal.x = sin(theta)*sin(phi);
	normal.y = cos(phi);
	normal.z = cos(theta)*sin(phi);
	normal = normalize(normal);
	//color = texture2D(imposterTex,gl_TexCoord[IMPOSTER].st);
	vec3 lightDir = gl_TexCoord[2].xyz;
	gl_FragColor = texture2D(imposterTex,gl_TexCoord[IMPOSTER].st);
	/*
	float ndl = dot(normal,lightDir);
	gl_FragColor.xyz *= ndl;
	//calculate specular
	vec3 h = lightDir + gl_TexCoord[3].xyz-2.0*normal-gl_TexCoord[4].xyz;
	h = normalize(h);
	if(ndl > 0)
		ndl = 1.0;
	else
		ndl = 0.0;
 	
	float spec = 0.8*pow(max(dot(normal,h),0.0),42.0)*ndl;
	vec3 specFac = specColor*spec;
	gl_FragColor.w = 1.0;
	*/
	//gl_FragColor.xyz += specFac;
	//gl_FragColor.xyz = color.xyz;
}