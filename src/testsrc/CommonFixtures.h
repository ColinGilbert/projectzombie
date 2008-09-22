/*
 * CommonFixtures.h
 *
 *  Created on: Sep 21, 2008
 *      Author: bey0nd
 */

#ifndef COMMONFIXTURES_H_
#define COMMONFIXTURES_H_

#include <iostream>
using namespace std;
#include <Ogre.h>
using namespace Ogre;

struct GPUEntsPropsFixture
{
  GPUEntsPropsFixture() : minx(-100.0f),minz(-100.0f),maxx(100.0f),maxz(100.0f),
  texW(16),texH(16)
  {

  }
  ~GPUEntsPropsFixture(){}
  Real minx; Real minz;
  Real maxx; Real maxz;
  int texW; int texH;
};

#endif /* COMMONFIXTURES_H_ */
