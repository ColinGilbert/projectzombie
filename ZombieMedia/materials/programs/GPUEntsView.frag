uniform sampler2D imposterTex;

void main()
{
	const int IMPOSTER = 0;
	gl_FragColor = texture2D(imposterTex,gl_TexCoord[IMPOSTER].st);
	
}