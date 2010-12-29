/**
Permission is hereby granted by Fdastero LLC, free of charge, to any person obtaining a copy
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
/**
*author: beyzend 
*email: llwijk@gmail.com
**/
#pragma once

#include "gui/GuiPrerequisite.h"

namespace ZGame
{
    namespace Gui
    {
        class DocumentManager
        {
        public:
            DocumentManager(){}
            ~DocumentManager(){}

            /** \brief define a key to document. Document assumed to be null.*/
            void define(const Ogre::StringVector& path)
            {
                for(size_t i=0; i < path.size(); ++i)
                {
                    _docMap[path[i].c_str()] = static_cast<Rocket::Core::ElementDocument*>(0);
                }
            }
            void
                defineRoot(const Rocket::Core::String& root)
            {
                _rootKey = root;
                //validate root key
                StrToDocumentMap::const_iterator findMe = _docMap.find(_rootKey);
                if(findMe == _docMap.end())
                    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Root key is invalid",
                    "DocumentManager::defineRoot");
            }

            Rocket::Core::ElementDocument*
                getRootDocument()
            {
                if(_rootKey.Empty())
                    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "RootKey is empty",
                    "DocumentManager::getRootDocument");
                //if we get here then root key has to be valid, because defineRoot should of caught 
                //problems with that. But still, let's find it.
                StrToDocumentMap::const_iterator findMe = _docMap.find(_rootKey);
                if(findMe == _docMap.end())
                    OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "Rootkey is not found.",
                    "DocumentManager::getRootDocument");
                return _docMap[_rootKey];
            }

            Rocket::Core::ElementDocument*
                get(const Rocket::Core::String &key)
            {
                StrToDocumentMap::iterator findMe = _docMap.find(key);
                if(findMe == _docMap.end())
                    OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "Document not found in Manager.",
                    "DocumentManager::get");
                return findMe->second;
            }

            void clear()
            {
                _docMap.clear();
            }
            StrToDocumentMap& 
                getAll()
            {
                return _docMap;
            }


        protected:
        private:
            StrToDocumentMap _docMap;
            Rocket::Core::String _rootKey;
        };
    }
}