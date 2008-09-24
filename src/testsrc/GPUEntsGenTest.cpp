/*
 * GPUEntsGenTest.cpp
 *
 *  Created on: Sep 21, 2008
 *      Author: bey0nd
 */
#include <iostream>
#include <memory>
using namespace std;
#include <Ogre.h>
using namespace Ogre;
#include <boost/test/unit_test.hpp>
#include "GPUEntsGen.h"
#include "ZEntity.h"
#include "GPUEntities.h"
#include "GPUEntsGenProps.h"
#include "ZTestSuite.h"
#include "CommonFixtures.h"
using namespace ZGame;

struct Fix
{
  Fix() : ent(0),props(0)
  {
    cout << "In FIx() constructor" << endl;
  }
  ~Fix()
  {
    cout << "In ~Fix()" << endl;
    if(ent)
      delete ent;
    if(props)
      delete props;
    }
  void setup();
  ZEntity* ent;
  GPUEntsGenProps* props;
};

void Fix::setup()
{
  BOOST_TEST_MESSAGE("In Fix::setup");
  GPUEntsPropsFixture f;
  ent = new ZEntity("TESTENTITY", "robot.mesh");
  props = new GPUEntsGenProps(f.texW, f.texH,f.entHeight);
  props->setExtents(f.minx, f.minz, f.maxx, f.maxz);
}

BOOST_AUTO_TEST_SUITE(gpuentsgen_test);
BOOST_FIXTURE_TEST_CASE(test_gpuentsgenbuild,Fix)
{
  setup();
  auto_ptr<GPUEntsGenProps> p(props);
  props = 0;
  GPUEntsGen entsGen(ent,p);
  entsGen.build();

}

BOOST_FIXTURE_TEST_CASE(test_gpuentsgenoutput,Fix)
{
  setup();
  auto_ptr<GPUEntsGenProps> p(props);
  props = 0;
  GPUEntsGen entsGen(ent,p);
  entsGen.build();
  auto_ptr<GPUEntities> gpuEnts = entsGen.getOutput();
}

BOOST_AUTO_TEST_SUITE_END();
