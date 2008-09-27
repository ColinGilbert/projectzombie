//This program defines the vertex program for GPUEntsView
//This proram will accomplish the following:
//Transform the vertex from object space into world space as an imposter.
//Displace the vertex based on the current GPUEnts' state texture (i.e. move to current location of an entity).
//Transform into projection space.

uniform sampler2D gpuEntsStates; //the gpu entities' state texture
//uniform float imposterWidth;
//uniform float imposterHeight;
//uniform float texDim;
uniform float scaleS;
uniform float scaleT;
void main()
{
	const int STATE = 1;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	vec2 scaleUV;
	scaleUV.s = scaleS;
	scaleUV.t = scaleT;
	gl_TexCoord[0].st *= scaleUV;
	gl_TexCoord[1] = gl_MultiTexCoord1;
	
	vec4 vertPos = gl_Vertex;
	vec4 entPos = texture2D(gpuEntsStates,gl_TexCoord[STATE].st);
	vertPos.xyz += entPos.xyz; //displace
	gl_Position = gl_ModelViewProjectionMatrix*vertPos;
	//vertPos.z = 0.0;
	//vertPos.x = 0.0;
	//gl_Position = gl_ModelViewProjectionMatrix*vertPos;
	//gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
}