"""
This file is the top level Scons file.
Note: We're not using any advanced features of Scons (e.g. autoconf like features) because we're learning and we are simply trying to
get something working on Linux.
"""

import os

HOME = os.environ['HOME']
ATISTREAMSDKROOT = os.environ['ATISTREAMSDKROOT']
#ATISTREAMSDKROOT = HOME
PRJZ_HOME = HOME+"/projects/prjz_new/projectzombie/"
OGRE_INCLUDE = "/usr/local/include/OGRE/ /usr/local/include/OGRE/Terrain/"
RAKNET_HOME = HOME+"/software/raknet_new/"
RAKNET_PATH = RAKNET_HOME
includes = OGRE_INCLUDE + " /usr/include/ /usr/local/include/ "+ATISTREAMSDKROOT+"/include/ "+PRJZ_HOME+"include/" + " /usr/include/CL/ "+RAKNET_HOME+"Source"
OGRE_LIB = "/usr/local/lib/OGRE/"
libpath = OGRE_LIB + " /usr/lib/ /usr/local/lib/ "+ATISTREAMSDKROOT+"/lib/x86/" 
libs = "OgreMain OgreTerrain OgrePaging OgreRTShaderSystem OIS OpenCL" 

env = Environment()

env.Decider('MD5-timestamp')
env.Append( RAKNET_PATH = RAKNET_PATH)
env.Append( INCDIR = Split(includes))
env.Append( LIBDIR = Split(libpath))
env.Append( CPPPATH = Split(includes))
env.Append( LIBPATH = Split(libpath))
env.Append( LIBS = Split(libs))
env.Append( CPPFLAGS = ['-o2'])
env['CXXCOM'] = "$CXX $_CCCOMCOM -o $TARGET -c $CXXFLAGS $CCFLAGS $SOURCES"



env.SConscript('src/SConscript', exports='env', variant_dir='build/', src_dir='src/', duplicate=0)
