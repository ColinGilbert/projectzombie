/*
 * ZEntity.h
 *
 *  Created on: Sep 18, 2008
 *      Author: bey0nd
 */

#ifndef ZENTITY_H_
#define ZENTITY_H_

//#include <string>

#include <Ogre.h>

//#include <OpenSteer/SimpleVehicle.h>

#include "entities/EntitiesDefs.h"
#include "entities/ZEntityResource.h"
#include "entities/Component.h"
#include "delegates/EntityDelegates.h"

namespace ZGame
{
  namespace Entities
  {
    /**
    * This class represents a ZEntity component. Every entity in the system shall have this component.
    *
    **/
      class ZEntity : public Component
    {
    public:
      ZEntity(const ZENT_KEY &key, const Ogre::String &meshName);
      ZEntity(const ZEntityResource &res);
      virtual
      ~ZEntity();

      virtual const fastdelegate::DelegateMemento
          getInputDM(const Ogre::String &memName);

      virtual void
          addOutputDM(const Ogre::String &memName, const fastdelegate::DelegateMemento dm);

      virtual void
          deleteOutputDM(const Ogre::String &memName, const fastdelegate::DelegateMemento dm);


      const ZENT_KEY getEntityName() const;
      //void setEntityName(const Ogre::String &entName);
      //const ZENT_KEY getEntityName() const
      //{
        //return _entKey;
      //}
      const ZENT_KEY
      getGroupName() const
      {
        //return _groupKey;
      }

      ZEntityResource const* const
      getResource() const
      {
        return &_resource;
      }

      bool
          onRead(const Ogre::Vector3 &pos, const Ogre::Quaternion &orient);

      bool
      onWrite(Ogre::Vector3 &pos, Ogre::Quaternion &orient);

      const Ogre::Vector3&
      getPosition() const
      {
        return _worldPos;
      }
      const Ogre::Quaternion&
      getOrientation() const
      {
        return _worldOrient;
      }
      int
      getGroupId()
      {
        //return _groupId;
      }

      void
          onEvent(unsigned int state);


    protected:
    private:
      ZENT_KEY _entKey;
      ZEntityResource _resource;
      Ogre::Vector3 _worldPos;
      Ogre::Quaternion _worldOrient;

      typedef Ogre::vector<Entities::EntityUpdateEvent>::type ONWRITE_OUTPUT;
      ONWRITE_OUTPUT _onWriteOutput;

      typedef Ogre::vector<Entities::EntityOnEventEvent>::type ONEVENT_OUTPUT;
      ONEVENT_OUTPUT _onEventOutput;


    };
  }
}

#endif /* ZENTITY_H_ */
