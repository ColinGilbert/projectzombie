#pragma warning(disable : 4503)
#pragma once
/*
 * PerlinNoiseMapGen.h
 *
 *  Created on: Sep 24, 2010
 *      Author: beyzend
 */

/**
 * \file This file defines interface for a Volume Map generator which is based on lib noise.
 *
 */
#include <vector>
#include <utility>

#include <iostream>
#include <noise.h>
#include <noiseutils.h>
#include "world/MapGenerator.h"
#include "world/WorldDefs.h"



namespace ZGame
{
  namespace World
  {
    class GradientBlockMap
    {
    public:
      void
      addGradientPoint(std::pair<float, uint8_t> pair);
      uint8_t
      getMappedValue(double y);
      void
      finalize();
    private:
      std::vector<std::pair<float, uint8_t> > _gradientMap;
      std::vector<std::pair<std::pair<float, uint8_t>, std::pair<float, uint8_t> > > _valueBucket;
      size_t _numOfBuckets;
    };
    class PerlinNoiseMapGen : public MapGenerator
    {
    public:
      PerlinNoiseMapGen();
      ~PerlinNoiseMapGen();

      static void initGradientPoints();

      void
      generate(PolyVox::UInt8Volume* data, PolyVox::Region region, Ogre::Real pageX, Ogre::Real pageY);

    private:
        struct HeightVal
        {
            uint8_t uValue;
            float value;
        };
    private:
      PolyVox::UInt8Volume *_data;
      //noise::module::Billow _myModule;
      static GradientBlockMap above;
      static GradientBlockMap below;
      float _valRange;
      //noise::module::Perlin finalTerrain;
      /*
      noise::module::RidgedMulti mountainTerrain;
      noise::module::Billow baseFlatTerrain;
      noise::module::ScaleBias flatTerrain;
      noise::module::Perlin terrainType;
      noise::module::Select terrainSelection;
      noise::module::Turbulence finalTerrain;*/
      noisepp::RidgedMultiModule mountainTerrain;
      noisepp::BillowModule baseFlatTerrain;
      noisepp::ScaleBiasModule flatTerrain;
      noisepp::PerlinModule terrainType;
      noisepp::SelectModule terrainSelection;
      noisepp::TurbulenceModule finalTerrain;
      
    };
  }
}

//using namespace noise;
using namespace ZGame::World;
using namespace PolyVox;

using std::cout;
using std::endl;

inline void PerlinNoiseMapGen::generate(UInt8Volume* data, PolyVox::Region region, 
    Ogre::Real pageX, Ogre::Real pageY)
{
    using namespace noisepp;
    using std::make_pair;
    _data = data;

    const int width = region.width();
    const int height = region.height() - 10;
    const int depth = region.depth();
    const float halfHeight = (float)(height) / 2.0;
    const float oceanFloor = halfHeight - 16.0;
    
    const float mod = 1.0 / 16.0;
    HeightVal hVals[WORLD_BLOCK_WIDTH + 1][WORLD_BLOCK_DEPTH + 1];

    Pipeline2D pipeline;

    ElementID id = finalTerrain.addToPipeline(&pipeline);
    PipelineElement2D* finalElement = pipeline.getElement(id);
    Cache *cache = pipeline.createCache();
    //First construct a 2D perlin noise using a cache. Height value is constant and is defined as oceanFloor.
    for(size_t z=0; z <= width; z++)
    {
        for(size_t x = 0; x <= depth; x++)
        {
            Vector3DFloat v3dCurrentPos((float)x, oceanFloor, (float)z);
            //double val = finalTerrain.GetValue(((float) (pageX) + v3dCurrentPos.getX() / (depth - 1)) * mod, (v3dCurrentPos.getY() / (height)) * mod,
              //    ((float) pageY + v3dCurrentPos.getZ() / (width - 1)) * mod);
            
            float val = finalElement->getValue((pageX + v3dCurrentPos.getX() / (depth - 1)) * mod,
                (pageY + v3dCurrentPos.getZ() / (width - 1)) * mod, cache);
            hVals[z][x].uValue = above.getMappedValue(val);
            hVals[z][x].value = oceanFloor + (height - 8.0) * (val + 1.0) / 2.0;
        }
    }
    pipeline.freeCache(cache);
    //Do a flood fill thing. Where if the current height is less than precomputed "height map", then fill with rock.
    //If it's the current height, fill with value stored in height map. Else it is air.
    for(size_t z=region.getLowerCorner().getZ(); z <= region.getUpperCorner().getZ(); z++)
    {
        for(size_t y = region.getLowerCorner().getY(); y <= region.getUpperCorner().getY(); y++)
        {
            for(size_t x = region.getLowerCorner().getX(); x <= region.getUpperCorner().getX(); x++)
            {
                size_t regionZ = z - region.getLowerCorner().getZ();
                size_t regionX = x - region.getLowerCorner().getX();
                size_t val = (size_t)(hVals[regionZ][regionX].value);
                if(y < val)
                {
                    //rocks all the way down
                    _data->setVoxelAt(x, y, z, 2);
                }
                else if(y == val)
                {
                    //_data->setVoxelAt(x, y, z, MaterialDensityPair44(hVals[z][x].uValue, hVals[z][x].uValue > 0 ? MaterialDensityPair44::getMaxDensity()
                  //: MaterialDensityPair44::getMinDensity()));
                    _data->setVoxelAt(x, y, z, hVals[regionZ][regionX].uValue);
                }
                else if(y <= (size_t(halfHeight - 10.0))) //halfHeight is sea level
                {
                    //_data->setVoxelAt(x, y, z, MaterialDensityPair44(206, MaterialDensityPair44::getMaxDensity()));
                    _data->setVoxelAt(x, y, z, 206);
                }
                else
                {
                    //_data->setVoxelAt(x, y, z, MaterialDensityPair44(0, MaterialDensityPair44::getMinDensity()));
                    _data->setVoxelAt(x, y, z, 0);
                }
            }
        }
    }
}

/**
 * THis method will map the given double value into uint8_t from the gradient value map pairs bucket. The uin8_t value represents block types.
 * \precondition value is in the range of -1 to 1. A bucket of gradient, block mappings has been constructed.
 *
 */
inline uint8_t GradientBlockMap::getMappedValue(double y)
{
  using std::vector;
  using std::pair;

  //First transform y into a bucket.
  size_t bucketIdx = (y + 1.0) / 2.0 * _valueBucket.size();
  std::pair<float, uint8_t> x0 = _valueBucket[bucketIdx].first;
  std::pair<float, uint8_t> y0 = _valueBucket[bucketIdx].second;
  //Then linear interpolate between the two gradients in the bucket to compute map.
  //Let's linear map the value into blocks. x0 + t(y0 - x0) = y => t = (y - x0) / (y0 - x0)
  double t = y - x0.first / (y0.first - x0.first);
  if(t < 0.5)
      return x0.second;
  return y0.second;
}
