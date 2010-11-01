/*
 * VolumeMapView.h
 *
 *  Created on: Sep 21, 2010
 *      Author: beyzend
 */

#pragma once

#include <vector>
#include <Ogre.h>
#include <SurfaceMesh.h>

using std::vector;

namespace ZGame
{
  namespace World
  {
    /**
     * This class will handle the representation layer of Volume Maps. It's main responsibility is to render the VolumeMap.
     */
    class VolumeMapView
    {
    public:
      VolumeMapView();
      virtual
      ~VolumeMapView();

      void
      createRegion(bool regionEmpty, PolyVox::SurfaceMesh<PolyVox::PositionMaterial>* mesh);
      void
      updateRegion(bool regionEmpty, PolyVox::SurfaceMesh<PolyVox::PositionMaterial> *mesh);
      void
      unloadRegion(bool regionEmpty);
      void
      updateOrigin(const Ogre::Vector3 &origin)
      {
        _origin = origin;
      }

    protected:



    private:

      Ogre::ManualObject* _manual;
      Ogre::SceneNode* _root;
      Ogre::Vector3 _origin; //the origin of this block
      Ogre::SceneManager* _scnMgr;

      void
      _initManualObject();
      /** \brief This method will create a manual mesh from the given PolyVox SurfaceMesh.**/
      void
      _manualFromMesh(bool isUpdate, PolyVox::SurfaceMesh<PolyVox::PositionMaterial>* mesh, Ogre::ManualObject* manual);

      void
      _convertColorFromMaterialID(Ogre::ColourValue &val, uint8_t materialID);

    };
  }
}
