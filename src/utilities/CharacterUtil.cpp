#include <ctime>
//#include <sstream>

#include <Ogre.h>

#include "EngineView.h"
#include "utilities/CharacterUtil.h"
#include "CommandController.h"
#include "CommandDelegates.h"

using namespace ZGame::Util;

CharacterUtil::CharacterUtil() :
_minExt(-10.0f,-1.0f,-1.0f),
_maxExt(10.0f,1.0f,1.0f),
_xDist(_minExt.x,_maxExt.x),
_zDist(_minExt.z,_maxExt.z),
_dist(_rng,_xDist,_zDist)
{
  Ogre::String CHARLIST("charlist");
  Ogre::String CHARLISTMESHES("charlistmeshes");
  Ogre::String CHARCREATE("charcreate");
  Ogre::String NODELIST("nodelist");
  _rng.seed(time(0));
  ZGame::CommandController* cmdCtrl = ZGame::CommandController::getSingletonPtr();
  COMMAND::ConsoleCommand cmd;
  cmd.bind(&ZGame::Util::CharacterUtil::list,this);
  cmdCtrl->addCommand(CHARLIST,cmd);
  cmd.bind(&ZGame::Util::CharacterUtil::listMeshes,this);
  cmdCtrl->addCommand(CHARLISTMESHES,cmd);
  cmd.bind(&ZGame::Util::CharacterUtil::create,this);
  cmdCtrl->addCommand(CHARCREATE,cmd);
  cmd.bind(&ZGame::Util::CharacterUtil::listNodes,this);
  cmdCtrl->addCommand(NODELIST,cmd);
}

void CharacterUtil::setInput()
{
}

void CharacterUtil::createCharacters()
{

}

bool CharacterUtil::list(const Ogre::StringVector &params)
{
  using namespace std;
  if(params.size() == 1)
    listAllChars();
  else
  {
    if(params[1].compare("-help") == 0)
    {
      printUsage(LIST);
    }
  }
  return 0;
}



/**
*This method will list the characters.
*
*/
bool CharacterUtil::listMeshes(const Ogre::StringVector &params)
{
  using namespace std;
  if(params.size() == 1) //command was executed with only command (no parameters beside command.)
    listAllMeshes();
  else //There are parameters passed along with the command.
  {
    if(params[1].compare("-help") == 0)
    {
      printUsage(LISTMESH);
    }
  }
  return 0;
}

/**
*This method will create the character specified in the params[1].
*usage:
*/
bool CharacterUtil::create(const Ogre::StringVector &params)
{
  using namespace std;
  if(params.size() != 2)
  {
    printUsage(CREATE);
    return false;
  }
  //now ready to create.
  bool created = createCharFromMesh(params[1]);
  if(!created)
  {
    Ogre::LogManager::getSingleton().logMessage(Ogre::LML_CRITICAL,"Something bad happened! charcreate failed!");
  }

  return created;
}

bool CharacterUtil::listNodes(const Ogre::StringVector &params)
{
  using namespace std;
  Ogre::SceneManager* scnMgr = EngineView::getSingleton().getSceneManager();

  //Ogre::SceneNode* root = scnMgr->getRootSceneNode();

  Ogre::Node* root = (Ogre::Node*)scnMgr->getRootSceneNode();
  Ogre::Log::Stream oss = Ogre::LogManager::getSingleton().stream();
  oss << "listnodes utility: \n";
  
  //oss << "Got root: " << root->

  //iterate through all scene nodes and print name.
  //Ogre::SceneNode::ObjectIterator iter(root->getAttachedObjectIterator());
  Ogre::Node::ChildNodeIterator iter(root->getChildIterator());  

  while(iter.hasMoreElements())
  {
    oss << "objmap key: " << iter.peekNextKey() << "\n";
    iter.moveNext();
  }
  return false;
  
}

void CharacterUtil::printUsage(CHAR_CMDS_ENUM CMD_ENUM)
{
  using namespace std;
  Ogre::Log::Stream oss = Ogre::LogManager::getSingleton().stream();
  oss << "Usage: " << "\n";
  switch(CMD_ENUM)
  {
  case CharacterUtil::LISTMESH:
    oss << "charlistmeshes will enumerate and list all character resources." << "\n";
    oss << "charlist [parameters] " << "\n";
    oss << "parameters: " << "\n";
    oss << "-help :- prints usage info. " << "\n";
    oss << "\n";
    break;
  case CharacterUtil::CREATE:
    oss << "charcreate will create a character based on a given mesh name. Use charlist for a list of availible meshes." << "\n";
    oss << "characters will not be created if no skeleton is assoicated with the given mesh." << "\n";
    oss << "charcreate [parameters] mesh_name" << "\n";
    oss << "parameters: " << "\n";
    oss << "mesh_name :- the mesh of the character that you want to create. " << "\n";
    break;
  case CharacterUtil::LIST:
    oss << "charlist will list characters." << "\n";
    oss << "charlist [parameters] char_name." << "\n";
    oss << "parameters: " << "\n";
    oss << "char_name :- the name of the specific character to list. If none is given then all characters are listed." << "\n";
  default:
    break;
  }
}




void CharacterUtil::listAllChars()
{
  using namespace std;
  Ogre::Log::Stream oss = Ogre::LogManager::getSingleton().stream();
  oss << "Character Utility character listings: " << "\n";
  oss << "--------------------------------------" << "\n";
  oss << "\n";
  if(_charNodes.size() < 1)
  {
    oss << "No characters were found. " << "\n";
  
  }
  else
  {
    //loop through the character node storage and list each one.
    CHAR_ITR itr;
    Ogre::SceneNode* scnNode;
    for(itr=_charNodes.begin();itr != _charNodes.end(); itr++)
    {
      scnNode = itr->second;
      oss << "key: " << itr->first << "\n";
      oss << "name: " << scnNode->getName();
      oss << "position: " << scnNode->getPosition() << "\n";
    }
  }
}

void CharacterUtil::listAllMeshes()
{
  using namespace std;
  //print to logger.
  Ogre::Log::Stream oss = Ogre::LogManager::getSingleton().stream();
  Ogre::MeshManager* meshMan = Ogre::MeshManager::getSingletonPtr();
  oss << "Character Utility mesh listings: " << "\n";
  oss << "--------------------------------------" << "\n";
  oss << "\n";
  //print some resource group
  Ogre::StringVector resGrpList = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();
  oss << "Resource Groups and resources: " << "\n";
  for(size_t i=0; i < resGrpList.size(); i++)
  {
    oss << resGrpList[i] << "\n";
  }

  oss << "Resource Group: ZGAME_CHARACTER: " << "\n";
  Ogre::StringVectorPtr resInGrpList = Ogre::ResourceGroupManager::getSingleton().listResourceNames("ZGAME_CHARACTER");
  for(size_t i=0; i < resInGrpList->size(); i++)
  {
    oss << "Name: " << resInGrpList->at(i) << "\n";
  }

  if(Ogre::ResourceGroupManager::getSingleton().isResourceGroupLoaded("ZGAME_CHARACTER"))
  {
    oss << "ZGAME_CHARACTER is loaded" << "\n";
  }
  else
  {
    oss << "ZGAME_CHARACTER is not loaded" << "\n";
  }
  //automatically load in all the meshes from ZGAME_CHARACTER: It is a hack for now. We need some other way of loading these meshes in.
  Ogre::ResourceGroupManager::getSingleton().loadResourceGroup("ZGAME_CHARACTER");
  if(Ogre::ResourceGroupManager::getSingleton().isResourceGroupLoaded("ZGAME_CHARACTER"))
  {
    oss << "ZGAME_CHARACTER is loaded" << "\n";
  }
  else
  {
    oss << "ZGAME_CHARACTER is not loaded" << "\n";
  }

  resInGrpList = Ogre::ResourceGroupManager::getSingleton().listResourceNames("ZGAME_CHARACTER");
  
  for(size_t i=0; i < resInGrpList->size(); i++)
  {
    oss << "Adding resource to MeshManager. " << "\n";
    oss << "Name: " << resInGrpList->at(i) << "\n";
    try
    {
      meshMan->load(resInGrpList->at(i),"ZGAME_CHARACTER");
    }catch(Ogre::InternalErrorException exp)
    {
      //do nothing. This happens because ZGAME_CHARACTER resource group contain skeletons, and we are trying to load these skeleton with
      //the mesh manager. Obviously this is not going to work. This is a temp. work around so we can finish initial implementation with
      //CharacterUtil.
    }
    
  }
  oss << "Currently availible resource in MeshManager: " << "\n";
 
  Ogre::ResourceManager::ResourceMapIterator resMapIt = meshMan->getResourceIterator();
  
  while(resMapIt.hasMoreElements())
  {
    cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << "\n";
    cout << "resource map key: " << resMapIt.peekNextKey() << "\n";
    Ogre::ResourcePtr res = resMapIt.getNext();
    oss << "++++++++++++++++++++++++++++++" << "\n";
    oss <<  res->getName()<< "\n";
    oss << "handle: " << res->getHandle() << "\n";
    oss << "isLoaded: " << res->isLoaded() << "\n";
    oss << "group: " << res->getGroup() << "\n";
    oss << "++++++++++++++++++++++++++++++" << "\n";
    oss << "\n";
  }
}

bool CharacterUtil::createCharFromMesh(const Ogre::String &meshName)
{
  using namespace std;
  Ogre::Log::Stream oss = Ogre::LogManager::getSingleton().stream();
  bool created = true;
  Ogre::MeshManager* meshMan = Ogre::MeshManager::getSingletonPtr();
  //First let's see if the mesh exists.
  /*
  Ogre::MeshPtr theMesh = meshMan->getByName(meshName);
  if(!theMesh.isNull())
  {*/
  //If so, let's check if skeleton is attached to this mesh.
    //if(theMesh->hasSkeleton())
    //{
      //Now everything is good to go. Create the scene nodes and attach mesh to it.
      oss << "Creating character: " << meshName << "\n";
      Ogre::SceneManager* scnMgr = EngineView::getSingleton().getSceneManager();
      Ogre::String nodeName = meshName+"NODE";
      Ogre::String entName = "";
      Ogre::SceneNode* root = scnMgr->getRootSceneNode();
      //Ogre::SceneNode* node = root->createChildSceneNode(nodeName);
      Ogre::SceneNode* node = root->createChildSceneNode();
      oss << "Created child node in root called: " << node->getName() << "\n";
      entName = meshName + node->getName();
      Ogre::Entity* entity = scnMgr->createEntity(entName,meshName);
      //entity->setDisplaySkeleton(true);
      oss << "Created entity: " << entName << "\n";
      //entity->scale(0.0078125,0.0078125,0.0078125);
      node->attachObject(entity);
      //node->setInheritScale(true);
      node->setScale(0.01,0.01,0.01);
      Ogre::Vector3 pos;
      _dist.nextPosition(pos);
      oss << "Translating to position: " << pos << "\n";
      //node->translate(pos);



      /*     
      _dist.nextPosition(pos);
      oss << "Translating to position: " << pos << "\n";
      _dist.nextPosition(pos);
      oss << "Translating to position: " << pos << "\n";
      _dist.nextPosition(pos);
      oss << "Translating to position: " << pos << "\n";
      */

    //}
      /*
    else
    {
      oss << "No skeleton is associated with the specified mesh, cannot continue!" << "\n";
      created = false;
    }*/
  /*
  }
  else{
    oss << "No mesh with the specified name is found within MeshManager, cannot continue!" << "\n";
    created = false;
  }*/
  //If not, then return not created.
  return created;
}


