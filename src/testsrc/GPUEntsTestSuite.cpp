/*
 * GPUEntsTestSuite.cpp
 * This file is a collection of various tests related to GPUEntities. e.g. View, Controller, Meshbuilder.
 *  Created on: Sep 24, 2008
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
#include "GPUEntsView.h"
#include "GPUEntsMeshBuilder.h"

using namespace ZGame;

namespace GPUEntsTestSuite
{
struct Fix
{
  Fix() : props(0)
  {
    cout << "In FIx() constructor" << endl;
    setup();
  }
  ~Fix()
  {
    cout << "In ~Fix()" << endl;

    if(props)
      delete props;
    cout << "Deleting GPU entities" << endl;

    }
  void setup();
  void createGPUEntities();
  //ZEntity* ent;
  boost::shared_ptr<ZEntity> ent;
  GPUEntsGenProps* props;
  auto_ptr<GPUEntities> gpuEnts;
};

void Fix::setup()
{
  BOOST_TEST_MESSAGE("In Fix::setup");
  GPUEntsPropsFixture f;
  //ent = new ZEntity("TESTENTITY", "robot.mesh");
  ent.reset(new ZEntity("TESTENTITY","rbot.mesh"));
  props = new GPUEntsGenProps(f.texW, f.texH,f.entHeight);
  props->setExtents(f.minx, f.minz, f.maxx, f.maxz);
  createGPUEntities();
}

void Fix::createGPUEntities()
{
  auto_ptr<GPUEntsGenProps> p(props);
  props = 0;
  GPUEntsGen entsGen(ent,p);
  entsGen.build();
  gpuEnts = entsGen.getOutput();
}

}

BOOST_AUTO_TEST_SUITE(gpuentities_test);

BOOST_FIXTURE_TEST_CASE(test_gpuents,GPUEntsTestSuite::Fix)
{
  cout << "GPU Entities name: " << gpuEnts->getName();
  GPUEntsGenProps* props = gpuEnts->getProperties();
  cout << "Number of entities: " << props->getNumOfEntities();
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(gpuentsmeshbld_test);

BOOST_FIXTURE_TEST_CASE(test_gpumeshbld,GPUEntsTestSuite::Fix)
{
  Ogre::MeshPtr ptr = GPUEntsMeshBuilder::build("GPUEntsMesh",gpuEnts->getProperties());
  MeshManager::getSingleton().remove(ptr->getName());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(gpuentsview_test);

BOOST_FIXTURE_TEST_CASE(test_gpuentsview,GPUEntsTestSuite::Fix)
{
  GPUEntsView entsView;
  entsView.attachGPUEnts(gpuEnts.get());
}

BOOST_AUTO_TEST_SUITE_END();



