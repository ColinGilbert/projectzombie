/*
 * CommonFixtures.h
 *
 *  Created on: Sep 21, 2008
 *      Author: bey0nd
 */

#ifndef COMMONFIXTURES_H_
#define COMMONFIXTURES_H_

#include <iostream>
#include <Ogre.h>

struct GPUEntsPropsFixture
{
  GPUEntsPropsFixture() : minx(-100.0f),minz(-100.0f),maxx(100.0f),maxz(100.0f),
  texW(16),texH(16),entHeight(1.0f)
  {

  }
  ~GPUEntsPropsFixture(){}
  Ogre::Real minx; Ogre::Real minz;
  Ogre::Real maxx; Ogre::Real maxz;
  int texW; int texH;
  Ogre::Real entHeight;
};

#endif /* COMMONFIXTURES_H_ */
