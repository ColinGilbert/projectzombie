#pragma OPENCL EXTENSION cl_khr_byte_addressable_store : enable
#pragma OPENCL EXTENSION cl_amd_printf : enable

#define IA 16807                        // a
#define IM 2147483647                   // m
#define AM (1.0/IM)                     // 1/m - To calculate floating point result
#define IQ 127773
#define IR 2836
#define NTAB 4
#define NDIV (1 + (IM - 1)/ NTAB)
#define EPS 1.2e-7
#define RMAX (1.0 - EPS)
#define GROUP_SIZE 80

/* Generate uniform random deviation */
/* Park-Miller with Bays-Durham shuffle and added safeguards
 Returns a uniform random deviate between (-FACTOR/2, FACTOR/2)
 input seed should be negative */
float ran1(int idum, __local int *iv)
  {
    int j;
    int k;
    int iy = 0;
    int tid = get_local_id(0);
    //printf("local id, idum: %d %d\n", tid, idum);

    for(j = NTAB; j >=0; j--) //Load the shuffle

      {
        k = idum / IQ;
        idum = IA * (idum - k * IQ) - IR * k;

        if(idum < 0)
        idum += IM;

        if(j < NTAB)
        iv[NTAB* tid + j] = idum;
      }
    iy = iv[NTAB* tid];

    k = idum / IQ;
    idum = IA * (idum - k * IQ) - IR * k;

    if(idum < 0)
    idum += IM;

    j = iy / NDIV;
    iy = iv[NTAB * tid + j];
    return AM * iy; //AM *iy will be between 0.0 and 1.0
  }

bool
IsInRange(float d, float l, float r)
{
  float x, y;
  //if(d >= l && d <= r)
  //return true;
  x = step(l, d);
  y = step(r, d);
  if (x == 1.0f && y == 0.0f)
    return true;
  return false;
}

float
ComputeTankPot(float4* goal, float4* pos)
{
  //We are working in MAP space.
  const float tol = 0.000001f;
  const float D = 7.0f;
  const float R = 1024.0f;
  const float W = 0.5f;
  const float USCALE = 240.0f;
  const float DF = 2.0f;
  //compute distance to goal
  float d = fast_distance(*goal, *pos);
  float rt = W;
  if (d < tol)
    rt *= 180.0f;
  if (IsInRange(d, 0.0f, D - DF) == true)
    {
      rt *= USCALE / d * (D - 2.0f);
    }
  if (IsInRange(d, D - DF, D) == true)
    {
      rt *= USCALE;
    }
  if (IsInRange(d, D, R) == true)
    {
      rt *= USCALE - 0.24f * (d - D);
    }
  return rt;
}

uint
getCellOffset(int4* cell, const uint mapHeight)
{
  return (*cell).z * mapHeight + (*cell).x;
}

float
DensityGradientAtCell(__global float* density, int4* cell, float4* result, const uint mapHeight, const uint mapWidth)
  {
    const int4 u = (int4)(0, 0, 1, 0);
    const int4 d = (int4)(0, 0, -1, 0);
    const int4 l = (int4)(-1, 0, 0, 0);
    const int4 r = (int4)(1, 0, 0, 0);
    float pl, pr, pu, pd;
    const float half = 0.5f;

    float4 temp;
    int4 ucell = *cell + u;
    int4 dcell = *cell + d;
    int4 lcell = *cell + l;
    int4 rcell = *cell + r;

    ucell = convert_int4(clamp(convert_float4(ucell), 0.0f, (float) mapHeight));
    ucell = convert_int4(clamp(convert_float4(ucell), 0.0f, (float) mapWidth));
    dcell = convert_int4(clamp(convert_float4(dcell), 0.0f, (float) mapHeight));
    dcell = convert_int4(clamp(convert_float4(dcell), 0.0f, (float) mapWidth));
    lcell = convert_int4(clamp(convert_float4(lcell), 0.0f, (float) mapHeight));
    lcell = convert_int4(clamp(convert_float4(lcell), 0.0f, (float) mapWidth));
    rcell = convert_int4(clamp(convert_float4(rcell), 0.0f, (float) mapHeight));
    rcell = convert_int4(clamp(convert_float4(rcell), 0.0f, (float) mapWidth));

    //grab the density of the 4 cells.
    uint offset = getCellOffset(&ucell, mapHeight);
    pu = -vload4(offset, density).x;
    //printf("pu: %f\n", pu);
    offset = getCellOffset(&dcell, mapHeight);
    pd = -vload4(offset, density).x;
    //printf("pd %f\n", pd);
    offset = getCellOffset(&lcell, mapHeight);
    pl = -vload4(offset, density).x;
    //printf("pl %f\n", pl);
    offset = getCellOffset(&rcell, mapHeight);
    pr = -vload4(offset, density).x;
    //printf("pr %f\n", pr);

    //compute potential
    //Compute gradient;
    (*result).z = (pu - pd) * half;
    (*result).x = (pr - pl) * half;

  }

void
GradientAtCell(float4* goal, int4* cell, float4* result, const uint mapHeight, const uint mapWidth)
{
  const int4 u = (int4)(0, 0, 1, 0);
  const int4 d = (int4)(0, 0, -1, 0);
  const int4 l = (int4)(-1, 0, 0, 0);
  const int4 r = (int4)(1, 0, 0, 0);
  float pl, pr, pu, pd;
  const float half = 0.5f;
  float4 temp;
  int4 ucell = *cell + u;
  int4 dcell = *cell + d;
  int4 lcell = *cell + l;
  int4 rcell = *cell + r;
  ucell = convert_int4(clamp(convert_float4(ucell), 0.0f, (float) mapHeight));
  ucell = convert_int4(clamp(convert_float4(ucell), 0.0f, (float) mapWidth));
  dcell = convert_int4(clamp(convert_float4(dcell), 0.0f, (float) mapHeight));
  dcell = convert_int4(clamp(convert_float4(dcell), 0.0f, (float) mapWidth));
  lcell = convert_int4(clamp(convert_float4(lcell), 0.0f, (float) mapHeight));
  lcell = convert_int4(clamp(convert_float4(lcell), 0.0f, (float) mapWidth));
  rcell = convert_int4(clamp(convert_float4(rcell), 0.0f, (float) mapHeight));
  rcell = convert_int4(clamp(convert_float4(rcell), 0.0f, (float) mapWidth));
  //compute potential
  temp = convert_float4(lcell);
  pl = ComputeTankPot(goal, &temp);
  temp = convert_float4(rcell);
  pr = ComputeTankPot(goal, &temp);
  temp = convert_float4(ucell);
  pu = ComputeTankPot(goal, &temp);
  temp = convert_float4(dcell);
  pd = ComputeTankPot(goal, &temp);
  //Compute gradient;
  (*result).z = (pu - pd) * half;
  (*result).x = (pr - pl) * half;
}

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
  float s = sin(angleInRad / 2);
  float4 angles = (float4)(s, s, s, cos(angleInRad / 2));
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
ZAxisOfMatrixFromQuaternion(float4* quat, float4* zaxis)
{
  float4 sqr = *quat * *quat; //square the matrix by doing component-wise multiplication.
  float invs = 1.0f / (sqr.x + sqr.y + sqr.z + sqr.w);
  //Compute the third column only.

  (*zaxis).x = 2.0f * ((*quat).x * (*quat).z + (*quat).y * (*quat).w) * invs; //m20
  (*zaxis).y = 2.0f * ((*quat).y * (*quat).z - (*quat).x * (*quat).w) * invs; //m21
  (*zaxis).z = (-sqr.x - sqr.y + sqr.z + sqr.w) * invs; //m22
  (*zaxis).w = 1.0f; //m23


}

/**
 *This function will update the entity with position and orientation, given the relevant pointers, and entity offsets.
 *\Note: One must flip the quaternion from x,y,z,w to w,x,y,z when writing out, as the host program expects it in that format.
 */
void
UpdateEntity(float4* pos, float4* orient, float4* velocity, float4* goal, float4* stone, int* mode,
    __global float* entsPosPtr, __global float* entsOrientPtr, __global float* entsVelPtr, __global float* goals, __global float* storeone,
    size_t offset,
    const float scale, //This is unit per meter scale.
    float dt,
    bool impulse)
  {
    //Do physics update using RK2.

    //float TC = 200.0f; //thrust
    float4 TDir = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
    float4 F = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
    float4 A = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
    const float c = 0.9f; //velocity damping term.
    //const float mass = 1.0f / scale;
    const float mass = 1.0f;
    //const float c = 1.0f;
    float4 k1 = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
    float4 k2 = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
    float4 entZAxis = (float4)(0.0f, 0.0f, 1.0f, 0.0f);
    const float oldThrust = (*velocity).w;
    (*velocity).w = 0.0f;
    float thrust = oldThrust;
    ZAxisOfMatrixFromQuaternion(orient, &entZAxis); //Get the ZAxis from orientation quaternion.
    const float4 gravity = (float4)(0.0f, -9.8f, 0.0f, 0.0f) / scale;

    if(impulse)
      {
        thrust = 350.0f;
      }

    TDir = entZAxis * thrust;

    F = TDir - (*velocity * c);
    F = F + gravity;
    A = F * (1.0f / mass);
    k1 = A * dt;

    F = TDir - ((*velocity + k1) * c);
    A = F * (1.0f / mass);
    k2 = A * dt;

    *velocity = (*velocity) + (k1 + k2) / 2.0f;
    *pos = *pos + *velocity * dt;
    
    
    //*pos = *pos + TDir * dt;

    //Output
    (*pos).w = convert_float(*mode);
    (*velocity).w = oldThrust; //restore thrust.
    vstore4(*pos, offset, entsPosPtr);
    (*orient).xyzw = (*orient).wxyz; //flip to the format expected by host program.
    vstore4(*orient, offset, entsOrientPtr);
    vstore4(*velocity, offset, entsVelPtr);
    vstore4(*goal, offset, goals);
    vstore4(*stone, offset, storeone);
  }

__kernel void
updateEnt(__constant float* gradIn,
    __constant float* contourIn,
    __global float* entsPos,
    __global float* entsOrient,
    __global float* entsVel,
    __global float* goals,//__global uchar* entsMode,
    __global float* density,
    __global float* storeone,
    const uint numOfEnts,
    const uint mapHeight, //Number of rows. Parameter U for u in U.
    const uint mapWidth, //number of columns. Parameter V for v in V.
    const float unitsPerBlock, //scale factor. How many units per world map block in meters.
    const float dt //delta time in seconds
)
  {
    const int CLIMB = 1.0f;
    const int DESCEND = 2.0f;
    const int EXIT_DESCEND = 3.0f;
    // const int AT_GOAL = 4.0f;
    int4 targetCell;
    float4 cGrad;
    float4 goalCGrad; //computed from our goal potential
    float4 tgtCGrad = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
    float4 tgtGoalCGrad = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
    float4 normal;

    float dpp;
    float4 YAXIS = (float4)(0.0f, 1.0f, 0.0f, 1.0f);
    const float DPPCMP1 = -0.0001f;
    bool impulse = false;
    //Get the operating id for the entity that we are going to work with.
    size_t tid = get_global_id(0); //We have four floats per entities.
    //const int GROUP_SIZE = get_local_size(0);

    float randGoal = 10.0f;
    float di;
    float4 worldPos = vload4(tid, entsPos);
    int mode = convert_int(worldPos.w);
    __local int iv[NTAB * GROUP_SIZE];
    float4 goal = vload4(tid, goals);
    float4 goalOld = goal;
    float urand = ran1(-goal.w, iv);
    float seed = urand * IM;
    goalOld.w = seed; //save the seed.
    goal.w = 0.0f;
    goal /= unitsPerBlock;
    float4 worldOrient = vload4(tid, entsOrient);
    float4 velocity = vload4(tid, entsVel);
    float4 mapPos = worldPos / unitsPerBlock; //transform into map space.

    float densityLength;

    float4 stone = vload4(tid, storeone);

    mapPos = clamp(mapPos, 0.0f, (float)mapHeight);
    mapPos = clamp(mapPos, 0.0f, (float)mapWidth);

    int4 cell = convert_int4(mapPos);
    uint cellOffset1 = cell.z * mapHeight + cell.x;

    //printf("tid: %d", tid);

    di = fast_distance(mapPos, goal);

    //printf("goal: %f,%f,%f. seed: %f. urand: %f. di: %f. randGoal: %f\n", goal.x, goal.y, goal.z, seed, urand, di, randGoal);
    if(mode == CLIMB) //ascending mode: CLIMB

      {
        //Read in the direction vector.
        cGrad = vload4(cellOffset1, gradIn);
        GradientAtCell(&goal, &cell, &goalCGrad, mapHeight, mapWidth);
        cGrad += goalCGrad;
        //add the density gradient for stupid crowd avoidance.
        DensityGradientAtCell(density, &cell, &goalCGrad, mapHeight, mapWidth);
        densityLength = fast_length(goalCGrad);
        cGrad += goalCGrad;

        //compute the target cell.
        targetCell = convert_int4(mapPos + cGrad);//cell + convert_int4(dir);
        //Read in the gradient at target cell.
        uint cellOffset2 = targetCell.z * mapHeight + targetCell.x;
        tgtCGrad = vload4(cellOffset2, gradIn);
        //Compute the goal graidents

        GradientAtCell(&goal, &targetCell, &tgtGoalCGrad, mapHeight, mapWidth);
        //implied linear gradient operator. Linear independence P(x+y) = P(x) + P(y)

        tgtCGrad += tgtGoalCGrad;

        //compute dot produce of entity direction and gradient vector.
        //normal = fast_normalize(tgtCGrad);

        dpp = dot(fast_normalize(cGrad), fast_normalize(tgtCGrad));

        if(dpp < DPPCMP1 && densityLength < 0.00000001)
          {
            mode = DESCEND; //DESCENDING mode.
            impulse = true;
          }
        else
          {
            if ( di < 5.0 + randGoal)
              {
                //play proability game.
                if ( di < stone.x )
                return;
              }
            else
              {
                stone.x = 5.0 + urand * randGoal;
                //compute quaternion.
                AxisAngleToQuaternion(&worldOrient, &YAXIS, atan2(cGrad.x, cGrad.z));
                //update entities.
                UpdateEntity(&worldPos, &worldOrient, &velocity, &goalOld, &stone, &mode, entsPos, entsOrient, entsVel, goals, storeone,
                    tid, unitsPerBlock, dt, impulse);
                return;
              }

          }
      }

    //Look at contour map.
    cGrad = vload4(cellOffset1, contourIn);
    //targetCell = cell + convert_int4(dir);
    targetCell = convert_int4(mapPos + cGrad);
    cellOffset1 = targetCell.z * mapHeight + targetCell.x;
    //NOTE: Remember to clamp target cells. TOo lazy to do it now. LOL.
    tgtCGrad = vload4(cellOffset1, gradIn);
    normal = fast_normalize(tgtCGrad);
    dpp = dot(cGrad, normal);
    //Descend mode
    if(mode == EXIT_DESCEND) //If is in exit descend mode.
      mode = CLIMB; //go back to climb mode.
    if(dpp > DPPCMP1)
      {
        impulse = true;
        if(mode != CLIMB) //We need to go one more step.
        mode = EXIT_DESCEND; //enter EXIT_DESCEND mode.
      }

    if ( di < 5.0 + randGoal)
      {
        //play proability game.
        if ( di < stone.x )
        return;
      }
    else
      {
        stone.x = 5.0 + urand * randGoal;
        //compute quaternion.
        AxisAngleToQuaternion(&worldOrient, &YAXIS, atan2(cGrad.x, cGrad.z));
        UpdateEntity(&worldPos, &worldOrient, &velocity, &goalOld, &stone, &mode, entsPos, entsOrient, entsVel, goals, storeone,
            tid, unitsPerBlock, dt, impulse);

        return;
      }

  }
