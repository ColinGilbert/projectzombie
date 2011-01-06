/**
*Copyright (c) 2010 Fdastero LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
**/

#pragma once

#include "ZPrerequisites.h"
#include "ZException.h" 
using ZGame::Exception;
#include "utilities/cl.hpp"
#include "utilities/Timer.h"

namespace ZGame
{
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
                init(std::string configName);
            void prepare(Entities::ZEntityBuffers* entBufs, World::WorldMap* worldMap);
            bool onUpdate(const Ogre::FrameEvent &evt);
            bool onUpdate();
            bool onInit(ZGame::ZInitPacket *packet){ return true;}
            bool onDestroy(void){ return true;}
            void printKernelTime();
            /** \brief This method will get the position and orientation buffers.**/
            void getBuffers(const float* &posBuf, const float* &orientBuf, const float* &velocityBuf)
            {
                //NOTE!!! We assume that both buffers are valid when you call this method.
                posBuf = _entsPosBuf;
                orientBuf = _entsOrientBuf;
                velocityBuf = _entsVelBuf;
            }


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
            /** \brief This method will enqueue kernels to the command queue.*/
            void enqueueKernel0(bool blocking);
            void enqueueKernel1(bool blocking);
            /** \brief This method will read back the buffers.**/
            //void readBackBuffers();
            
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
                    throw(Exception(except.str().c_str()));
                }
            }
            
            void loadKernel(std::vector<cl::Kernel> &kernel, std::string&, std::string);

        private:
            cl::Context _context;
            std::vector<cl::Device> _devices;
            cl::CommandQueue _queue;
            cl::Program _program;
            std::vector<cl::Kernel> _kernel;
            cl::Buffer _entsPosCL;
            cl::Buffer _entsOrientCL;
            cl::Buffer _entsVelCL;
            cl::Buffer _entsModeCL;
            cl::Buffer _gradCL;
            cl::Buffer _contourCL;
            cl::Buffer _entsGoalsCL;
            cl::Buffer _entsStoreOneCL;
            cl::Buffer _densityCL;
            size_t _entsDim; //The dimension of vector per entity. This is used as stride into flat 1D buffer.
            size_t _numOfEnts; 
            size_t _entsBufLen;
            Ogre::Real* _entsPosBuf;
            Ogre::Real* _entsOrientBuf;
            Ogre::Real* _entsVelBuf;
            Ogre::uchar* _entsModeBuf;
            Ogre::Real* _entsGoalsBuf;
            Ogre::Real* _entsStoreOneBuf;
            Ogre::Real* _densityBuf;
            std::vector<size_t> _mapShape;
            size_t _mapBufLen;
            Ogre::Real* _gradMap;
            Ogre::Real* _contourMap;
            
            size_t _iterations; //number of iterations to run kernel.
            double _deviceKernelTime; //variable to track performance time for device kernels.
            CPerfCounter _counter; //performance counter.
            size_t _argI;
            bool _useGPU;
            size_t _iterCount;
            long _loopI;
                    
        };
    }
}
