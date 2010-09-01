#include <iostream>
using std::cout;
using std::endl;
#include <boost/random.hpp>
#include <Ogre.h>
#include "entities/ZEntityBuilder.h"
#include "CommandController.h"
#include "command/CommandList.h"
#include "command/CreateEntCmd.h"
#include "entities/ZEntity.h"
#include "delegates/EntityDelegates.h"
#include "GPUEntsDistributor.h"
#include "world/WorldScale.h"

using namespace ZGame::Entities;

extern ZGame::World::WorldScale WSCALE;


/**
*This method will build specified number of entities. The user has the option to specify whether to use the command and control system to generate a system
*wide command which correspond to the operation of building entities (e.g. generate RenderEntities.)
*
*
*
*/
bool 
EntitiesBuilder::build(EntitiesManager* entMgr)
{
    using namespace Ogre;
    using ZGame::COMMAND::CreateEntCmd;
    using ZGame::Entities::ZENTITY_VEC;
    using ZGame::GPUEntsDistributor;
    int numOfEnts = entMgr->getNumOfEntities();


    //Loop through each entities and randomly assign coords.
    ZENTITY_VEC const* ents = entMgr->getEntities();
    ZENTITY_VEC::const_iterator iter;
    std::vector<Entities::EntitiesGroup*> groups = entMgr->getGroups();

    Vector3 min = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 max = Vector3(500.0f*WSCALE.unitsPerMeter, 0.0f, 25.0f*WSCALE.unitsPerMeter);
    boost::minstd_rand rng;

    //Generate distrubtion on a unit circle.
    boost::uniform_on_sphere<> unitSDist(2.0);
    boost::variate_generator<boost::minstd_rand, boost::uniform_on_sphere<> > randOnCircle(rng, unitSDist);
    int grpIdx = 0;
    int endIdx = groups[grpIdx]->numOfEnts;
    Vector3 center = groups[grpIdx]->center;
    Real radius = groups[grpIdx]->radius*WSCALE.unitsPerMeter;
    //boost::uniform_real<> radiusDist(1.0f*WSCALE.unitsPerMeter, radius);
    //boost::variate_generator<boost::minstd_rand, boost::uniform_real<> > randRadius;

    //GPUEntsDistributor<boost::minstd_rand, boost::uniform_int<> > dist(rng, xDist, zDist);
    Vector3 pos(Ogre::Vector3::ZERO); 
    Quaternion orient;
    Vector3 offset(Ogre::Vector3::ZERO);
    cout << "Creating " << numOfEnts << " number of entities." << endl;

    for(int i=0; i<numOfEnts; ++i)
    {
        if(i == endIdx)
          {
            grpIdx++;
            endIdx = endIdx + groups[grpIdx]->numOfEnts;
            center = groups[grpIdx]->center;
            radius = groups[grpIdx]->radius*WSCALE.unitsPerMeter;
          }
        const std::vector<double> vecOnSphere = randOnCircle();
        offset.x = vecOnSphere[0] * radius;
        offset.z = vecOnSphere[1] * radius; //scale by radius
        ZEntity* ent = entMgr->createZEntity();
        //cout << "builder center: " << center << endl;
        //cout << "builder offset: " << offset << endl;
        pos = center + offset;
        //cout << "builder pos: " << pos << endl;
        ent->onRead(pos, orient, center);
    }
    
    CreateEntCmd createCmd;
    cout << "Calling executeCmd: " << endl;
    CommandController::getSingleton().executeCmd(createCmd);
    return true;
}

bool
EntitiesBuilder::unbuild(ZEntity* zEnt)
{
    
    assert(zEnt != 0 && "ZEntity is null!");
    cout << "In PlayerENtity::onDestroyClient()" << endl;
    COMMAND::StringCommand nodeRemove(CommandList::NODEREMOVE);
    return true;
}
