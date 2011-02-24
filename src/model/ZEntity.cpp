/*
* ZEntity.cpp
*
*  Created on: Sep 18, 2008
*      Author: bey0nd
*/

#include <iostream>

#include "entities/ZEntity.h"

#include "CommandController.h"
#include "command/CommandList.h"


//using namespace ZGame;
using namespace ZGame::Entities;

ZEntity::ZEntity(const ZENT_KEY &key, const Ogre::String &meshName) 
: Component(), 
_entKey(key), _resource(key, meshName), _worldPos(Ogre::Vector3(0, 0, 0)), _worldOrient(Ogre::Quaternion())
{
}

ZEntity::ZEntity(const ZEntityResource &res) 
: _entKey(res.getKey()), _resource(res)
{
}

ZEntity::~ZEntity()
{

}

/*
* Implements Component methods. This is temp. we will need some utility methods for this.
* Also may want to move the public version of this method to base class. Then have the base class
*call private virtual methods in this class.
*
*/

const fastdelegate::DelegateMemento
    ZEntity::getInputDM(const Ogre::String &memName)
{
    using fastdelegate::DelegateMemento;
    DelegateMemento dlgM;
    if(memName == "OnReadInput")
    {
        Entities::ConstEntityUpdateEvent del;
        del.bind(this, &ZEntity::onRead);
        dlgM = del.GetMemento();
    }
    else
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Invalid Input Delegate Memento name.", 
        "ZEntity::getInputDM");
    return dlgM;
}

void
    ZEntity::addOutputDM(const Ogre::String &memName, const fastdelegate::DelegateMemento dm)
{
    //WARNING: assuming passed in dm is of the type. 
    if(memName == "OnWriteOutput")
    {
        Entities::EntityUpdateEvent ue;
        ue.SetMemento(dm);
        _onWriteOutput.push_back(ue);
    }
    else if(memName == "OnEventOutput")
    {
        Entities::EntityOnEventEvent oee;
        oee.SetMemento(dm);
        _onEventOutput.push_back(oee);
    }
    else
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Invalid Output Delegate Memento name.",
        "ZEntity::addOutputDM");
}

void
    ZEntity::deleteOutputDM(const Ogre::String &memName, const fastdelegate::DelegateMemento dm)
{
    if(memName == "OnWriteOutput")
    {
        Entities::EntityUpdateEvent ue;
        ue.SetMemento(dm);
        //assuming == is overloaded to mean they point to the same function
        auto iter = std::find(_onWriteOutput.begin(), _onWriteOutput.end(), ue);
        if(iter != _onWriteOutput.end())
            _onWriteOutput.erase(iter);
    }
    else if(memName == "OnEventOutput")
    {
        Entities::EntityOnEventEvent oee;
        oee.SetMemento(dm);
        auto iter = std::find(_onEventOutput.begin(), _onEventOutput.end(), oee);
        if(iter != _onEventOutput.end())
            _onEventOutput.erase(iter);
       
    }
    else
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Invalid Output Delegate Memento name.",
        "ZEntity::deleteOutputDM");
}

const ZENT_KEY 
ZEntity::getEntityName() const
{
    return _entKey;
}

/**
*This function is called for on read events--meaning, that 
*
*/
bool
ZEntity::onRead(const Ogre::Vector3 &pos, const Ogre::Quaternion &orient) //const Ogre::Vector3 goal)
{
    _worldPos = pos;   
    _worldOrient = orient;
    for(auto iter = _onWriteOutput.begin(); iter != _onWriteOutput.end(); ++iter)
    {
        (*iter)(_worldPos, _worldOrient);
    }
    return true;
}
bool
ZEntity::onWrite(Ogre::Vector3 &pos, Ogre::Quaternion &orient)
{
    pos = _worldPos;
    orient = _worldOrient;
    return true;
}

void
    ZEntity::onEvent(unsigned int state)
{
    for(auto iter = _onEventOutput.begin(); iter != _onEventOutput.end(); ++iter)
    {
        (*iter)(state);
    }
}