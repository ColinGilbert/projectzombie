//This program defines a test shader for doing "Minecraft" voxel shading.

//uniform sampler2D terrainTexture;
uniform vec4 lightPosition;
uniform vec3 eyePosition;
uniform mat4 worldViewProjMatrix;
uniform mat4 texWorldViewProjMatrix0;
uniform mat4 texWorldViewProjMatrix1;
uniform mat4 texWorldViewProjMatrix2;
uniform vec4 depthRange0;
uniform vec4 depthRange1;
uniform vec4 depthRange2;


//attribute vec4 LightPosition;
attribute vec4 vertex;
attribute vec3 normal;
attribute vec4 colour;
attribute vec4 uv0;
attribute vec4 uv1;
attribute vec4 uv2;
//attribute vec3 tangent;

varying vec3  DiffuseColor;
varying vec4 oUv0;
varying vec3 oLightDir;
varying vec3 oHalfAngle;
varying vec4 oLightPosition0;
varying vec4 oLightPosition1;
varying vec4 oLightPosition2;
varying vec3 oNormal;


//const float SpecularContrib = 0.2;
//const float DiffuseContrib = 1.0 - SpecularContrib;

const float ATLAS_WIDTH = 512.0;
const float TEX_WIDTH = 32.0;
const float BlockOffset = TEX_WIDTH / ATLAS_WIDTH;

const float eOffset = (TEX_WIDTH) / ATLAS_WIDTH;// - 12*pixOffset ;
varying float LightIntensity;
//varying vec2 MCposition;

const float C1 = 0.429043;
const float C2 = 0.511664;
const float C3 = 0.743125;
const float C4 = 0.886227;
const float C5 = 0.247708;

// Constants for Funston Beach Sunset lighting
const vec3 L00  = vec3( 0.6841148,  0.6929004,  0.7069543);
const vec3 L1m1 = vec3( 0.3173355,  0.3694407,  0.4406839);
const vec3 L10  = vec3(-0.1747193, -0.1737154, -0.1657420);
const vec3 L11  = vec3(-0.4496467, -0.4155184, -0.3416573);
const vec3 L2m2 = vec3(-0.1690202, -0.1703022, -0.1525870);
const vec3 L2m1 = vec3(-0.0837808, -0.0940454, -0.1027518);
const vec3 L20  = vec3(-0.0319670, -0.0214051, -0.0147691);
const vec3 L21  = vec3( 0.1641816,  0.1377558,  0.1010403);
const vec3 L22  = vec3( 0.3697189,  0.3097930,  0.2029923);

// Constants for Grace Cathedral lighting
const vec3 L00A  = vec3( 0.78908,  0.43710,  0.54161);
const vec3 L1m1A = vec3( 0.39499,  0.34989,  0.60488);
const vec3 L10A  = vec3(-0.33974, -0.18236, -0.26940);
const vec3 L11A  = vec3(-0.29213, -0.05562,  0.00944);
const vec3 L2m2A = vec3(-0.11141, -0.05090, -0.12231);
const vec3 L2m1A = vec3(-0.26240, -0.22401, -0.47479);
const vec3 L20A  = vec3(-0.15570, -0.09471, -0.14733);
const vec3 L21A  = vec3( 0.56014,  0.21444,  0.13915);
const vec3 L22A  = vec3( 0.21205, -0.05432, -0.30374);

void main(void)
{
   vec3 tnorm      = normal;//normalize(gl_NormalMatrix * normal);

   float pixelOffset = eOffset / TEX_WIDTH;

   

   //DiffuseColor   *= 1.0;//ScaleFactor;
  //LightIntensity = DiffuseContrib * diffuse + SpecularContrib * spec;
  LightIntensity = 1.0;
  /*
  if(vertex.y <= 117.0) 
    {//Below ground constant. We are assuming we're at sunset, and thus sun directional light is parrallel to the ground.
      LightIntensity = mix(0.0, 0.0007, vertex.y / 117.0);
       DiffuseColor    = C1 * L22A * (tnorm.x * tnorm.x - tnorm.y * tnorm.y) +
                      C3 * L20A * tnorm.z * tnorm.z +
                      C4 * L00A -
                      C5 * L20A +
                      2.0 * C1 * L2m2A * tnorm.x * tnorm.y +
                      2.0 * C1 * L21A  * tnorm.x * tnorm.z +
                      2.0 * C1 * L2m1A * tnorm.y * tnorm.z +
                      2.0 * C2 * L11A  * tnorm.x +
                      2.0 * C2 * L1m1A * tnorm.y +
                      2.0 * C2 * L10A  * tnorm.z;

    }
  else
  {*/
      DiffuseColor    = C1 * L22 * (tnorm.x * tnorm.x - tnorm.y * tnorm.y) +
                      C3 * L20 * tnorm.z * tnorm.z +
                      C4 * L00 -
                      C5 * L20 +
                      2.0 * C1 * L2m2 * tnorm.x * tnorm.y +
                      2.0 * C1 * L21  * tnorm.x * tnorm.z +
                      2.0 * C1 * L2m1 * tnorm.y * tnorm.z +
                      2.0 * C2 * L11  * tnorm.x +
                      2.0 * C2 * L1m1 * tnorm.y +
                      2.0 * C2 * L10  * tnorm.z;
      // }
  gl_Position = worldViewProjMatrix * vertex;
  oUv0.xy = uv0.xy;
  oUv0.z = gl_Position.z;
  //vec4 texStart = colour;
  //texStart.x = texStart.x + 16*pixOffset;//vec4(gl_Color, 0.0, 0.0, 0.0);
  //texStart.x = texStart.x; //+ pixOffset;//vec4(gl_Color, 0.0, 0.0, 0.0);
  //float idx = colour.x * 256.0 - 1.0;
  float idx = uv1.x * 256.0 - 1.0;
  //idx = 206;
  
  float blocky = floor((idx) / 16.0);
  float blockx = ((idx) - blocky * 16.0);

  
  gl_TexCoord[0] =  vec4(blockx, blocky, 0, 0) * eOffset + uv0 * eOffset;
  //gl_TexCoord[0].z = gl_Position.z;
  
if(uv0.x == 1.0)
  gl_TexCoord[0].x -= pixelOffset;//uv0.x -= pixelOffset;
 if(uv0.y == 1.0)
   gl_TexCoord[0].y -= pixelOffset;//uv0.y -= pixelOffset;

 //caculate tangent space light vector
 oLightDir = normalize(lightPosition.xyz - (vertex * lightPosition.w).xyz);
 //calculate half-angle in tangent space
 vec3 eyeDir = normalize(eyePosition - vertex.xyz);
 oHalfAngle = normalize(eyeDir + oLightDir);
 
 //calculate the position of vertex in light space
 oLightPosition0 = texWorldViewProjMatrix0 * vertex;
 oLightPosition1 = texWorldViewProjMatrix1 * vertex;
 oLightPosition2 = texWorldViewProjMatrix2 * vertex;
 
 //make linear
 oLightPosition0.z = (oLightPosition0.z - depthRange0.x) * depthRange0.w;
 oLightPosition1.z = (oLightPosition1.z - depthRange1.x) * depthRange1.w;
 oLightPosition2.z = (oLightPosition2.z - depthRange2.x) * depthRange2.w;

 oNormal = normal;

 

 //gl_TexCoord[0] =  vec4(colour.x * 256.0, 0, 0, 0) * eOffset + uv0 * eOffset;//texStart;// + uv0*eOffset;// - pixelOffset;//BlockOffset;
}