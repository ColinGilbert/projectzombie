"""
This file is the top level Scons file.
Note: We're not using any advanced features of Scons (e.g. autoconf like features) because we're learning and we are simply trying to
get something working on Linux.
"""

#NOTE: Please change all the directory strings into a LIST PLEASE!!!

import os

HOME = os.environ['HOME']
ATISTREAMSDKROOT = os.environ['ATISTREAMSDKROOT']
#ATISTREAMSDKROOT = HOME
PRJZ_HOME = HOME+"/projects/prjz_new/projectzombie/"
OGRE_INCLUDE = "/usr/local/include/OGRE/ /usr/local/include/OGRE/Terrain/ /usr/local/include/OGRE/Paging /usr/include/OIS/"
TBB_INCLUDE = "/home/beyzend/software/tbb30_20100915oss/include"
OGREBULLET_INCLUDE = "/usr/local/include/OgreBullet/Collisions /usr/local/include/OgreBullet/Dynamics/"
BULLET_INCLUDE="/usr/local/include/bullet"
NOISEPP_INCLUDE="/home/beyzend/projects/noisepp-0.3/noisepp/core/ /home/beyzend/projects/noisepp-0.3/noisepp/threadpp/ /home/beyzend/projects/noisepp-0.3/noisepp/utils"
RAKNET_HOME = HOME+"/software/raknet_new/"
RAKNET_PATH = RAKNET_HOME
includes = OGRE_INCLUDE + " /usr/include/ /usr/include/c++/4.4/ /usr/local/include/ "+ATISTREAMSDKROOT+"/include/ "+PRJZ_HOME+"include/ "+RAKNET_HOME+"Source" 
includes += " /usr/local/include/PolyVoxCore/ /usr/local/include/PolyVoxUtil/"
includes += " "+TBB_INCLUDE
includes += " "+OGREBULLET_INCLUDE
includes += " "+BULLET_INCLUDE
includes += " "+NOISEPP_INCLUDE

OGRE_LIB = "/usr/local/lib/OGRE/"
TBB_LIB ="/home/beyzend/software/tbbrelease"
NOISEPP_LIB = "/home/beyzend/projects/noisepp-0.3/build/lib/Release"

#libpath = OGRE_LIB + " "+ATISTREAMSDKROOT+"/lib/x86/" + " /usr/lib/ /usr/local/lib/ "
libpath = OGRE_LIB + " "+ATISTREAMSDKROOT+"/lib/x86/" + " /usr/local/lib/"
libpath += " " + TBB_LIB
libpath += " " + NOISEPP_LIB

libs = "OgreMain OgreTerrain OgrePaging OgreRTShaderSystem OIS OpenCL PolyVoxCore PolyVoxUtil tbb tbbmalloc RocketCore RocketControls RocketDebugger OgreBulletCol OgreBulletDyn \
OgreProcedural noisepp skyx" 

env = Environment()

env.Decider('MD5-timestamp')
env.Append( RAKNET_PATH = RAKNET_PATH)
env.Append( INCDIR = Split(includes))
env.Append( LIBDIR = Split(libpath))
env.Append( CPPPATH = Split(includes))
env.Append( LIBPATH = Split(libpath))
env.Append( LIBS = Split(libs))
#env.Append( CPPFLAGS = ['-ggdb'])
env.Append( CPPFLAGS = ['-DNDEBUG','-O3','-std=c++0x','-enable-checking=none' ]) #build with optimization level 2 AND output debug info.
#env.Append( CCFLAGS = [])
print env['CXXCOM'] + "\n"
print env['_CCCOMCOM'] + "\n"
env['CXXCOM'] = "$CXX $_CCCOMCOM -o $TARGET -c $CXXFLAGS $CCFLAGS $SOURCES"


env.SConscript('src/SConscript', exports='env', variant_dir='build/', src_dir='src/', duplicate=0)
