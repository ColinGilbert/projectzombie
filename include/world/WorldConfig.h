#pragma once
/**
* \file This file contains an implementation for configuring the world.
*
*/
#include <sstream>
#include <Ogre.h>

namespace ZGame
{
    namespace World
    {
        struct WorldMapConfig
        {
            float loadRadius;
            float unloadRadius;
            Ogre::uint32 minx;
            Ogre::uint32 miny;
            Ogre::uint32 maxx;
            Ogre::uint32 maxy;
            bool forceSync;
        };
        class WorldConfig
        {
        public:
            WorldConfig(){}
            ~WorldConfig(){}

            WorldMapConfig& getWorldMapConfig()
            {
                return _worldMapConfig;
            }

            void load()
            {
                using namespace Ogre;
                Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL, "------------------------------------");
                Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL, "Loading World Configuration");
                Ogre::Log::Stream logs = Ogre::LogManager::getSingleton().getDefaultLog()->stream(Ogre::LML_TRIVIAL);
               
                ConfigFile cf;
                cf.load("world.cfg");
                ConfigFile::SectionIterator seci = cf.getSectionIterator();
                String secName, typeName, archName;
                std::istringstream instream;

                while(seci.hasMoreElements())
                {
                    secName = seci.peekNextKey();
                    ConfigFile::SettingsMultiMap* settings = seci.getNext();
                    ConfigFile::SettingsMultiMap::iterator i;
                    for(i = settings->begin(); i != settings->end(); ++i)
                    {
                        typeName = i->first;
                        archName = i->second;
                        instream.clear();
                        instream.str(archName);
                        if(typeName.compare("LOADRADIUS") == 0)
                        {
                            instream >> _worldMapConfig.loadRadius;
                        }
                        else if(typeName.compare("UNLOADRADIUS") == 0)
                        {
                            instream >> _worldMapConfig.unloadRadius;
                        }
                        else if(typeName.compare("MINX") == 0)
                        {
                            instream >> _worldMapConfig.minx;
                        }
                        else if(typeName.compare("MINY") == 0)
                        {
                            instream >> _worldMapConfig.miny;
                        }
                        else if(typeName.compare("MAXX") == 0)
                        {
                            instream >> _worldMapConfig.maxx;
                        }
                        else if(typeName.compare("MAXY") == 0)
                        {
                            instream >> _worldMapConfig.maxy;
                        }
                        else if(typeName.compare("FORCESYNC") == 0)
                        {
                            instream >> _worldMapConfig.forceSync;
                        }
                       
                    }
                }

            }
        private:
            WorldMapConfig _worldMapConfig;
            
        };
        
    }
}