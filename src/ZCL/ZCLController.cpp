#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using std::ifstream;
using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::cerr;

#include "ZCL/ZCLController.h"
#include "entities/EntitiesManager.h"
#include "world/WorldMap.h"

using ZGame::Entities::ZEntityBuffers;
using ZGame::World::WorldMap;

using namespace ZGame::ZCL;

ZCLController::ZCLController() : _entsDim(0), _numOfEnts(0), _entsBufLen(0), _entsPosBuf(0), _entsOrientBuf(0), _entsModeBuf(0),
    _mapBufLen(0), _gradMap(0), _contourMap(0)
{
}

ZCLController::~ZCLController()
{
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
        char*  str;
        string s;

        if(f.is_open())
        {
            size_t fileSize;
            f.seekg(0, ifstream::end);
            size = fileSize = f.tellg();
            f.seekg(0, ifstream::beg);

            str = new char[size+1];
            if (!str) throw(&std::exception("Could not allocate memory"));

            f.read(str, fileSize);
            f.close();
            str[size] = '\0';

            s = str;
            delete [] str;
            return s;
        }
    }
    catch(std::string msg)
    {
        cerr << "Exception caught in FileToString(): " << msg << endl;
        if(f.is_open())
            f.close();
    }
    catch(...)
    {
        cerr << "Exception caught in FileToString()" << endl;
        if(f.is_open())
            f.close();
    }
    string errorMsg = "FileToString()::Error: Unable to open file "
        + fileName;
    std::exception except(errorMsg.c_str());
    throw(except);
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
    ZCLController::init(std::string configName, ZEntityBuffers* entBufs, WorldMap* worldMap)
{
    initCL(configName);
    //Call init buffers with entManager data and world controller.
    cout << "Initializing OpenCL buffers." << endl;
    initCLBuffers(entBufs, worldMap);
    cout << "Finished initializing OpenCL buffers." << endl;
    cout << "Initializing OpenCL arguments." << endl;
    initArgs();
    cout << "Finished initializing OpenCL arguments." << endl;
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
    if(!(allPlatforms.size() > 0))
        throw (std::exception("ZCLController::init() failed: No platforms found!"));
    targetPlatform = allPlatforms[0];

    cout << "Creating OpenCL GPU context." << endl;
    cl_context_properties cprops[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties)targetPlatform(), 0};

    //We're going to check out GPU devices.

    _context = cl::Context(CL_DEVICE_TYPE_GPU, cprops, 0, 0, &err);
    _chkErr(err, "Context::Context() for GPU devices.");
    _devices = _context.getInfo<CL_CONTEXT_DEVICES>();
    printDeviceInfo(_devices);
    _devices.clear(); //be sure to clear devices here. Note: We are doing this so we can get a look at GPU. We shouldn't be doing it this way normally.

    cout << "Creating OpenCL CPU context." << endl;
    _context = cl::Context(CL_DEVICE_TYPE_CPU, cprops, 0, 0, &err);
    _chkErr(err, "Context::Context() for CPU devices.");

    _devices = _context.getInfo<CL_CONTEXT_DEVICES>();
    if(_devices.size() < 1)
        throw (std::exception("ZCLController::init() failed: no devices found!"));
    cout << "Number of devices in system: " << _devices.size() << endl;

    printDeviceInfo(_devices); //output CPU devices.

    cout << "Loading Kernels." << endl;
    string sourceStr = FileToString(configName);
    cl::Program::Sources sources(1, std::make_pair(sourceStr.c_str(), sourceStr.length()));
    _program = cl::Program(_context, sources);
    err = _program.build(_devices,"");
    _chkErr(err, "Program::build");
    cl::Kernel kernel = cl::Kernel(_program, "updateEnt", &err);
    _chkErr(err, "Kernel::Kernel");
    _kernel.push_back(kernel);


    cout << "Creating command queue" << endl;
    _queue = cl::CommandQueue(_context, _devices[0]);
}

void
    ZCLController::printDeviceInfo(std::vector<cl::Device> &devs)
{
    for(std::vector<cl::Device>::iterator iter = devs.begin(); iter != devs.end(); ++iter)
    {
        std::vector<size_t> max_work_items_sizes = iter->getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();
        std::string device_name = iter->getInfo<CL_DEVICE_NAME>();
        std::string device_vendor = iter->getInfo<CL_DEVICE_VENDOR>();
        std::string device_profile = iter->getInfo<CL_DEVICE_PROFILE>();
        std::string device_version = iter->getInfo<CL_DEVICE_VERSION>();
        std::string driver_version = iter->getInfo<CL_DRIVER_VERSION>();
        std::string extensions = iter->getInfo<CL_DEVICE_EXTENSIONS>();

        int dim=0;
        for(std::vector<size_t>::iterator jter = max_work_items_sizes.begin();
            jter != max_work_items_sizes.end(); ++jter)
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

        cl_uint max_compute_units = iter->getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
        cl_uint max_work_item_dim = iter->getInfo<CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS>();
        cl_uint max_work_group_size = iter->getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
        cl_uint preferred_vec_width_char = iter->getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR>();
        cl_uint preferred_vec_width_int = iter->getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT>();
        cl_uint preferred_vec_width_float = iter->getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT>();

        cout << "Max compute units: " << max_compute_units << endl;
        cout << "Max work item dimensions: " << max_work_item_dim << endl;
        cout << "Max work group size: " << max_work_group_size << endl;
        cout << "preferred vector width char: " << preferred_vec_width_char << endl;
        cout << "preferred vector width int: " << preferred_vec_width_int << endl;
        cout << "preferred vector width float: " << preferred_vec_width_float << endl;

        cl_uint max_clock_freq = iter->getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>();
        cl_uint max_read_image_args = iter->getInfo<CL_DEVICE_MAX_READ_IMAGE_ARGS>();
        cl_uint max_write_image_args = iter->getInfo<CL_DEVICE_MAX_WRITE_IMAGE_ARGS>();
        cl_ulong max_mem_alloc_size = iter->getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>();
        size_t max_image2d_width = iter->getInfo<CL_DEVICE_IMAGE2D_MAX_WIDTH>();
        size_t max_image2d_height = iter->getInfo<CL_DEVICE_IMAGE2D_MAX_HEIGHT>();
        size_t max_image3d_width = iter->getInfo<CL_DEVICE_IMAGE3D_MAX_WIDTH>();
        size_t max_image3d_height = iter->getInfo<CL_DEVICE_IMAGE3D_MAX_HEIGHT>();
        size_t max_image3d_depth = iter->getInfo<CL_DEVICE_IMAGE3D_MAX_DEPTH>();
        cl_uint image_support = iter->getInfo<CL_DEVICE_IMAGE_SUPPORT>();

        cout << "Max clock frequency: " << max_clock_freq << endl;
        cout << "Max read image arguments: " << max_read_image_args << endl;
        cout << "Max write image arguments: " << max_write_image_args << endl;
        cout << "Max memory allocation size: " << max_mem_alloc_size << endl;
        cout << "Max image2D width, height: " << max_image2d_width << ", " << max_image2d_height << endl;
        cout << "Max image3D width, height, depth: " << max_image3d_width << ", " << max_image3d_height << ", " << max_image3d_depth << endl;
        cout << "Image support: " << image_support << endl;

        size_t max_parameter_size = iter->getInfo<CL_DEVICE_MAX_PARAMETER_SIZE>();
        size_t max_samplers = iter->getInfo<CL_DEVICE_MAX_SAMPLERS>();
        cl_uint mem_base_addr_align = iter->getInfo<CL_DEVICE_MEM_BASE_ADDR_ALIGN>();
        cl_uint min_data_type_align_size = iter->getInfo<CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE>();
        cl_uint global_mem_cacheline_size = iter->getInfo<CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE>();
        cl_ulong global_mem_cache_size = iter->getInfo<CL_DEVICE_GLOBAL_MEM_CACHE_SIZE>();
        cl_ulong global_mem_size = iter->getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
        cl_ulong max_constant_buffer_size = iter->getInfo<CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE>();
        cl_uint max_constant_args = iter->getInfo<CL_DEVICE_MAX_CONSTANT_ARGS>();
        cl_ulong local_mem_size = iter->getInfo<CL_DEVICE_LOCAL_MEM_SIZE>();
        cl_bool error_correction_support = iter->getInfo<CL_DEVICE_ERROR_CORRECTION_SUPPORT>();

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
    ZCLController::initCLBuffers(ZEntityBuffers* entBufs, WorldMap* worldMap)
{
    using Ogre::Real;
    using Ogre::uchar;
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
    _gradCL = cl::Buffer(_context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
        _mapBufLen, _gradMap, &err); //Real buffer length is in bytes.
    _chkErr(err, "Buffer::Buffer(): gradient read-only buffer.");
    _contourCL = cl::Buffer(_context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
        _mapBufLen, _contourMap, &err);
        
    //Setup entities buffers.
    cout << "Loading entities OpenCL buffers." << endl;
    _entsDim = entsBuf->COMPONENT_DIM;
    _numOfEnts = entsBuf->numOfEnts;
    _entsPosBuf = entsBuf->worldPos;
    _entsOrientBuf = entsBuf->worldOrient;
    _entsModeBuf = entsBuf->mode;

    size_t bufferLen = _entsDim * _numOfEnts * sizeof(Real); //We have numOfEnts entities with a _entsDim dimensional vector per entity.
    _entsBufLen = bufferLen;
    //Initialize the OpenCL buffers by using host memory ptr.
    //Position
    _entsPosCL = cl::Buffer(_context, CL_MEM_USE_HOST_PTR, bufferLen, 
            _entsPosBuf, &err);
    _chkErr(err, "Buffer::Buffer(): entities position buffer.");
    //Orientation
    _entsOrientCL = cl::Buffer(_context, CL_MEM_USE_HOST_PTR, bufferLen,
        _entsOrientBuf, &err);
    _chkErr(err, "Buffer::Buffer(): entities orientation buffer.");
    //Mode buffer
    _entsModeCL = cl::Buffer(_context, CL_MEM_USE_HOST_PTR, _numOfEnts * sizeof(Real),
        _entsModeBuf, &err);
    _chkErr(err, "Buffer::Buffer(): entities mode buffer."); 
}

void
    ZCLController::initArgs()
{
    cl_int err;
    err =  _kernel[0].setArg(0, _gradCL); //gradIn
    _chkErr(err, "Kernel::setArg()");
    err = _kernel[0].setArg(1, _contourCL); //contourIn
    _chkErr(err, "Kernel::setArg()");
    err = _kernel[0].setArg(2, _entsPosCL); //entsPos
    _chkErr(err, "Kernel::setArg()");
    err = _kernel[0].setArg(3, _entsOrientCL);
    _chkErr(err, "Kernel:;setArg()");
    err = _kernel[0].setArg(4, _entsModeCL);
}


/**
*This method will handle onUpdate events from the engine.
**/
bool
    ZCLController::onUpdate(const Ogre::FrameEvent &evt)
{

    return true;
}

bool
    ZCLController::onUpdate()
{
    /*
    //Let's enqueue the our kernel.
    cl_int err;
    cl::Event event;
    err = _queue.enqueueNDRangeKernel(_kernel[0], 
        cl::NullRange,
        cl::NDRange(_hw.length()+1),
        cl::NDRange(1, 1),
        0,
        &event);
    event.wait(); //block
    err = _queue.enqueueReadBuffer(_outCL,
        CL_TRUE, 0, _hw.length()+1, _hwBuffer);
    _chkErr(err, "CommandQueue::enqueueReadBuffer()");
    cout << _hwBuffer << endl;
    */
    return true;
}