/*
 * GPUEntsDistributorTest.cpp
 *
 *  Created on: Sep 19, 2008
 *      Author: bey0nd
 */

#include <boost/random.hpp>
#include <Ogre.h>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ZombieTesting
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "GPUEntsDistributor.h"

BOOST_AUTO_TEST_SUITE(gpuentdist_test);
BOOST_AUTO_TEST_CASE(test_gpuentdist_nextposition)
{
  using namespace ZGame;
  using namespace Ogre;
  int START_X = 1; int END_X = 6;
  int START_Z = 1; int END_Z = 7;
  boost::mt19937 rng;
  boost::uniform_int<> xDist(START_X,END_X); //distribution over x
  boost::uniform_int<> zDist(START_Z,END_Z); //distribution over z

  GPUEntsDistributor<boost::mt19937,boost::uniform_int<> > dist(rng,xDist,zDist);

  Ogre::Vector3 pos;
  dist.nextPosition(pos);
  //check the position distribution. We're doing simple range checks, if you want to get fancy we can
  //actually CHECK to see if the position is actually in the DISTRIBUTION. But I think that's overkill.
  BOOST_CHECK(pos.x >= START_X && pos.x <= END_X); //check X range.
  BOOST_CHECK(pos.z >= START_Z && pos.z <= END_Z);

}
BOOST_AUTO_TEST_SUITE_END();

