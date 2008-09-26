uniform sampler2D imposterTex;
uniform float imposterWidth; //imposter dimension
uniform float imposterHeight;
void main()
{
	const int IMPOSTER = 0;
	const float texDim = 256.0;
	float imposterScaleU = texDim/imposterWidth;
	float imposterScaleV = texDim/imposterHeight; 
	vec2 st = gl_TexCoord[IMPOSTER]*vec2(imposterScaleU,imposterScaleV);
	gl_FragColor = texture2D(imposterTex,st);
	
}