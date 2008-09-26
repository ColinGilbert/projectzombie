//this is the brick vertex program
uniform vec3 LightPosition;

const float SpecularContribution = 0.5;
const float DiffuseContribution = 1.0 - SpecularContribution;

varying float LightIntensity;
varying vec2 MCPosition;

void main()
{
  vec3 ecPosition = vec3(gl_ModelViewMatrix*gl_Vertex);
  vec3 tnorm = normalize(gl_NormalMatrix*gl_Normal);
  vec3 lightVec = normalize(LightPosition-ecPosition);
  vec3 reflectVec = reflect(-lightVec,tnorm);
  vec3 viewVec = normalize(-ecPosition);
  float diffuse = max(dot(lightVec,tnorm),0.0);
  float spec = 0.0;

  if(diffuse>0.0)
    {
      spec = max(dot(reflectVec,viewVec),0.0);
      spec = pow(spec,16.0);
    }
  /* LightIntensity = DiffuseContribution*diffuse*  */
/*     SpecularContribution*spec;  */

  LightIntensity = DiffuseContribution*diffuse;

  MCPosition = gl_Vertex.xy;
  gl_Position = ftransform();
}
