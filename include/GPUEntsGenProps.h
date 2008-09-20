/*
 * GPUEntsGenProps.h
 * This class defines the properties used by GPUEntsGen to generate GPU entities.
 * Note:
 * Once the editor is implement we can for example defines a set of regions where the entities are, then
 * have the entities generator random distribute entities through-out that region.
 *  Created on: Sep 19, 2008
 *      Author: bey0nd
 */

#ifndef GPUENTSGENPROPS_H_
#define GPUENTSGENPROPS_H_
#include <Ogre.h>
namespace ZGame
{
  class GPUEntsGenProps
  {
  public:
    GPUEntsGenProps();
    virtual ~GPUEntsGenProps();

    void setNumOfEntities(size_t nums) {_numOfEntities = nums;}
    size_t getNumOfEntities() {return _numOfEntities;}
    void setExtents(Ogre::Real minx,Ogre::Real minz,Ogre::Real maxx,Ogre::Real maxz);
    const Ogre::AxisAlignedBox& getExtents() {return _extents;}
  protected:
    size_t _numOfEntities;
    Ogre::AxisAlignedBox _extents;

  private:
  };
}

#endif /* GPUENTSGENPROPS_H_ */
