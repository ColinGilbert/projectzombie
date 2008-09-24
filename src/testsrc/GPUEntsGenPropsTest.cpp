/*
 * GPUEntsGenPropsTest.cpp
 *
 *  Created on: Sep 19, 2008
 *      Author: bey0nd
 */

#include <iostream>

using namespace std;

#include <Ogre.h>
using namespace Ogre;
#include <boost/test/unit_test.hpp>
#include "GPUEntsGenProps.h"
#include "ZTestSuite.h"
#include "CommonFixtures.h"

using namespace ZGame;



BOOST_AUTO_TEST_SUITE(gpuentsgenprops_test);


BOOST_AUTO_TEST_CASE(test_gpuentsgenprops)
{
  GPUEntsPropsFixture f;

  GPUEntsGenProps props(f.texW,f.texH,f.entHeight);

  props.setExtents(f.minx,f.minz,f.maxx,f.maxz);
  Ogre::AxisAlignedBox testBox;

  testBox = props.getExtents();

  Vector3 min = testBox.getMinimum();
  Vector3 max = testBox.getMaximum();

  if(min.x != f.minx || min.y != 0.0f || min.z != f.minz)
    BOOST_FAIL("Miniumum extents did not match.");
  if(max.x != f.maxx || max.y != 0.0f || max.z != f.maxz)
    BOOST_FAIL("Maximum extents did not match.");

  //test width and height
  if(props.getTexWidth() != f.texW || props.getTexHeight() != f.texH)
    BOOST_FAIL("Texture width and height did not match!");

  //test num of entities
  if(props.getNumOfEntities() != f.texW*f.texH)
    BOOST_FAIL("Number of entities is incorrect!");

}





BOOST_AUTO_TEST_SUITE_END();
