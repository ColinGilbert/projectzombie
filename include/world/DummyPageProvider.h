#pragma once

#include <OgrePageManager.h>
#include <OgrePagedWorldSection.h>
#include <OgrePagedWorld.h>
#include <OgrePageManager.h>



namespace ZGame
{
    namespace World
    {
        class VolumeMap;
        class VolumePagedWorldSection;
        class DummyPageProvider : public Ogre::PageProvider
        {
            
        public:
            bool
                prepareProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section)
            {
                return true;
            }
            bool
                loadProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section)
            {
                return true;
            }
            bool
                unloadProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section)
            {
                return true;
            }
            bool
                unprepareProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section)
            {
                return true;
            }
        };
    }
}