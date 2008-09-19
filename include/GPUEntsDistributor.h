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
    GPUEntsDistributor(Engine eng, Distribution dist);
    virtual ~GPUEntsDistributor();

    void nextPosition(Ogre::Vector3 &pos);

  protected:
    typedef Engine Eng_Type;
    typedef Distribution Dist_Type;

    boost::variate_generator<Eng_Type,Dist_Type> _rand;

  };

  template<class Engine,class Distribution>
  GPUEntsDistributor<Engine,Distribution>::GPUEntsDistributor(Engine eng,Distribution dist) :
    _rand(eng,dist)
  {
    //_rand = new boost::variate_generator<Eng_Type,Dist_Type>(eng,dist);
  }

  template<class Engine,class Distribution>
  GPUEntsDistributor<Engine,Distribution>::~GPUEntsDistributor()
  {
  }

  template<class Engine,class Distribution>
  void GPUEntsDistributor<Engine,Distribution>::nextPosition(Ogre::Vector3 &pos)
  {
    //generate position
  }
}

#endif /* GPUENTSDISTRIBUTOR_H_ */
