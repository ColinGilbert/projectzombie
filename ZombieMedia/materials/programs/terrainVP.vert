uniform sampler2D heightMap;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	vec4 position = gl_Vertex;
	//position.z = texture2D(heightMap,gl_TexCoord[0].st); //displace Z from terrain height map
    position.y = texture2D(heightMap,gl_TexCoord[0].st).r;
    
    //position = (gl_ModelViewProjectionMatrix*position);
    gl_Position= gl_ModelViewProjectionMatrix*position;
}