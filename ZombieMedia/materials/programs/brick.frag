//brick fragment program

uniform vec3 BrickColor, MortarColor;
uniform vec2 BrickSize;
uniform vec2 BrickPct;

varying vec2 MCPosition;
varying float LightIntensity;

void main()
{
  vec3 color;
  vec2 position, useBrick;

  position = MCPosition / BrickSize;
  
  if(fract(position.y*0.5) > 0.5)
    position.x += 0.5;
  
  position = fract(position);

  useBrick = step(position,BrickPct);

  color = mix(MortarColor,BrickColor,useBrick.x*useBrick.y);
  color *= LightIntensity;
  gl_FragColor = vec4(color,1.0);
  //gl_FragColor = vec4(1.0,0.0,0.0,1.0);
}
