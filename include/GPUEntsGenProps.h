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
//#include <string>
#include <memory>
using namespace std;
#include <Ogre.h>
namespace ZGame
{
  class Imposter;
  class GPUEntsGenProps
  {
  public:
    GPUEntsGenProps(const int texWidth, const int texHeight,
        Ogre::Real entsHeight);
    virtual
    ~GPUEntsGenProps();

    size_t
    getNumOfEntities()
    {
      return _numOfEntities;
    }
    void
    setExtents(Ogre::Real minx, Ogre::Real minz, Ogre::Real maxx,
        Ogre::Real maxz);
    const Ogre::AxisAlignedBox&
    getExtents();

    int
    getTexWidth();

    int
    getTexHeight();

    Ogre::Real
    getEntHeight();

    void
    setImposter(auto_ptr<Imposter> &imposter);

    Imposter*
    getImposter();

  protected:
    size_t _numOfEntities;
    Ogre::AxisAlignedBox _extents;
    int _texWidth;
    int _texHeight;
    Ogre::Real _entHeight; //height of entity
    auto_ptr<Imposter> _imposter; //the imposter that compose this set of GPU entities.
    void
    computeNumOfEnts(); //compute number of entities for a given texture width and height


  private:
  };
}

#endif /* GPUENTSGENPROPS_H_ */
