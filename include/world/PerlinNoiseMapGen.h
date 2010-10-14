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
#include <Ogre.h>
#include <MaterialDensityPair.h>
#include <Volume.h>

namespace ZGame
{
  namespace World
  {
    class GradientBlockMap
    {
    public:
      void
      addGradientPoint(std::pair<double, uint8_t> pair);
      uint8_t
      getMappedValue(double y);

    private:
      std::vector<std::pair<float, uint8_t> > _gradientMap;
    };
    class PerlinNoiseMapGen
    {
    public:
      PerlinNoiseMapGen();
      ~PerlinNoiseMapGen();

      static void initGradientPoints();

      void
      generate(PolyVox::Volume<PolyVox::MaterialDensityPair44>* data, Ogre::int32 pageX, Ogre::int32 pageY);

    private:
        struct HeightVal
        {
            uint8_t uValue;
            float value;
        };
    private:
      PolyVox::Volume<PolyVox::MaterialDensityPair44> *_data;
      //noise::module::Billow _myModule;
      static GradientBlockMap above;
      static GradientBlockMap below;
      float _valRange;
      //noise::module::Perlin finalTerrain;
      noise::module::RidgedMulti mountainTerrain;
      noise::module::Billow baseFlatTerrain;
      noise::module::ScaleBias flatTerrain;
      noise::module::Perlin terrainType;
      noise::module::Select terrainSelection;
      noise::module::Turbulence finalTerrain;
      //noise::module::Cache finalTerrainInput;

    };
  }
}

