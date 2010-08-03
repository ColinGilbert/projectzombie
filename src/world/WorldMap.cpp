#include <iostream>
#include <fstream>
#include <stdexcept>
using std::cout;
using std::endl;
using std::exception;
using std::ifstream;
using std::ios;
#include "world/WorldMap.h"

using namespace Ogre;
using ZGame::World::WorldMap;

WorldMap::WorldMap(void) : _grad(0), _contour(0), _buffer(0)
{
}

WorldMap::~WorldMap(void)
{
    delete [] _grad;
    delete [] _contour;
    delete [] _buffer;
    _grad = _contour = 0;
    _buffer = 0;
}

/**
*This method will load the world map. This mean it will parse the zdt (zombie data binary file) and load the gradient U,V and contour U,V maps. 
*
*
*/
void
    WorldMap::load(string fileName)
{
    ifstream::pos_type size;
    ifstream file;
    //Open the file and read in the entire content into a buffer.
    try
    {
        cout << "Opening world map data file for reading." << endl;

        //ifstream file(fileName.c_str(), ios::in | ios::binary | ios::ate);
        file.open(fileName.c_str(), ios::in | ios::binary | ios::ate);
        if(file.is_open())
        {
            size = file.tellg();
            _buffer = new char[size];
            file.seekg(0, ios::beg);
            file.read(_buffer, size);
            file.close();
            //Let's assume that the header is one unsigned long specifying the data file code: 420.
            //Then followed by the shape, which are two unsigned longs. We assume unsigned long as 4 bit.
            ZDT_HEADER* zdtF = reinterpret_cast<ZDT_HEADER*>(_buffer);
            cout << "ZDT_FILE CODE: " << zdtF->CODE << endl;
            cout << "Shape U,V: " << zdtF->ShapeU << "," << zdtF->ShapeV << endl;
            _shape.push_back(zdtF->ShapeU); _shape.push_back(zdtF->ShapeV);
            //We now loop and load the gradient and contour maps.
            //Here we assume that U ranges over the rows while V ranges over the columns.
            cout << "Reading in gradient map. " << endl;
            size_t numOfElements = zdtF->ShapeU*zdtF->ShapeV;
            _grad = new Real[numOfElements*_COMPONENT_DIM]; //So we have a ShapeU by ShapeV 4 element image. Four elements is because we want to utilize 4 wide vectorization in SSE. 
            Real* curPtr = reinterpret_cast<Real*>(++zdtF); //skip the header to get to data.
            Real* mapPtr = _grad;
            loadMap(mapPtr, curPtr, numOfElements, _COMPONENT_DIM);
            cout << "Gradient Map read." << endl;
            //Read in contour map.
            _contour = new Real[numOfElements*_COMPONENT_DIM];
            mapPtr = _contour;
            cout << "Reading in contour map." << endl;
            loadMap(mapPtr, curPtr, numOfElements, _COMPONENT_DIM);
            cout << "Contour map read." << endl;
        }
        else
        {
            cout << "File was not opened somehow!" << endl;
            throw exception("Exception when trying to open WorldMap file");
        }
    }catch(exception e)
    {
        delete [] _buffer;
        _buffer = 0;
        if(file.is_open())
            file.close();
        throw e;
    }
    //Deletete the buffers. Note we're doing RIAA so we're also delete buffering in deconstructor just to be safe.
    delete [] _buffer;
    _buffer = 0;
}

void WorldMap::loadMap(Real* mapPtr, Real* inPtr, size_t numOfElements, size_t dim)
{
    for(size_t u=0; u < numOfElements; ++u)
    {
        mapPtr[0] = *inPtr;
        mapPtr[1] = *++inPtr; ++inPtr;
        //cout << "grad[0],grad[1]: " << _grad[0] << ", " << _grad[1] << endl;
        for(size_t i = 2; i < dim; ++i)
        {
            mapPtr[i] = 0.0f;
        }
        mapPtr += dim; //advance gradient map pointer by number of components.
    }
}
