#define NUM_SHADOW_SAMPLES_1D 2.0
#define SHADOW_FILTER_SCALE 1

#define SHADOW_SAMPLES NUM_SHADOW_SAMPLES_1D*NUM_SHADOW_SAMPLES_1D

uniform sampler2D diffuseMap;
uniform float invShadowMapSize0;
uniform float invShadowMapSize1;
uniform float invShadowMapSize2;
uniform vec4 pssmSplitPoints;
//uniform sampler2D specular;
uniform sampler2D normalMap;
uniform sampler2D shadowMap0;
uniform sampler2D shadowMap1;
uniform sampler2D shadowMap2;
uniform vec4 lightDiffuse;
uniform vec4 lightSpecular;
uniform vec4 ambient;

varying vec4 oUv0;
varying vec3 oLightDir;
varying vec3 oHalfAngle;
varying vec4 oLightPosition0;
varying vec4 oLightPosition1;
varying vec4 oLightPosition2;
varying vec3 oNormal;


varying vec3 DiffuseColor;
varying float LightIntensity;


vec4 offsetSample(vec4 uv, vec2 offset, float invMapSize)
{
	return vec4(uv.xy + offset * invMapSize * uv.w, uv.z, uv.w);
}

float calcDepthShadow(sampler2D shadowMap, vec4 uv, float invShadowMapSize)
{
	// 4-sample PCF
	
	float shadow = 0.0;
	float offset = (NUM_SHADOW_SAMPLES_1D/2 - 0.5) * SHADOW_FILTER_SCALE;
	for (float y = -offset; y <= offset; y += SHADOW_FILTER_SCALE)
		for (float x = -offset; x <= offset; x += SHADOW_FILTER_SCALE)
		{
			float depth = texture2DProj(shadowMap, offsetSample(uv, vec2(x, y), invShadowMapSize)).x;
			if (depth >= 1 || depth >= uv.z)
				shadow += 1.0;
		}

	shadow /= SHADOW_SAMPLES;

	return shadow;
}
float shadowPCF(sampler2D shadowMap, vec4 shadowMapPos, vec2 offset)
{
	shadowMapPos = shadowMapPos / shadowMapPos.w;
	vec2 uv = shadowMapPos.xy;
	vec3 o = vec3(offset, -offset.x) * 0.3;

	// Note: We using 2x2 PCF. Good enough and is alot faster.
	float c =	(shadowMapPos.z <= texture2D(shadowMap, uv.xy - o.xy).r) ? 1.0 : 0.0; // top left
	c +=		(shadowMapPos.z <= texture2D(shadowMap, uv.xy + o.xy).r) ? 1.0 : 0.0; // bottom right
	c +=		(shadowMapPos.z <= texture2D(shadowMap, uv.xy + o.zy).r) ? 1.0 : 0.0; // bottom left
	c +=		(shadowMapPos.z <= texture2D(shadowMap, uv.xy - o.zy).r) ? 1.0 : 0.0; // top right

	return c / 4.0;
}

float calcPSSMDepthShadow(sampler2D sMap0, sampler2D sMap1, sampler2D sMap2,
			  vec4 lp0, vec4 lp1, vec4 lp2,
			  float invSMSize0, float invSMSize1, float invSMSize2,
			  vec4 pssmSP, float camDepth)
{
  vec4 splitColour;
  float shadowing;
  if (camDepth <= pssmSP.y)
	{
		splitColour = vec4(0.1, 0.0, 0.0, 1.0);
		shadowing = shadowPCF(sMap0, lp0, vec2(invSMSize0));
	}
	else if (camDepth <= pssmSP.z)
	{
		splitColour = vec4(0.0, 0.1, 0.0, 1.0);
		shadowing = shadowPCF(shadowMap1, oLightPosition1,vec2(invSMSize1));
	}
	else
	{
		splitColour = vec4(0.1, 0.1, 0.0, 1.0);
		shadowing = shadowPCF(shadowMap2, oLightPosition2, vec2(invSMSize2));
	}
  return shadowing;
}

float computePSSMDepthShadow(sampler2D sMap0, sampler2D sMap1, sampler2D sMap2,
			  vec4 lp0, vec4 lp1, vec4 lp2,
			  float invSMSize0, float invSMSize1, float invSMSize2,
			  vec4 pssmSP, float camDepth)
{
  float shadow;
  vec4 splitColour;
  //calculate shadow
  // calculate shadow
	if (camDepth <= pssmSP.y)
	{
		splitColour = vec4(0.3, 0.0, 0, 0);
		shadow = calcDepthShadow(sMap0, lp0, invSMSize0);
	}
	else if (camDepth <= pssmSP.z)
	{
		splitColour = vec4(0, 0.3, 0, 0);
		shadow = calcDepthShadow(sMap1, lp1, invSMSize1);
	}
	else
	{
		splitColour = vec4(0.0, 0.0, 0.3, 0);
		shadow = calcDepthShadow(sMap2, lp2, invSMSize2);
	}
	return shadow;
}

vec3 calcPSSMDebugShadow(sampler2D shadowMap0, sampler2D shadowMap1, sampler2D shadowMap2,
			 vec4 lsPos0, vec4 lsPos1, vec4 lsPos2,
			 float invShadowmapSize0, float invShadowmapSize1, float invShadowmapSize2,
			 vec4 pssmSplitPoints, float camDepth)
{
	vec4 splitColour;
	// calculate shadow
	if (camDepth <= pssmSplitPoints.y)
	{
		//splitColour = float4(0.3, 0.0, 0, 0);
		//splitColour = lsPos0 / lsPos0.w;
	        splitColour.rgb = vec3(texture2DProj(shadowMap0, lsPos0).x);
	}
	else if (camDepth <= pssmSplitPoints.z)
	{
		//splitColour = float4(0, 0.3, 0, 0);
		//splitColour = lsPos1 / lsPos1.w;
	  splitColour.rgb = vec3(texture2DProj(shadowMap1, lsPos1).x);
	}
	else
	{
		//splitColour = float4(0.0, 0.0, 0.3, 0);
		//splitColour = lsPos2 / lsPos2.w;
	  splitColour.rgb = vec3(texture2DProj(shadowMap2, lsPos2).x);
	}

	return splitColour.rgb;
}


void main()
{
	
  //compute shadow
  float shadowing = 1.0;
  
  float camDepth = oUv0.z;
  float shadow = computePSSMDepthShadow(shadowMap0, shadowMap1, shadowMap2,oLightPosition0, oLightPosition1, oLightPosition2,invShadowMapSize0, invShadowMapSize1, invShadowMapSize2,pssmSplitPoints, camDepth);

  
  //retrieve normalized light vector, expand from range-compressed
  //vec3 lightVec = normalize(oLightDir);
  
  //retrieve half angle and normalze through cube map
  //vec3 halfAngle = normalize(oHalfAngle);

  //get diffuse color
  //vec4 diffuseColour = texture2D(diffuse, tgl_TexCoord[0].xy);
  vec4 lightColor = texture2D(diffuseMap, gl_TexCoord[0].xy);
  lightColor.xyz *= DiffuseColor;// * mix(0.5, 1.0, shadow);
  gl_FragColor = lightColor;
  
  //gl_FragColor = vec4(calcPSSMDebugShadow(shadowMap0, shadowMap1, shadowMap2,oLightPosition0, oLightPosition1, oLightPosition2,invShadowMapSize0, invShadowMapSize1, invShadowMapSize2,pssmSplitPoints, camDepth), 1.0);
  
  //gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
