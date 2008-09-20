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

using namespace ZGame;


BOOST_AUTO_TEST_SUITE(gpuentsgenprops_test);


BOOST_AUTO_TEST_CASE(test_gpuentsgenprops_extents)
{
  Real minx = -100.0f; Real minz = -100.0f;
  Real maxx = 100.0f; Real maxz = 100.0f;
  GPUEntsGenProps props;

  props.setExtents(minx,minz,maxx,maxz);
  Ogre::AxisAlignedBox testBox;

  testBox = props.getExtents();

  Vector3 min = testBox.getMinimum();
  Vector3 max = testBox.getMaximum();

  if(min.x != minx || min.y != 0.0f || min.z != minz)
    BOOST_FAIL("Miniumum extents did not match.");
  if(max.x != maxx || max.y != 0.0f || max.z != maxz)
    BOOST_FAIL("Maximum extents did not match.");

}

BOOST_AUTO_TEST_CASE(test_gpuentsgenprops_numents)
{
  size_t nums = 100000;
  GPUEntsGenProps props;
  props.setNumOfEntities(nums);
  BOOST_CHECK(nums == props.getNumOfEntities());
}



BOOST_AUTO_TEST_SUITE_END();
