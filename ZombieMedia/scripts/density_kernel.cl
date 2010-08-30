#pragma OPENCL EXTENSION cl_amd_printf : enable

#define DENSITY 1.0f
//This is a kernel.

//__kernel
__kernel void updateDensity(__global float* entsPos,
    __global float* density,
    const uint mapHeight,
    const uint mapWidth,
    const float unitsPerBlock)
{
  size_t tid = get_global_id(0);

  //Write density value into grid.
  float4 worldPos = vload4(tid, entsPos);
  float4 mapPos = worldPos / unitsPerBlock;

  mapPos = clamp(mapPos, 0.0f, (float)mapHeight);
  mapPos = clamp(mapPos, 0.0f, (float)mapWidth);

  int4 cell = convert_int4(mapPos);
  uint cellOffset1 = cell.z * mapHeight + cell.x;

  float4 RD = vload4(cellOffset1, density);

  float4 WD = (float4)(DENSITY, 0.0f, 0.0f, 0.0f);

  printf("Density: %f\n", RD.x);

  WD += RD;
  //write density
  vstore4(WD, cellOffset1, density);
  //mem_fence(CLK_GLOBAL_MEM_FENCE);
}
