#pragma once

#include <string>
using std::string;
#include <vector>
#include <Ogre.h>

//using Ogre::Vector2;
//using Ogre::Real;

namespace ZGame
{
    namespace World
    {

        class WorldMap
        {
        public:
            typedef std::vector<size_t> WORLD_SHAPE;
            static const size_t _COMPONENT_DIM = 4; //This specifies a 4D vector.
            WorldMap(void);
            virtual ~WorldMap(void);

            void 
                load(string fileName="../data_pot/testdata.zdt");

            const WORLD_SHAPE& 
                getShape() const
            {
                return _shape;
            }

            Ogre::Real* 
                getGradientMap() 
            {
                assert(_grad && "Class invariant failed. Trying to get a nonexistent gradient map.");
                return _grad;
            }

            Ogre::Real* 
                getContourMap()
            {
                assert(_contour && "Class invariant failed. Trying to get a nonexistent contour map.");
                return _contour;
            }
        protected:
        private:
            /** \brief A helper function that will load the data from input pointer into mapPtr.**/
            Ogre::Real* loadMap(Ogre::Real* mapPtr, Ogre::Real* inPtr, size_t numOfElements, size_t dim, bool swtch); 
        private:
            struct ZDT_HEADER
            {
                //header
                long CODE;
                long ShapeU;
                long ShapeV;
            };
            WORLD_SHAPE _shape;
            /** This is a buffer which contain interleaved U,V gradient values.*/
            Ogre::Real* _grad; 
            /** This is a buffer which contain interleaved U,V contour values.*/
            Ogre::Real* _contour;
            char* _buffer;

            

        };
    }
}