/*
 * GPUEntsDistributor.h
 *
 *  Created on: Sep 18, 2008
 *      Author: bey0nd
 */

#ifndef GPUENTSDISTRIBUTOR_H_
#define GPUENTSDISTRIBUTOR_H_

#include <Ogre.h>

#include <boost/random.hpp>


namespace ZGame
{
  template<class Engine,class Distribution>
  class GPUEntsDistributor
  {
  public:
    typedef Engine EngType;
    typedef Distribution DistType;
    GPUEntsDistributor(EngType eng, DistType distX, DistType distZ);
    virtual ~GPUEntsDistributor();

    void nextPosition(Ogre::Vector3 &pos);

  protected:
    boost::variate_generator<EngType,DistType> _randX;
    boost::variate_generator<EngType,DistType> _randZ;

  };

  template<class Engine,class Distribution>
  GPUEntsDistributor<Engine,Distribution>::GPUEntsDistributor(EngType eng,DistType distX, DistType distZ) :
    _randX(eng,distX),_randZ(eng,distZ)
  {
  }

  template<class Engine,class Distribution>
  GPUEntsDistributor<Engine,Distribution>::~GPUEntsDistributor()
  {
  }

  template<class Engine,class Distribution>
  void GPUEntsDistributor<Engine,Distribution>::nextPosition(Ogre::Vector3 &pos)
  {
    using namespace Ogre;
    pos.x = (Real)_randX();
    pos.y = 0.0;
    pos.z = (Real)_randZ();
  }
}

#endif /* GPUENTSDISTRIBUTOR_H_ */
