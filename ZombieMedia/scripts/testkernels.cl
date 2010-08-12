#pragma OPENCL EXTENSION cl_khr_byte_addressable_store : enable

/**
 *This function will convert the given quaternion to a quaternion represented
 *with the given axis/angle.
 *
 **/
void
AxisAngleToQuaternion(float4* quat, float4* axis, float angleInRad)
{
  *quat = (float4)(1.0f, 1.0f, 1.0f, 1.0f);
  *quat = *axis; //load axis into quat.
  float s = sin(angleInRad/2);
  float4 angles = (float4)(s, s, s, cos(angleInRad/2));
  *quat *= angles;
}
/**
 *This is a quick utility function which returns the 3rd column of matrix after converting a quaternion to said matrix.
 *We assume the quaternion is in this format: (x, y, z, w) where w is the real part.
 *
 *Math and Code idea taken from www.euclideanspace.com
 *
 *\note Should change to pass by reference instead of pointer to make code look cleaner. ((*quat).w is not clean looking)
 *
 **/
void
ZAxisOfMatrixFromQuaternion(float4* quat, float4* zaxis )
{
  float4 sqr = *quat * *quat; //square the matrix by doing component-wise multiplication.
  float invs = 1.0f / (sqr.x + sqr.y + sqr.z + sqr.w);
  //Compute the third column only.
  
  (*zaxis).x = 2.0f * ((*quat).x*(*quat).z + (*quat).y*(*quat).w)*invs; //m20
  (*zaxis).y = 2.0f * ((*quat).y*(*quat).z - (*quat).x*(*quat).w)*invs; //m21
  (*zaxis).z = (-sqr.x - sqr.y + sqr.z + sqr.w)*invs; //m22
  (*zaxis).w = 1.0f; //m23
  
  
  
  
}

/**
 *This function will update the entity with position and orientation, given the relevant pointers, and entity offsets.
 *\Note: One must flip the quaternion from x,y,z,w to w,x,y,z when writing out, as the host program expects it in that format.
 */
void 
UpdateEntity(float4* pos, float4* orient, float4* velocity, float* mode, 
	     __global float* entsPosPtr, __global float* entsOrientPtr, __global float* entsVelPtr,
	     __global uchar* entsModePtr, size_t offset,
	     const float scale, //This is unit per meter scale.
	     float dt,
	     bool impulse)
{
  //Do physics update using RK2.
  
  //float TC = 200.0f; //thrust
  float4 TDir = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
  float4 VNew = (float4)(0.0f, 0.0f, 10.0f, 0.0f);
  //float4 SNew = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
  float4 F = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
  float4 A = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
  float c = 0.3f; //velocity damping term.
  float mass = 7.4051f / scale;
  //float mass = 1.0f;
  float4 k1 = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
  float4 k2 = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
  float4 entZAxis = (float4)(0.0f, 0.0f, 1.0f, 0.0f);
  float4 oldVel;
  ZAxisOfMatrixFromQuaternion(orient, &entZAxis); //Get the ZAxis from orientation quaternion.
  
  oldVel = *velocity;
  if(impulse)
    {
      
      *velocity = 800.0f;
    }
  
  TDir = entZAxis * (*velocity).x;  
  *velocity = oldVel;
  F = TDir - (*velocity * c);
  A = F * (1.0f / mass);
  k1 = A * dt;

  F = (TDir - ((*velocity + k1) * c));
  A = F * (1.0f / mass);
  k2 = A * dt;

  VNew = *velocity + (k1 + k2) / 2.0f;
  
  //const float dtt = 0.16f;
  *pos = *pos + TDir * dt;
  //*pos = *pos + VNew * dt;
  //*velocity = VNew;
  
  
  //Output
  (*pos).w = *mode;
  vstore4(*pos, offset, entsPosPtr);
  (*orient).xyzw = (*orient).wxyz; //flip to the format expected by host program.
  vstore4(*orient, offset, entsOrientPtr);
  vstore4(*velocity, offset, entsVelPtr);
  //entsModePtr[offset] = *mode;
}

__kernel void 
updateEnt(__constant float* gradIn, 
	  __constant float* contourIn,
	  __global float* entsPos,
	  __global float* entsOrient,
	  __global float* entsVel,
	  __global uchar* entsMode,
	  const uint numOfEnts,
	  const uint mapHeight, //Number of rows. Parameter U for u in U.
	  const uint mapWidth, //number of columns. Parameter V for v in V.
	  const float unitsPerBlock, //scale factor. How many units per world map block in meters.
	  const float dt //delta time in seconds
	  )
{
  const float CLIMB = 1.0f;
  const float DESCEND = 2.0f;
  const float EXIT_DESCEND = 3.0f;
  int4 targetCell; 
  float4 dir;
  float4 gradAtCell;
  float dpp;
  float4 YAXIS = (float4)(0.0f, 1.0f, 0.0f, 1.0f);
  const float DPPCMP1 = -0.0001f;
  const float DPPCMP2 = 0.0f;
  bool impulse = false;
  //Get the operating id for the entity that we are going to work with.
  size_t tid = get_global_id(0); //We have four floats per entities.
  
  float4 worldPos = vload4(tid, entsPos);
  float mode = worldPos.w;
  //mode = DESCEND;
  
  float4 worldOrient = vload4(tid, entsOrient); 
  float4 velocity = vload4(tid, entsVel);
  float4 mapPos = worldPos / unitsPerBlock; //transform into map space.
  mapPos = clamp(mapPos, 0.0f, (float)mapHeight);
  mapPos = clamp(mapPos, 0.0f, (float)mapWidth);
  
  int4 cell = convert_int4(mapPos);
  //uchar mode = entsMode[tid];
  
  
  uint cellOffset1 = cell.z * mapHeight + cell.x;

  float4 normal;

  //Read in the direction vector.
  dir = vload4(cellOffset1, gradIn);
  //compute the target cell.
  targetCell = convert_int4(mapPos + dir);//cell + convert_int4(dir); 
  //Read in the gradient at target cell.
  uint cellOffset2 = targetCell.z * mapHeight + targetCell.x;
  gradAtCell = vload4(cellOffset2, gradIn);
  //compute dot produce of entity direction and gradient vector.
  normal = fast_normalize(gradAtCell);
  dpp = dot(dir, normal);
  if(mode == CLIMB) //ascending mode: CLIMB
  { 
       
      if(dpp < DPPCMP1)
	{
	  mode = DESCEND; //DESCENDING mode.
	  impulse = true;
	}
      else
      {
	  //compute quaternion.
	  AxisAngleToQuaternion(&worldOrient, &YAXIS, atan2(dir.x, dir.z));
	  //update entities.
	  UpdateEntity(&worldPos, &worldOrient, &velocity, &mode, entsPos, entsOrient, entsVel, 
		       entsMode, tid, unitsPerBlock, dt, impulse);
	  return;
      }
  }
  
  //Look at contour map.
  dir = vload4(cellOffset1, contourIn);
  //targetCell = cell + convert_int4(dir);
  targetCell = convert_int4(mapPos + dir);
  cellOffset1 = targetCell.z * mapHeight + targetCell.x;
  //NOTE: Remember to clamp target cells. TOo lazy to do it now. LOL.
  gradAtCell = vload4(cellOffset1, gradIn);
  normal = fast_normalize(gradAtCell);
  dpp = dot(dir, normal);
  //Descend mode
  if(mode == EXIT_DESCEND) //If is in exit descend mode.
    mode = CLIMB; //go back to climb mode.
  if(dpp > DPPCMP1)
    {
      impulse = true;
      if(mode != EXIT_DESCEND) //We need to go one more step.
	mode = EXIT_DESCEND; //enter EXIT_DESCEND mode. 
	}
  AxisAngleToQuaternion(&worldOrient, &YAXIS, atan2(dir.x, dir.z));
  UpdateEntity(&worldPos, &worldOrient, &velocity, &mode, entsPos, entsOrient, entsVel, 
	       entsMode, tid, unitsPerBlock, dt, impulse);
   	  
  return;
}
