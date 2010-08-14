"""
This file is the top level Scons file.
Note: We're not using any advanced features of Scons (e.g. autoconf like features) because we're learning and we are simply trying to
get something working on Linux.
"""

import os

#platform = ARGUMENTS.get('OS', Platform())

print "Platform is: %s", "$PLATFORM"

OGRE_PATH = "$HOME/projects/ogre_new/ogre_src_v1-7-1/"
#directories
OGRE_INCLUDE = OGRE_PATH + "OgreMain/include/" #note, manually entering directory names for now. 
includes = OGRE_INCLUDE + " /usr/include/" #/usr/include is for boost and stuff. you have all that.
libpath = OGRE_PATH + "build/lib /usr/lib/ /usr/local/lib/"
libs = "OgreMain OIS projectz" 

env = Environment(INCDIR = Split(includes),
                  LIBDIR = Split(libpath),
                  CPPPATH = Split(includes),
                  LIBPATH = Split(libpath),
                  LIBS = Split(libs))

Export('env')


