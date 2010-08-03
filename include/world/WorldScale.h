#ifndef _ZGAME_WORLD_SCALE_H
#define _ZGAME_WORLD_SCALE_H

#include "Ogre.h"

using Ogre::Real;

namespace ZGame
{
    namespace World
    {
        struct WorldScale
        {
            WorldScale(){}
            virtual ~WorldScale(){}
            Real basisHeightInUnits;
            Real basisMeters;
            Real metersPerUnit;
            Real unitsPerMeter;

            static WorldScale computeAWorldScale(Real basisHeightInUnits = 195.74609375,
                                                 Real basisMeters = 1.9574609375)
            {
                WorldScale worldScale;
                worldScale.basisHeightInUnits = basisHeightInUnits;
                worldScale.basisMeters = basisMeters;
                worldScale.metersPerUnit = basisMeters / basisHeightInUnits;
                worldScale.unitsPerMeter = 1.0f / worldScale.metersPerUnit;

                return worldScale;
            }
        };

        extern WorldScale WSCALE;
        //WSCALE = computeAWorldScale();

    }
}
#endif