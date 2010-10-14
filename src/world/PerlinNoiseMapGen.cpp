/*
 * PerlinNoiseMapGen.cpp
 *
 *  Created on: Sep 24, 2010
 *      Author: beyzend
 */

#include "world/PerlinNoiseMapGen.h"
#include "world/WorldDefs.h"
using namespace noise;
using namespace ZGame::World;
using namespace PolyVox;

using std::cout;
using std::endl;


GradientBlockMap PerlinNoiseMapGen::above;
GradientBlockMap PerlinNoiseMapGen::below;

void PerlinNoiseMapGen::initGradientPoints()
{
  using std::make_pair;
above.addGradientPoint(make_pair(-1.0000, 2));
above.addGradientPoint(make_pair(-0.875, 3));
above.addGradientPoint(make_pair(-0.700, 1));
//low ground layer
above.addGradientPoint(make_pair(0.0625, 1)); //grass
above.addGradientPoint(make_pair(0.3750, 1)); //dirt
above.addGradientPoint(make_pair(0.7500, 2)); //rock
above.addGradientPoint(make_pair(1.0000, 2)); //rock

//Below: rock, dirt, grass, air
below.addGradientPoint(make_pair(-1.0000, 1));
below.addGradientPoint(make_pair(0.0625, 1));
below.addGradientPoint(make_pair(0.7500, 2));
below.addGradientPoint(make_pair(1.0000, 1));
}
void
GradientBlockMap::addGradientPoint(std::pair<double, uint8_t> pair)
{
  _gradientMap.push_back(pair);
}

/**
 * THis method will map the given double value into uint8_t from the gradient value map pairs. The uin8_t value represents block types.
 * \precondition value is in the range of -1 to 1. Number of gradient points is in multiple of two. We also assume gradient points are ordered!
 *
 */
uint8_t
GradientBlockMap::getMappedValue(double y)
{
  using std::vector;
  using std::pair;
  //make sure we have at least two gradient points for now.
  vector<pair<float, uint8_t> >::const_iterator iter;
  for (iter = _gradientMap.begin() + 1; iter != _gradientMap.end(); ++iter)
    {
      if (y <= iter->first)
        {
          //Let's linear map the value into blocks. x0 + t(y0 - x0) = y => t = (y - x0) / (y0 - x0)
          double t = (y - (iter - 1)->first) / (iter->first - (iter - 1)->first);
          double x0 = (double) ((iter - 1)->second);
          //We only do discrete values for blockt types.
          uint8_t types[2] =
            { iter->second, (iter - 1)->second };
          size_t idx = (size_t)(t);
          return types[idx];
        }
    }
  return 0;
}

PerlinNoiseMapGen::PerlinNoiseMapGen() :
  _valRange(7.0f)
{
  using std::make_pair;
  //Above: air, grass, dirt, wood, rock, wall
  //AIR layer

  //finalTerrain.SetFrequency(0.5);
  //finalTerrain.SetOctaveCount(8);
  //finalTerrain.SetPersistence(1.0 / 2.0);

  mountainTerrain.SetFrequency(6.0);
  mountainTerrain.SetOctaveCount(1);
  //mountainTerrain.SetPer
  baseFlatTerrain.SetFrequency(1.0);
  flatTerrain.SetScale(0.25);
  flatTerrain.SetSourceModule(0, baseFlatTerrain);
  flatTerrain.SetBias(-0.75);

  terrainSelection.SetSourceModule(0, flatTerrain);
   terrainSelection.SetSourceModule(1, mountainTerrain);
   terrainSelection.SetControlModule(terrainType);
   terrainSelection.SetBounds(0.0, 1000.0);
   terrainSelection.SetEdgeFalloff(0.125);

   finalTerrain.SetSourceModule(0, terrainSelection);
   finalTerrain.SetFrequency(1.0);
   finalTerrain.SetPower(0.125);

  /*
  finalTerrain.SetFrequency(8.0);
  finalTerrain.SetOctaveCount(1);
  finalTerrain.SetPersistence(3.0 / 4.0);
  //finalTerrain.SetSourceModule(0, finalTerrainInput);
  */
}

PerlinNoiseMapGen::~PerlinNoiseMapGen()
{

}
/*
void
PerlinNoiseMapGen::generate_(Volume<MaterialDensityPair44>* data, Ogre::int32 pageX, Ogre::int32 pageY)
{
  using namespace noise;
  using std::make_pair;

  /*
   module::RidgedMulti mountainTerrain;


   module::Billow baseFlatTerrain;
   baseFlatTerrain.SetFrequency(2.0);

   module::ScaleBias flatTerrain;

   flatTerrain.SetScale(0.25);
   flatTerrain.SetSourceModule(0, baseFlatTerrain);
   flatTerrain.SetBias(-0.75);

   module::Perlin terrainType;
   terrainType.SetFrequency(0.5);
   terrainType.SetPersistence(0.25);

   module::Select terrainSelection;
   terrainSelection.SetSourceModule(0, flatTerrain);
   terrainSelection.SetSourceModule(1, mountainTerrain);
   terrainSelection.SetControlModule(terrainType);
   terrainSelection.SetBounds(0.0, 1000.0);
   terrainSelection.SetEdgeFalloff(0.125);

   module::Turbulence finalTerrain;
   finalTerrain.SetSourceModule(0, terrainSelection);
   finalTerrain.SetFrequency(1.0);
   finalTerrain.SetPower(0.125);
   

  _data = data;
  //Add the gradient points which maps gradients into block types.

  int width = _data->getWidth();
  int height = _data->getHeight();
  int depth = _data->getDepth();
  float halfHeight = (float)(height) / 2.0;
  //_data->setBorderValue(MaterialDensityPair44(0, MaterialDensityPair44::getMaxDensity()));
  const double mod = 1.0 / 32.0; /// 8.0;
  //Vector3DFloat v3dVolCenter(_data->getWidth() / 2, _data->getHeight() / 2, _data->getDepth() / 2);
  for (int z = 0; z < width; z++)
    {
      for (int y = 0; y < height; y++)
        {
          for (int x = 0; x < depth; x++)
            {
              Vector3DFloat v3dCurrentPos(x, y, z);
              uint8_t uValue = 0;
              double val = finalTerrain.GetValue(((float) (pageX) + v3dCurrentPos.getX() / (depth - 1)) * mod, (v3dCurrentPos.getY() / (height - 1)) * mod,
                  ((float) pageY + v3dCurrentPos.getZ() / (width - 1)) * mod);
              /*
               * We differentiate between below and above ground because the scale which maps into block ranges (i.e. value which ranges from grass to rock) is fliped.
               * For above ground, the map is such that the "floor" (in terms of the map generate by Perlin noise) consit mostly of air. It then moves to grass->rock (for terrain).
               * For below ground, it is the opposite. With "Air" consiting of rock. With the upper range air (i.e. pockets of air within in rocks.)
               
              if (v3dCurrentPos.getY() < halfHeight - (10.0 * (val + 1.0f) / 2.0))
                {
                  val = -val; //flip the scale so rock is more.
                  uValue = below.getMappedValue(val);
                }

              else if (v3dCurrentPos.getY() > (float)(height) - (halfHeight * (val + 1.0f) / 2.0))
                {
                  uValue = 0;
                }

              else
                {
                  uValue = above.getMappedValue(val);
                }
              
              uValue = above.getMappedValue(val);
              //cout << "uValue: " << uValue << endl;
              _data->setVoxelAt(x, y, z, MaterialDensityPair44(uValue, uValue > 0 ? MaterialDensityPair44::getMaxDensity()
                  : MaterialDensityPair44::getMinDensity()));
            }
        }
    }

}
*/
void
PerlinNoiseMapGen::generate(Volume<MaterialDensityPair44>* data, Ogre::int32 pageX, Ogre::int32 pageY)
{
    using namespace noise;
    using std::make_pair;
    _data = data;

    const int width = _data->getWidth();
    const int height = _data->getHeight();
    const int depth = _data->getDepth();
    const float halfHeight = (float)(height) / 2.0;
    const float oceanFloor = halfHeight - 16.0;
    const double mod = 1.0 / 32.0;
    HeightVal hVals[WORLD_BLOCK_WIDTH][WORLD_BLOCK_DEPTH];

    //First construct a 2D perlin noise using a cache. Height value is constant and is defined as oceanFloor.
    for(size_t z=0; z < width; z++)
    {
        for(size_t x = 0; x < depth; x++)
        {
            Vector3DFloat v3dCurrentPos(x, oceanFloor, z);
            double val = finalTerrain.GetValue(((float) (pageX) + v3dCurrentPos.getX() / (depth - 1)) * mod, (v3dCurrentPos.getY() / (height - 1)) * mod,
                  ((float) pageY + v3dCurrentPos.getZ() / (width - 1)) * mod);
            hVals[z][x].uValue = above.getMappedValue(val);
            hVals[z][x].value = oceanFloor + (height - 8.0 - oceanFloor) * (val + 1.0) / 2.0;
        }
    }
    //Do a flood fill thing. Where if the current height is less than precomputed "height map", then fill with rock.
    //If it's the current height, fill with value stored in height map. Else it is air.
    for(size_t z=0; z < width; z++)
    {
        for(size_t y = 0; y < height; y++)
        {
            for(size_t x = 0; x < depth; x++)
            {
                float val = (size_t)(hVals[z][x].value);
                if(y < val)
                {
                    //rocks all the way down
                    _data->setVoxelAt(x, y, z, MaterialDensityPair44(2, MaterialDensityPair44::getMaxDensity()));
                }
                else if(y == val)
                {
                    _data->setVoxelAt(x, y, z, MaterialDensityPair44(hVals[z][x].uValue, hVals[z][x].uValue > 0 ? MaterialDensityPair44::getMaxDensity()
                  : MaterialDensityPair44::getMinDensity()));
                }
                else if(y <= halfHeight - 10.0) //halfHeight is sea level
                {
                    _data->setVoxelAt(x, y, z, MaterialDensityPair44(206, MaterialDensityPair44::getMaxDensity()));
                }
                else
                {
                    _data->setVoxelAt(x, y, z, MaterialDensityPair44(0, MaterialDensityPair44::getMinDensity()));
                }
            }
        }
    }
}
