//This file defines a set of common util fragment functions.
const float DWIDTH = 256.0;

//This function does bilinear interpolation, with 4 known neighbors, such that:
//
// 12 ------------------ 22
//  |                    |    
//  |                    |
//  |             x      |
//  |                    |
// 11--------------------21 
//With x denoting the location of interest (x marks the spot :).
//
void bilerp(in vec2 x, out vec4 qtynew, in sampler2D qty)
{
  //figure out the texture coords
  vec4 st;
  //x = clamp(x,1.0/DWIDTH,1.0-1.0/DWIDTH);
  st.xy = floor(x*DWIDTH-vec2(0.5,0.5))+vec2(0.5,0.5); //implied translation into simulation domain.
  st.zw = st.xy + vec2(1.0,1.0);
  //st /= 128.0; //translate back into texture space
  //now access the texture
  vec4 t11 = texture2D(qty,st.xy/DWIDTH);
  vec4 t21 = texture2D(qty,st.zy/DWIDTH);
  vec4 t12 = texture2D(qty,st.xw/DWIDTH);
  vec4 t22 = texture2D(qty,st.zw/DWIDTH);
  //now lerp
  
  vec2 factor = x*DWIDTH- st.xy; //i.e s.x - st.x and s.y - st.y gets the factor with respect
                           //to the two axis.
  //bilerp, such that, we compute lerp:
  //l1 = 11 to 21, l2 = 12 to 22, lerping long the x axis.
  //final = l1 to l2, lerping along the y axis. Please refer to any documentation on bilerp.
  qtynew = mix(mix(t11,t21,factor.x),mix(t12,t22,factor.x),factor.y);
}
