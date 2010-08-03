#pragma once
#include <iostream>
#include <sstream>
#include <exception>
#include <vector>
#include <vector>
#include <string>
#include <Ogre.h>
#include "utilities/cl.hpp"

namespace ZGame
{
    namespace Entities
    {
        struct ZEntityBuffers;
    }
    namespace World
    {
        class WorldMap;
    }
    namespace ZCL
    {
        /**
        *This class implements the Project Zombie OpenCL features.
        *
        */
        class ZCLController
        {
        public:
            ZCLController();
            virtual ~ZCLController();
            /** \brief This method will initialize the OpenCL controller.**/
            void
                init(std::string configName, Entities::ZEntityBuffers* entBufs, World::WorldMap* worldMap);
            bool onUpdate(const Ogre::FrameEvent &evt);
            bool onUpdate();
            bool onInit(void){ return true;}
            bool onDestroy(void){ return true;}

        private:
            /** \brief This helper method will initialize the OpenCL context.*/
            void initCL(std::string& configName);
            /** \brief This helper method will initialize the OpenCL buffers. **/
            void initCLBuffers(Entities::ZEntityBuffers* entBufs, World::WorldMap* worldMap);
            /** \brief This method initializes the arguments for OpenCL kernels.*/
            void initArgs();
            /** \brief Utility function to convert a text file to string. Take directly from AMD convolution OpenCL example.**/
            /** \brief This utility method will output device information. */
            void printDeviceInfo(std::vector<cl::Device> &devs);
            std::string
                FileToString(const std::string fileName);
            inline void
                _chkErr(cl_int err, const char* name)
            {
                if(err != CL_SUCCESS)
                {
                    std::stringstream except;
                    except << "ERROR: " << name << " (" << err << ")";
                    //std::cerr << "ERROR: " << name << " (" << err << ")" << std::endl;
                    throw(std::exception(except.str().c_str()));
                }
            }
            
        private:
            cl::Context _context;
            std::vector<cl::Device> _devices;
            cl::CommandQueue _queue;
            cl::Program _program;
            std::vector<cl::Kernel> _kernel;
            cl::Buffer _entsPosCL;
            cl::Buffer _entsOrientCL;
            cl::Buffer _entsModeCL;
            cl::Buffer _gradCL;
            cl::Buffer _contourCL;
            size_t _entsDim; //The dimension of vector per entity. This is used as stride into flat 1D buffer.
            size_t _numOfEnts; 
            size_t _entsBufLen;
            Ogre::Real* _entsPosBuf;
            Ogre::Real* _entsOrientBuf;
            Ogre::uchar* _entsModeBuf;
            std::vector<size_t> _mapShape;
            size_t _mapBufLen;
            Ogre::Real* _gradMap;
            Ogre::Real* _contourMap;
            
                    
        };
    }
}
