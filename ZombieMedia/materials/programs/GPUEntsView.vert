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
uniform vec3 viewUp;
uniform vec3 viewSide;
uniform vec3 camPos;
void main()
{
	const int STATE = 1;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	vec2 scaleUV;
	scaleUV.s = scaleS;
	scaleUV.t = scaleT;
	gl_TexCoord[0].st *= scaleUV;
	gl_TexCoord[1] = gl_MultiTexCoord1;
	
	//vec4 vertPos = gl_Vertex;
	gl_Position = gl_Vertex;
	vec4 entPos = texture2D(gpuEntsStates,gl_TexCoord[STATE].st);
	//rotate the billboard to face the screen. We get this info from 
	//vertPos.xyz += entPos.xyz; //displace
	//rotate into the billboard's coordinate frame, i.e. transform by billboard's orthonormal basis.
	vec3 dir = normalize(camPos-entPos.xyz); //billboard direction
	vec3 side = cross(viewUp,dir);
	vec3 up = cross(dir,side);
	
	gl_Position.x = dot(side,gl_Vertex.xyz); //we are manually rotating into the billboard's coorindate frame.
	gl_Position.y = dot(up,gl_Vertex.xyz);
	gl_Position.z = dot(-dir,gl_Vertex.xyz);
	gl_Position.xyz += entPos.xyz; //displace
		//note, we assume transformation from object into world space transform from the engine is null, since we just did the
	//object to world transform above, manually in the shader. 
	gl_Position = gl_ModelViewProjectionMatrix*gl_Position; //note the way our billboards are setup the
														    //transformation from object space into world space is null. 
}