#include <OgreRoot.h>
#include "entities/InstancedRdrEntitiesBuilder.h"

//#define CROWD 1
//#define INSTANCING 1


using namespace Ogre;
using namespace ZGame::Entities;
#ifdef CROWD
const size_t InstancedRdrEntitiesBuilder::MAX_PER_BATCH = 4;
#else 
const size_t InstancedRdrEntitiesBuilder::MAX_PER_BATCH = 80;
#endif
InstancedRdrEntitiesBuilder::InstancedRdrEntitiesBuilder(SceneManager* scnMgr) : _scnMgr(scnMgr)
{
}

InstancedRdrEntitiesBuilder::~InstancedRdrEntitiesBuilder()
{
}

void
    InstancedRdrEntitiesBuilder::build(size_t numOfMeshes, std::vector<Ogre::InstancedGeometry*> &renderInstance, std::string meshName)
{

    size_t objectCount = numOfMeshes;
    size_t numRendered = 1;
    _chkVertexProgram();

    while(objectCount > MAX_PER_BATCH)
    {
        numRendered++;
        objectCount-=MAX_PER_BATCH;
    }

    MeshPtr m = MeshManager::getSingleton().getByName(meshName + ".mesh");
    if(m.isNull())
    {
        m = MeshManager::getSingleton().load(meshName + ".mesh",
            ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
    }
    const Real radius = m->getBoundingSphereRadius();
    std::vector<Vector3*> posMat;
    posMat.resize(numRendered);
    posMat.reserve(numRendered);

    Entity* ent = _scnMgr->createEntity(meshName, meshName+".mesh");

    //InstancedGeometry* batch = new InstancedGeometry(_scnMgr, meshName + "s");
    InstancedGeometry* batch = _scnMgr->createInstancedGeometry(meshName + "s");
    //ent->setMaterialName("Ogre/DepthShadowmap/Receiver/Robot");
    batch->setCastShadows(true);
    

    batch->setBatchInstanceDimensions(Vector3(100000, 100000, 100000));
    const size_t batchSize = (numOfMeshes > MAX_PER_BATCH) ? MAX_PER_BATCH : numOfMeshes;
    setupInstancedMaterialToEntity(ent);

    for(size_t i = 0; i < batchSize; ++i)
    {
        //batch->add
        batch->addEntity(ent, Vector3::ZERO);
    }
    batch->setOrigin(Vector3::ZERO);
    
    batch->build();

    //Add the batch instances. We are going to set the actual positions for batch instances elsewhere.
    for(size_t k = 0; k < numRendered-1; ++k)
    {
        batch->addBatchInstance();
    }
    batch->setVisible(true);
    renderInstance.push_back(batch);

    //destroy the entity. We no longer need it. It's been "instanced" or cloned now.
    //_scnMgr->destroyEntity(ent);
}

/**
*This method will check for vertex program capabilities in order to allow instancing batching. As this features requires vertex program since it does transformations there.
**/
void
    InstancedRdrEntitiesBuilder::_chkVertexProgram()
{
    if(Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(RSC_VERTEX_PROGRAM) == false)
    {
        OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Your video card doesn't support batching", "InstancedRdrEntitiesBuilder::build");
    }
}

void 
    InstancedRdrEntitiesBuilder::setupInstancedMaterialToEntity(Entity*ent)
{
    for (Ogre::uint i = 0; i < ent->getNumSubEntities(); ++i)
    {
        SubEntity* se = ent->getSubEntity(i);
        String materialName= se->getMaterialName();
        se->setMaterialName(buildInstancedMaterial(materialName));
    }
}
Ogre::String 
    InstancedRdrEntitiesBuilder::buildInstancedMaterial(const String &originalMaterialName)
{

    // already instanced ?
    if (StringUtil::endsWith (originalMaterialName, "/instanced"))
        return originalMaterialName;

    MaterialPtr originalMaterial = MaterialManager::getSingleton ().getByName (originalMaterialName);

    // if originalMat doesn't exists use "Instancing" material name
    const String instancedMaterialName (originalMaterial.isNull() ? "Instancing" : originalMaterialName + "/Instanced");
    MaterialPtr  instancedMaterial = MaterialManager::getSingleton ().getByName (instancedMaterialName);

    // already exists ?
    if (instancedMaterial.isNull())
    {
        instancedMaterial = originalMaterial->clone(instancedMaterialName);
        instancedMaterial->load();
        Technique::PassIterator pIt = instancedMaterial->getBestTechnique ()->getPassIterator();
        while (pIt.hasMoreElements())
        {

            Pass * const p = pIt.getNext();
#ifdef CROWD
            p->setVertexProgram("Crowd", true);
            p->setShadowCasterVertexProgram("CrowdShadowCaster");
#else
            p->setVertexProgram("Instancing", false);
            p->setShadowCasterVertexProgram("InstancingShadowCaster");
#endif
            //p->getVertexProgramParameters("");

        }
    }
    instancedMaterial->load();
    return instancedMaterialName;


}