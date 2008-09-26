//This file defines a frag program that copies a texture to the current frame buffer.
uniform sampler2D field;

void main()
{
  gl_FragColor = texture2D(field,gl_TexCoord[0].st);
}
