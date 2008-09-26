//This file defines fragment functions for our flowsim
const float DWIDTH = 256.0;
//void bilerp(in vec2 x, out vec4 qtynew, in sampler2D qty);

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


//The advection function:
//This function advects a quantity.
void advect(in vec2 coords, out vec4 qtynew, //the output quantity
	    in float dt, in float rdx,
	    in sampler2D vel, in sampler2D qty)
{
  //dt = 0.5;
  //vec2 pos = coords - dt*rdx*texture2D(vel,coords).xy;
  vec2 pos = coords - dt*(texture2D(vel,coords).xy/DWIDTH);
  bilerp(pos,qtynew,qty);
  //qtynew = 0.5 + 0.5*qtynew;
  //qtynew = vec4(1.0,0.0,0.0,1.0);
}
//This function sets the boundary conditions for our simulation.
//
//Idea came from Mark Harris's article in GPU Gems.

void boundary(in vec2 coords, //the texture coords
	      in vec2 offset, //offset into interior cells
	      out vec4 bv, //the computed boundary value
	      in float scale, //scale to determine whether pressure or velocity
	      in sampler2D x)
{
  offset /= DWIDTH;
  bv = scale*texture2D(x,coords+offset);
  //bv = vec4(0.0,0.0,0.0,1.0);
}

//This function computes a divergence of a vector field w using finite difference.
void divergence(in vec2 coords, //the texture coords
		out vec4 div, //the output
		in float halfrdx,
		in sampler2D w)
{
  //halfrdx = 0.5/128.0);
  //compute the 4 neighbors
  vec4 wl = texture2D(w,coords-vec2(1,0)/DWIDTH);
  vec4 wr = texture2D(w,coords+vec2(1,0)/DWIDTH);
  vec4 wu = texture2D(w,coords+vec2(0,1)/DWIDTH);
  vec4 wd = texture2D(w,coords-vec2(0,1)/DWIDTH);
  //halfrdx = 0.5;
  div = vec4(halfrdx*((wr.x-wl.x)+(wu.y-wd.y)),0.0,0.0,0.0);
}

//this function implements jacobi iteration
void jacobi(in vec2 coords, //texture coords
	    out vec4 xNew, //output computed values
	    in float alpha, 
	    in float rbeta, //reciprocal of beta
	    in sampler2D x, //x vector in Ax=b
	    in sampler2D b) //b vector in Ax=b
{
  vec4 xl = texture2D(x, coords-vec2(1,0)/DWIDTH);
  vec4 xr = texture2D(x, coords+vec2(1,0)/DWIDTH);
  vec4 xu = texture2D(x, coords+vec2(0,1)/DWIDTH);
  vec4 xd = texture2D(x, coords-vec2(0,1)/DWIDTH);
  vec4 bc = texture2D(b,coords); //b center
  //alpha = -1.0;
  //rbeta = 0.25;
  xNew = (xl + xr + xd + xu + alpha*bc)*rbeta;
}
	    
//this function implements gradient subtract.
//u = w - grad p 
void gradsub(in vec2 coords, //the texture coords
	     out vec4 uNew, //the new velocity
	     in float halfrdx, //0.5 / gridscale
	     in sampler2D p, //pressure scalar field
	     in sampler2D w, //velocity
	     in int pIndex) //index which indicates where pressure scalar field is stored in the component of p texture.
{
  float pl = texture2D(p,coords-vec2(1,0)/DWIDTH)[0];
  float pr = texture2D(p,coords+vec2(1,0)/DWIDTH)[0];
  float pu = texture2D(p,coords+vec2(0,1)/DWIDTH)[0];
  float pd = texture2D(p,coords-vec2(0,1)/DWIDTH)[0];
  uNew = texture2D(w,coords);
  //halfrdx = 0.5;
  uNew.xy -= halfrdx*vec2(pr-pl,pu-pd);
}

void vorticity(in vec2 coords,
	       out float vort,
	       in sampler2D u)
{
  vec4 ul,ur,uu,ud;
  ul = texture2D(u,coords-vec2(1,0)/DWIDTH);
  ur = texture2D(u,coords+vec2(1,0)/DWIDTH);
  uu = texture2D(u,coords+vec2(0,1)/DWIDTH);
  ud = texture2D(u,coords-vec2(0,1)/DWIDTH);

  vort = 0.5*((ur.y-ul.y)-(uu.x-ud.x));
}

void vortForce(in vec2 coords,
	       out vec2 uNew,
	       in float dt,
	       in sampler2D vort,
	       in sampler2D u)
{
  float vl,vr,vu,vd,vc;
  vl = texture2D(vort,coords-vec2(1,0)/DWIDTH).x;
  vr = texture2D(vort,coords+vec2(1,0)/DWIDTH).x;
  vu = texture2D(vort,coords+vec2(0,1)/DWIDTH).x;
  vd = texture2D(vort,coords-vec2(0,1)/DWIDTH).x;
  vc = texture2D(vort,coords).x;

  vec2 force = 0.5*vec2(abs(vu)-abs(vd),abs(vr)-abs(vl));
  
  static const float EPSILON = 2.4414e-4;
  float magSqr=max(EPSILON,dot(force,force));
  force = force*inversesqrt(magSqr);
  force *= 1.0*vc*vec2(1,-1);
  uNew = texture2D(u,coords).xy;
  uNew += dt*force;
}
