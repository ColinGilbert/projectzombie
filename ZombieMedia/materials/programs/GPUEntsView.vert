//This program defines the vertex program for GPUEntsView
//This proram will accomplish the following:
//Transform the vertex from object space into world space as an imposter.
//Displace the vertex based on the current GPUEnts' state texture (i.e. move to current location of an entity).
//Transform into projection space.

#extension GL_EXT_gpu_shader4 : enable

uniform sampler2D gpuEntsStates; //the gpu entities' state texture
//uniform float imposterWidth;
//uniform float imposterHeight;
//uniform float texDim;
uniform float scaleS;
uniform float scaleT;
uniform vec3 viewUp;
uniform vec3 viewSide;
uniform vec3 camPos;
uniform vec3 viewDirObjS;
const float EPSILON = 2.4414e-4;

void computeNewS(inout vec3 s)
{
	if(abs(s.x)<abs(s.y) && abs(s.x)<abs(s.z))
		s = vec3(0.0,-s.z,s.y);
	else if(abs(s.y)<abs(s.x) && abs(s.y)<abs(s.z))
		s= vec3(-s.z,0.0,s.x);
	else if(abs(s.z)<abs(s.x) && abs(s.z)<abs(s.y))
		s = vec3(-s.y,s.x,0);
}

void main()
{
	const int STATE = 1;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;
	//figure out which imposter texture to access based on viewDirObjectSpace
	
	vec3 forward = vec3(0.0,0.0,1.0); //forward in object space
	vec3 side = vec3(1.0,0.0,0.0);
	
	vec3 xzview = viewDirObjS;
	xzview.y = 0; //we only care about this vector on the xz plane.
	
	vec2 scaleUV;
	scaleUV.s = scaleS;
	scaleUV.t = scaleT;
	gl_TexCoord[0].st *= scaleUV;
	
	
	gl_Position = gl_Vertex;
	vec4 entPos = texture2D(gpuEntsStates,gl_TexCoord[STATE].st);
	
	//create the billboard matrix
	mat3 billMat; 
	billMat[2] = normalize(camPos-entPos.xyz); //Z basis
	billMat[0] = cross(viewUp,billMat[2]); //X basis
	billMat[1] = viewUp; //Y basis

	gl_Position.xyz = billMat*gl_Vertex.xyz; //transform by billboard's orthonormal basis
	
	gl_Position.xyz += entPos.xyz; //displace
	//note, we assume transformation from object into world space transform from the engine is null, since we just did the
	//object to world transform above, manually in the shader. 
	gl_Position = gl_ModelViewProjectionMatrix*gl_Position;  
}