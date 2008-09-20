/*
 * ImposterGenTest.cpp
 *
 *  Created on: Sep 20, 2008
 *      Author: bey0nd
 */

#include <iostream>
using namespace std;

#include <Ogre.h>
using namespace Ogre;
#include <boost/test/unit_test.hpp>
#include "Imposter.h"
#include "ImposterGen.h"

using namespace ZGame;

BOOST_AUTO_TEST_SUITE(impostergen_test);

BOOST_AUTO_TEST_CASE(test_impostergen_build)
{
  cout << "Running test_impostergen_build" << endl;
  Imposter imposter("robot.mesh");
  ImposterGen impGen;
  impGen.setInput(&imposter);
  impGen.build();
  //Everything okay.
}

BOOST_AUTO_TEST_SUITE_END();
