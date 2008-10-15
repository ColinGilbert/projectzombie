//This program defines the vertex program for GPUEntsView
//This proram will accomplish the following:
//Transform the vertex from object space into world space as an imposter.
//Displace the vertex based on the current GPUEnts' state texture (i.e. move to current location of an entity).
//Transform into projection space.

#extension GL_EXT_gpu_shader4 : enable

uniform sampler2D gpuEntsStates; //the gpu entities' state texture
uniform sampler2D DirTex;
//uniform float imposterWidth;
//uniform float imposterHeight;
//uniform float texDim;
uniform float scaleS;
uniform float scaleT;
uniform vec3 viewUp;
//uniform vec3 viewSide;
uniform vec3 camPos;
uniform int segPhi;
uniform int segTheta;

uniform vec3 vPos;

const float EPSILON = 2.4414e-4;
const float TWOPI = 6.283185;
const float PIHALF = 1.5707963;
const float PI = 3.1415926;

//uniform vec4 lightPos;

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

	//gl_TexCoord[2] = gl_MultiTexCoord2;
	//gl_TexCoord[1].s = 1.0 - gl_TexCoord[1].s;
	//gl_TexCoord[1].t = 1.0 - gl_TexCoord[1].t;
	
	//Transformation of billbord/////////////////////////////
	gl_Position = gl_Vertex;
	vec4 entPos = texture2D(gpuEntsStates,gl_TexCoord[STATE].st);
	
	//create the billboard matrix
	vec3 height = vec3(0.0,1.5,0.0);
	mat3 billMat;
	
	billMat[2] = normalize(camPos-entPos.xyz); //Z basis
	billMat[0] = cross(viewUp,billMat[2]); //X basis
	billMat[1] = viewUp; //Y basis

	gl_Position.xyz = billMat*gl_Vertex.xyz; //transform by billboard's orthonormal basis
	
	gl_Position.xyz += entPos.xyz; //displace
	//note, we assume transformation from object into world space transform from the engine is null, since we just did the
	//object to world transform above, manually in the shader. 
	gl_Position = gl_ModelViewProjectionMatrix*gl_Position;  
	
	//Compute Imposter Texture coordinates by deriving PHI and THETA indices into Imposter Texture.
	//We use sphereical coordinates to derive phi and theta. With phi = acos(y), theta=atan2(x,z)
	//vec3 vpos = vec3(0.0,0.0,100.0);
	vec3 viewDir = entPos.xyz - camPos;
	viewDir = normalize(viewDir);
	
	//transform into object space
	//We read the xy directional vector from dir texture and figure out theta.
	vec4 objDir = texture2D(DirTex,gl_TexCoord[STATE].st);
	
	float objTheta = atan(objDir.x,objDir.y); //note x,y here implies x, and z.
	
	mat3 wdToObjMat; 
	//construct inverse rotation matrix to rotate from world space into object space.
	wdToObjMat[0].x = cos(objTheta); wdToObjMat[1].x = 0.0; wdToObjMat[2].x = -sin(objTheta);
	wdToObjMat[0].y = 0.0; wdToObjMat[1].y = 1.0; wdToObjMat[2].y = 0.0;
	wdToObjMat[0].z = sin(objTheta); wdToObjMat[1].z = 0.0; wdToObjMat[2].z = cos(objTheta);
	
	//transform camera
	viewDir = wdToObjMat*viewDir; 
	
	float phi = acos(viewDir.y);
	phi -= PIHALF;
	
	float theta;
	theta = atan(viewDir.x,viewDir.z);
	theta += PI;
	phi = mod(round(phi/PIHALF*float(segPhi)),float(segPhi));
	theta = mod(round((theta/TWOPI)*float(segTheta)),float(segTheta));
	
	vec2 scaleUV;
	scaleUV.s = scaleS;
	scaleUV.t = scaleT;
	gl_TexCoord[0].p = gl_TexCoord[0].s;
	gl_TexCoord[0].q = gl_TexCoord[0].t;
	gl_TexCoord[0].st *= scaleUV;
	gl_TexCoord[0].s += scaleS*theta;
	gl_TexCoord[0].t += scaleT*phi;
	
	//const vec4 lightPos = camPos;
	
	//light up 
	//vec3 lightPos = vec3(0.0,-10.0,0.0);
	vec3 lightPos = camPos;
	lightPos += vec3(0.0,10.0,0.0);
	vec3 lightDir = normalize(lightPos - entPos.xyz-height);
	 
	//matrix to transform from world space into billboard space.
	mat3 invBillM;
	
	invBillM[0].x = billMat[0].x; invBillM[1].x = billMat[0].y; invBillM[2].x = billMat[0].z;
	invBillM[0].y = billMat[1].x; invBillM[1].y = billMat[1].y; invBillM[2].y = billMat[1].z;
	invBillM[0].z = billMat[2].x; invBillM[1].z = billMat[2].y; invBillM[2].z = billMat[2].z;
	
	gl_TexCoord[2].xyz = invBillM*lightDir;
	gl_TexCoord[3].xyz = invBillM*camPos;
	gl_TexCoord[4].xyz = invBillM*entPos.xyz;
}