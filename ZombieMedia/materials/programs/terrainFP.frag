uniform sampler2D terrainTexture;

void main()
{
	gl_FragColor = texture2D(terrainTexture,gl_TexCoord[0].st);
}