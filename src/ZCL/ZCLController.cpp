#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdio>

#define GROUP_SIZE 80

using std::ifstream;
using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::cerr;

//#define MAX_OBJ_PER_BATCH 4

#include "ZCL/ZCLController.h"
#include "entities/EntitiesManager.h"
#include "world/WorldMap.h"
#include "world/WorldScale.h"
#include "ZInitPacket.h"

using namespace ZGame;

using ZGame::ZCL::ZCLController;
//using ZGame::Entities::ZEntityBuffers;
using ZGame::World::WorldMap;

using namespace ZGame::ZCL;

extern ZGame::World::WorldScale WSCALE;

ZCLController::ZCLController() :
  _entsDim(0), _numOfEnts(0), _entsBufLen(0), _entsPosBuf(0), _entsOrientBuf(0), _entsModeBuf(0), _mapBufLen(0), _gradMap(0), _contourMap(0),
      _iterations(1000), _deviceKernelTime(0.0), _argI(0), _useGPU(false), _iterCount(0), _loopI(0)
{
}

ZCLController::~ZCLController()
{
  cout << "In ZCLController destructor." << endl;
}

/*
 * Converts the contents of a file into a string
 */
string
ZCLController::FileToString(const string fileName)
{
  ifstream f(fileName.c_str(), ifstream::in | ifstream::binary);

  try
    {
      size_t size;
      char* str;
      string s;

      if (f.is_open())
        {
          size_t fileSize;
          f.seekg(0, ifstream::end);
          size = fileSize = f.tellg();
          f.seekg(0, ifstream::beg);

          str = new char[size + 1];
          if (!str)
            throw(Exception("Could not allocate memory"));

          f.read(str, fileSize);
          f.close();
          str[size] = '\0';

          s = str;
          delete[] str;
          return s;
        }
    }
  catch (std::exception except)
    {
      if (f.is_open())
        f.close();
      throw except;
    }
  return 0;
}

/**
 *This method will initialize the ZCL controller system. The method will initialze OpenCL. It will setup buffers and arguments to the kernels. The CL buffers are
 *set using USE_MEM_PTR. We assume that these buffers are managed by other systems.
 *
 *\param configName the file name to the configuration file.
 *
 *\note Currently loading parameters from configuration file is not implemented. The file name specifies the kernel file to be loaded.
 */
void
ZCLController::init(std::string configName)
{
  initCL(configName);
  //Call init buffers with entManager data and world controller.
}

void
ZCLController::prepare(Entities::ZEntityBuffers* entBufs, World::WorldMap* worldMap)
{
  initCLBuffers(entBufs, worldMap);
  initArgs();
}

void
ZCLController::initCL(std::string& configName)
{
  cl_int err;
  cout << "Initializing OpenCL" << endl;
  std::vector<cl::Platform> allPlatforms;
  std::vector<cl::Platform>::iterator iter;
  cl::Platform targetPlatform;

  cl::Platform::get(&allPlatforms);
  if (!(allPlatforms.size() > 0))
    throw(Exception("ZCLController::init() failed: No platforms found!"));
  int platformIdx = 0;
  std::string platformVendor;
  cout << "number of platforms: " << allPlatforms.size() << endl;
  for (int i = 0; i < allPlatforms.size(); i++)
    {
      allPlatforms[i].getInfo((cl_platform_info) CL_PLATFORM_VENDOR, &platformVendor);
      cout << "Platform: " << platformVendor << endl;
      if (platformVendor.compare("Advanced Micro Devices, Inc.") == 0 && !_useGPU)
        {
          platformIdx = i;
	  break;
        }
      else
        platformIdx = i;
    }
  allPlatforms[platformIdx].getInfo((cl_platform_info) CL_PLATFORM_VENDOR, &platformVendor);
  cout << "Using platform vendor: " << platformVendor << endl;
  targetPlatform = allPlatforms[platformIdx];

  cout << "Creating OpenCL GPU context." << endl;
  cl_context_properties cprops[3] =
    { CL_CONTEXT_PLATFORM, (cl_context_properties) targetPlatform(), 0 };

  //We're going to check out GPU devices.
  size_t dId = 0;
  if (_useGPU)
    {
      _context = cl::Context(CL_DEVICE_TYPE_GPU, cprops, 0, 0, &err);
      dId = 0;
    }
  else
    {
      _context = cl::Context(CL_DEVICE_TYPE_CPU, cprops, 0, 0, &err);
      dId = 0;
    }
  _chkErr(err, "Context::Context() for GPU devices.");
  _devices = _context.getInfo<CL_CONTEXT_DEVICES> ();
  printDeviceInfo(_devices);

  cout << "Loading Kernels." << endl;
  cout << "Kernel 0" << endl;
  loadKernel(_kernel, configName, "updateEnt");
  cout << "Kernel 1" << endl;
  configName = std::string("../scripts/density_kernel.cl");
  loadKernel(_kernel, configName, "updateDensity");
  cout << "Kernels loaded." << endl;

  cout << "Creating command queue" << endl;
  _queue = cl::CommandQueue(_context, _devices[dId]);
}

void
ZCLController::loadKernel(std::vector<cl::Kernel> &kernel, std::string &configName, std::string methodName)
{
  cl_int err;
  string sourceStr = FileToString(configName);
  cl::Program::Sources sources(1, std::make_pair(sourceStr.c_str(), sourceStr.length()));
  _program = cl::Program(_context, sources);
  err = _program.build(_devices, "");
  //_chkErr(err, "Program::build");
  try
    {
      _chkErr(err, "Program::Build");
      //query build info.
      std::string buildLog = _program.getBuildInfo<CL_PROGRAM_BUILD_LOG> (_devices[0]);
      cout << "Build Log: " << endl;
      cout << buildLog << endl;
    }
  catch (std::exception e)
    {
      //query build info.
      std::string buildLog = _program.getBuildInfo<CL_PROGRAM_BUILD_LOG> (_devices[0]);
      cout << "Build Log: " << endl;
      cout << buildLog << endl;
      throw e;
    }

  cl::Kernel tkernel = cl::Kernel(_program, methodName.c_str(), &err);
  _chkErr(err, "Kernel::Kernel");
  kernel.push_back(tkernel);
}

void
ZCLController::printDeviceInfo(std::vector<cl::Device> &devs)
{
  for (std::vector<cl::Device>::iterator iter = devs.begin(); iter != devs.end(); ++iter)
    {
      std::vector<size_t> max_work_items_sizes = iter->getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES> ();
      std::string device_name = iter->getInfo<CL_DEVICE_NAME> ();
      std::string device_vendor = iter->getInfo<CL_DEVICE_VENDOR> ();
      std::string device_profile = iter->getInfo<CL_DEVICE_PROFILE> ();
      std::string device_version = iter->getInfo<CL_DEVICE_VERSION> ();
      std::string driver_version = iter->getInfo<CL_DRIVER_VERSION> ();
      std::string extensions = iter->getInfo<CL_DEVICE_EXTENSIONS> ();

      int dim = 0;
      for (std::vector<size_t>::iterator jter = max_work_items_sizes.begin(); jter != max_work_items_sizes.end(); ++jter)
        {
          cout << "CL_DEVICE_MAX_WORK_ITEM_SIZES, dimension(" << dim++ << "): " << *jter << endl;
        }
      cout << "Device name: " << device_name << endl;
      cout << "Device vendor: " << device_vendor << endl;
      cout << "device profile: " << device_profile << endl;
      cout << "device_version: " << device_version << endl;
      cout << "driver_version: " << driver_version << endl;
      cout << "extension string: " << extensions << endl;

      //device

      cl_uint max_compute_units = iter->getInfo<CL_DEVICE_MAX_COMPUTE_UNITS> ();
      cl_uint max_work_item_dim = iter->getInfo<CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS> ();
      cl_uint max_work_group_size = iter->getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE> ();
      cl_uint preferred_vec_width_char = iter->getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR> ();
      cl_uint preferred_vec_width_int = iter->getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT> ();
      cl_uint preferred_vec_width_float = iter->getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT> ();

      cout << "Max compute units: " << max_compute_units << endl;
      cout << "Max work item dimensions: " << max_work_item_dim << endl;
      cout << "Max work group size: " << max_work_group_size << endl;
      cout << "preferred vector width char: " << preferred_vec_width_char << endl;
      cout << "preferred vector width int: " << preferred_vec_width_int << endl;
      cout << "preferred vector width float: " << preferred_vec_width_float << endl;

      cl_uint max_clock_freq = iter->getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY> ();
      cl_uint max_read_image_args = iter->getInfo<CL_DEVICE_MAX_READ_IMAGE_ARGS> ();
      cl_uint max_write_image_args = iter->getInfo<CL_DEVICE_MAX_WRITE_IMAGE_ARGS> ();
      cl_ulong max_mem_alloc_size = iter->getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE> ();
      size_t max_image2d_width = iter->getInfo<CL_DEVICE_IMAGE2D_MAX_WIDTH> ();
      size_t max_image2d_height = iter->getInfo<CL_DEVICE_IMAGE2D_MAX_HEIGHT> ();
      size_t max_image3d_width = iter->getInfo<CL_DEVICE_IMAGE3D_MAX_WIDTH> ();
      size_t max_image3d_height = iter->getInfo<CL_DEVICE_IMAGE3D_MAX_HEIGHT> ();
      size_t max_image3d_depth = iter->getInfo<CL_DEVICE_IMAGE3D_MAX_DEPTH> ();
      cl_uint image_support = iter->getInfo<CL_DEVICE_IMAGE_SUPPORT> ();

      cout << "Max clock frequency: " << max_clock_freq << endl;
      cout << "Max read image arguments: " << max_read_image_args << endl;
      cout << "Max write image arguments: " << max_write_image_args << endl;
      cout << "Max memory allocation size: " << max_mem_alloc_size << endl;
      cout << "Max image2D width, height: " << max_image2d_width << ", " << max_image2d_height << endl;
      cout << "Max image3D width, height, depth: " << max_image3d_width << ", " << max_image3d_height << ", " << max_image3d_depth << endl;
      cout << "Image support: " << image_support << endl;

      size_t max_parameter_size = iter->getInfo<CL_DEVICE_MAX_PARAMETER_SIZE> ();
      size_t max_samplers = iter->getInfo<CL_DEVICE_MAX_SAMPLERS> ();
      cl_uint mem_base_addr_align = iter->getInfo<CL_DEVICE_MEM_BASE_ADDR_ALIGN> ();
      cl_uint min_data_type_align_size = iter->getInfo<CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE> ();
      cl_uint global_mem_cacheline_size = iter->getInfo<CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE> ();
      cl_ulong global_mem_cache_size = iter->getInfo<CL_DEVICE_GLOBAL_MEM_CACHE_SIZE> ();
      cl_ulong global_mem_size = iter->getInfo<CL_DEVICE_GLOBAL_MEM_SIZE> ();
      cl_ulong max_constant_buffer_size = iter->getInfo<CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE> ();
      cl_uint max_constant_args = iter->getInfo<CL_DEVICE_MAX_CONSTANT_ARGS> ();
      cl_ulong local_mem_size = iter->getInfo<CL_DEVICE_LOCAL_MEM_SIZE> ();
      cl_bool error_correction_support = iter->getInfo<CL_DEVICE_ERROR_CORRECTION_SUPPORT> ();

      cout << "Max parameter size: " << max_parameter_size << endl;
      cout << "Max samplers: " << max_samplers << endl;
      cout << "Memory base address align: " << mem_base_addr_align << endl;
      cout << "Minimum data type align size: " << min_data_type_align_size << endl;
      cout << "Global memory cacheline size: " << global_mem_cacheline_size << endl;
      cout << "Global memory cache size: " << global_mem_cache_size << endl;
      cout << "Global memory size: " << global_mem_size << endl;
      cout << "Maximum constant buffer size: " << max_constant_buffer_size << endl;
      cout << "Maximum constant arguments: " << max_constant_args << endl;
      cout << "Local memory size: " << local_mem_size << endl;
      cout << "Error correction support: " << error_correction_support << endl;

    }

}

void
    ZCLController::initCLBuffers(Entities::ZEntityBuffers* entBufs, WorldMap* worldMap)
{
  using Ogre::Real;
  using Ogre::uchar;
  if(entBufs == 0 || worldMap == 0)
  {
      Ogre::LogManager::getSingleton().logMessage(Ogre::LML_CRITICAL, 
          "Cannot initialize OpenCL Buffers. Entitity buffer or world map is null.");
      return;
  }
  cl_int err;
  Entities::ZEntityBuffers* entsBuf = entBufs;
  assert((worldMap->_COMPONENT_DIM == entsBuf->COMPONENT_DIM) && "Class Invariance failed! WorldMap component dimension does not agree with entities' component dimension.");

  //Set up constant input buffers.
  _mapShape = worldMap->getShape();
  cout << "World map shape: (" << _mapShape[0] << ", " << _mapShape[1] << ")" << endl;
  _mapBufLen = _mapShape[0] * _mapShape[1] * worldMap->_COMPONENT_DIM * sizeof(Real); //Because we have a U,V shaped map, with N dimensions per element. Len is in bytes.
  //Load up gradient and contour buffers.
  cout << "Loading map OpenCL buffers." << endl;
  _gradMap = worldMap->getGradientMap();
  _contourMap = worldMap->getContourMap();
  _gradCL = cl::Buffer(_context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, _mapBufLen, _gradMap, &err); //Real buffer length is in bytes.
  _chkErr(err, "Buffer::Buffer(): gradient read-only buffer.");
  _contourCL = cl::Buffer(_context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, _mapBufLen, _contourMap, &err);

  //Setup entities buffers.
  cout << "Loading entities OpenCL buffers." << endl;
  _entsDim = entsBuf->COMPONENT_DIM;
  _numOfEnts = entsBuf->numOfEnts;
  _entsPosBuf = entsBuf->worldPos;
  _entsOrientBuf = entsBuf->worldOrient;
  _entsVelBuf = entsBuf->velocity;
  _entsModeBuf = entsBuf->mode;
  _entsGoalsBuf = entsBuf->goals;
  _entsStoreOneBuf = entsBuf->storeone;
  _densityBuf = entsBuf->density;

  size_t bufferLen = _entsDim * _numOfEnts * sizeof(Real); //We have numOfEnts entities with a _entsDim dimensional vector per entity.
  _entsBufLen = bufferLen;
  //Initialize the OpenCL buffers by using host memory ptr.
  //Position
  _entsPosCL = cl::Buffer(_context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, bufferLen, _entsPosBuf, &err);
  //_entsPosCL = cl::Buffer(_context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_WRITE, bufferLen, _entsPosBuf, &err);
  _chkErr(err, "Buffer::Buffer(): entities position buffer.");
  //Orientation
  _entsOrientCL = cl::Buffer(_context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, bufferLen, _entsOrientBuf, &err);
  //_entsOrientCL = cl::Buffer(_context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_WRITE, bufferLen, _entsOrientBuf, &err);
  _chkErr(err, "Buffer::Buffer(): entities orientation buffer.");
  //Velocity
  _entsVelCL = cl::Buffer(_context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, bufferLen, _entsVelBuf, &err);
  //_entsVelCL = cl::Buffer(_context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_WRITE, bufferLen, _entsVelBuf, &err);
  //Mode buffer
  //_entsModeCL = cl::Buffer(_context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, _numOfEnts * sizeof(unsigned char), _entsModeBuf, &err);
  //_entsModeCL = cl::Buffer(_context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_WRITE, _numOfEnts * sizeof(unsigned char), _entsModeBuf, &err);
  _chkErr(err, "Buffer::Buffer(): entities mode buffer.");
  _entsGoalsCL = cl::Buffer(_context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, bufferLen, _entsGoalsBuf, &err);
  _chkErr(err, "Buffer::Buffer(): entities goals buffer.");
  _entsStoreOneCL = cl::Buffer(_context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, bufferLen, _entsStoreOneBuf, &err);
  _chkErr(err, "Buffer::Buffer(): entities goals buffer.");
  _densityCL = cl::Buffer(_context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, _mapBufLen, _densityBuf, &err);
  cout << "Done loading OpenCL buffers." << endl;
}

void
ZCLController::initArgs()
{
  cl_int err;
  _argI = 0;
  size_t i = 0;
  err = _kernel[0].setArg(_argI++, _gradCL); //gradIn
  _chkErr(err, "Kernel::setArg()");
  err = _kernel[0].setArg(_argI++, _contourCL); //contourIn
  _chkErr(err, "Kernel::setArg()");
  err = _kernel[0].setArg(_argI++, _entsPosCL); //entsPos
  _chkErr(err, "Kernel::setArg()");
  err = _kernel[0].setArg(_argI++, _entsOrientCL);
  _chkErr(err, "Kernel::setArg()");
  err = _kernel[0].setArg(_argI++, _entsVelCL); //velocity
  _chkErr(err, "Kernel::setArg() velocity buffer");
  //err = _kernel[0].setArg(_argI++, _entsModeCL);
  //_chkErr(err, "Kernel::setArg()");
  err = _kernel[0].setArg(_argI++, _entsGoalsCL);
  err = _kernel[0].setArg(_argI++, _densityCL);

  err = _kernel[0].setArg(_argI++, _entsStoreOneCL);
  err = _kernel[0].setArg(_argI++, _numOfEnts);
  _chkErr(err, "Kernel::setArg()");
  err = _kernel[0].setArg(_argI++, _mapShape[0]); //Where _mapShapep[0] specifies U, or number of rows.
  _chkErr(err, "Kernel::setArg()");
  err = _kernel[0].setArg(_argI++, _mapShape[1]); //Where _mapShape[1] specifies V, or number of columns.
  _chkErr(err, "Kernel::setArg()");
  err = _kernel[0].setArg(_argI++, WSCALE.unitsPerMeter);
  _chkErr(err, "Kernel::setArg() units per meter");
  err = _kernel[0].setArg(_argI, 0.0f); //initialize dt.

  _kernel[1].setArg(0, _entsPosCL);
  _kernel[1].setArg(1, _densityCL);
  _kernel[1].setArg(2, _mapShape[0]);
  _kernel[1].setArg(3, _mapShape[1]);
  _kernel[1].setArg(4, WSCALE.unitsPerMeter);
  _counter.Reset();
}

/**
 *This method will handle onUpdate events from the engine.
 **/
bool
ZCLController::onUpdate(const Ogre::FrameEvent &evt)
{

  _counter.Start();
  //for(size_t i = 0; i < _iterations; ++i)
  //{

  //enqueueKernel1(false); //THis won't work because of global sync. issues. You need to sort/bin the items per workgroup first for this to work.

  _kernel[0].setArg(_argI, evt.timeSinceLastFrame); //update dt.
  enqueueKernel0(false); //run the kernels


  //}


  _counter.Stop();

  _deviceKernelTime = (_counter.GetElapsedTime() + _deviceKernelTime) / ++_loopI;// / double(_iterations);

  return true;
}

void
ZCLController::enqueueKernel1(bool block = false)
{
  cl::Event e;
  _queue.enqueueNDRangeKernel(_kernel[1], cl::NullRange, cl::NDRange(_numOfEnts), cl::NDRange(GROUP_SIZE), 0, &e);

  //if (block)
    // e.wait();

  //if (block)
    //  {
        //read back the buffer. Going to be slow.
      //  _queue.enqueueReadBuffer(_entsPosCL, true, 0, _entsBufLen, _entsPosBuf);
        //_queue.enqueueReadBuffer(_densityCL, true, 0, _entsBufLen, _densityBuf);
      //}
    //_queue.finish();
}

void
ZCLController::enqueueKernel0(bool block = false)
{
  cl::Event e;
  _queue.enqueueNDRangeKernel(_kernel[0], cl::NullRange, cl::NDRange(_numOfEnts), cl::NDRange(GROUP_SIZE), 0, &e);

  if (_useGPU)
     {
       //read back the buffer. Going to be slow.
       _queue.enqueueReadBuffer(_entsPosCL, true, 0, _entsBufLen, _entsPosBuf);
       _queue.enqueueReadBuffer(_entsOrientCL, true, 0, _entsBufLen, _entsOrientBuf);
       _queue.enqueueReadBuffer(_entsVelCL, true, 0, _entsBufLen, _entsVelBuf);
       _queue.enqueueReadBuffer(_entsGoalsCL, true, 0, _entsBufLen, _entsGoalsBuf);
       _queue.enqueueReadBuffer(_entsStoreOneCL, true, 0, _entsBufLen, _entsStoreOneBuf);
       _queue.enqueueReadBuffer(_densityCL, true, 0, _entsBufLen, _densityBuf);
    }
   _queue.finish();


}

bool
ZCLController::onUpdate()
{
  const float dt = 0.016f; //test dt.
  _counter.Reset();
  _counter.Start();
  for (size_t i = 0; i < _iterations; ++i)
    {
      _kernel[0].setArg(_argI, dt); //update dt.
      enqueueKernel0(false); //run the kernels
    }
  _queue.finish();
  _counter.Stop();
  _deviceKernelTime = _counter.GetElapsedTime() / double(_iterations);
  return true;
}

void
ZCLController::printKernelTime()
{

  cout << "Average Time: " << _deviceKernelTime << endl;
}
