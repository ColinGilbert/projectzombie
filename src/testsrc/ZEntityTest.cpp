/*
 * ZEntityTest.cpp
 *
 *  Created on: Sep 19, 2008
 *      Author: bey0nd
 */
#include <string>
using namespace std;
#include <boost/test/unit_test.hpp>
#include <Ogre.h>
#include "ZEntity.h"

BOOST_AUTO_TEST_SUITE(zentity_test);
BOOST_AUTO_TEST_CASE(test_zentitymeshname)
{
  using namespace ZGame;
  using namespace Ogre;
  string testname = "TESTENTITYMESH";
  string meshname = "robot.mesh";
  ZEntity zentity(testname,meshname);
  const string resultname = zentity.getMeshName();
  BOOST_CHECK((resultname.compare(meshname))==0);
}


BOOST_AUTO_TEST_SUITE_END();
