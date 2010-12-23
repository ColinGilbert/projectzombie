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
#include <iostream>
#include "gui/Screens.h"
#include "gui/ScreenXForm.h"
using std::cout; using std::endl;
using namespace ZGame::Gui;

void
    ScreenTransitionTranslate::_resetParams()
{
    _isDone = false;
    _accumulatedT = 0.0f;

}

void
    ScreenTransitionTranslate::pushTransition(Screens* from, Screens* to)
{
    //We will assume offset parent fills the screen. That is, absolute offset left and
    //offsetLeft (from offset parent) are the same. This is not 100%
    //true. The fix is to accumlate offsets all the way up to root document. 
    _resetParams();
    _fromDocs = &(from->getDocumentMap());
    _toDocs = &(to->getDocumentMap());
     _from = from;
    _to = to;
    _to->show(); //We should not be responsible for showing here. It should be down upstreams.
    //Here we wrongly assume the iterator is the "root" document. This needs to be fixed
    //but for now we just assume this.
    if(_fromDocs->size() > 0)
    {
        StrToDocumentMap::iterator first = _fromDocs->begin();
        _totalOffset = Ogre::Vector2(-first->second->GetClientWidth(), 0.0f);
        _accumulatedOffset = Ogre::Vector2(first->second->GetAbsoluteLeft(), 0.0f);//here assume it's going to be zero for now.
        _negate = 1;
    }
}

void
    ScreenTransitionTranslate::popTransition(Screens* from, Screens* to)
{
    _resetParams();
    _fromDocs = &(from->getDocumentMap());
    _toDocs = &(to->getDocumentMap());
    _from = from;
    _to = to;
    _from->show();
    if(_fromDocs->size() > 0)
    {
        StrToDocumentMap::iterator first = _fromDocs->begin();
        _totalOffset = Ogre::Vector2(first->second->GetClientWidth(), 0.0f);
        _accumulatedOffset = Ogre::Vector2(first->second->GetAbsoluteLeft(), 0.0f);
        _negate = -1;
    }
}

void
    ScreenTransitionTranslate::step(float dt)
{
    Ogre::Real tempT = dt / _allocatedTimeInSecs;

     _accumulatedT += tempT;
    if(_accumulatedT > 1.0f)
    {
        _isDone = true;
        return;
    }
    for(StrToDocumentMap::iterator iter = _fromDocs->begin();
        iter != _fromDocs->end(); ++iter)
    {
        Rocket::Core::Element* offsetParent = iter->second->GetOffsetParent();
        _accumulatedOffset += _totalOffset * tempT;
        iter->second->SetOffset(Rocket::Core::Vector2f(_accumulatedOffset.x, _accumulatedOffset.y),
            offsetParent);
    }
    for(StrToDocumentMap::iterator iter = _toDocs->begin();
        iter != _toDocs->end(); ++iter)
    {
        Rocket::Core::Element* offsetParent = iter->second->GetOffsetParent();
        float width = iter->second->GetClientWidth();
        iter->second->SetOffset(Rocket::Core::Vector2f(_accumulatedOffset.x + _negate*width, _accumulatedOffset.y),
            offsetParent);
    }
   

}