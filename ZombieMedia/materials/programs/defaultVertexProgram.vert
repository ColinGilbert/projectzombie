//This is the default vertex program
void main()
{
  //transport the texture coords
  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_Position = ftransform();
}
