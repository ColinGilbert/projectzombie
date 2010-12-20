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

#include "gui/Screens.h"
#include "gui/GuiController.h"

using namespace ZGame::Gui;

Screens::Screens(GuiController* guiCtrl) : _guiCtrl(guiCtrl)
{
    //We assume Rocket::Core has been initialized.
    Rocket::Core::Factory::RegisterEventListenerInstancer(this);
    this->RemoveReference();
}

Screens::~Screens()
{
}

void
    Screens::_loadDocuments(StrToDocumentMap &docMap)
{
    assert(_guiCtrl != 0 && "_guiCtrl is not supposed to be null.");
    _guiCtrl->loadDocumentsWithContext(_guiCtrl->getGui2d(), docMap);
}
/**
* temp. test method.
**/
void
    Screens::_showAllDocs(StrToDocumentMap &docMap)
{
    StrToDocumentMap::const_iterator cIter;
    for(cIter = docMap.cbegin(); cIter != docMap.end(); ++cIter)
    {
        cIter->second->Show();
    }
}

void
    Screens::_showDoc(Rocket::Core::ElementDocument* theDoc)
{
    theDoc->Show();
}

void
    Screens::_hideDoc(Rocket::Core::ElementDocument* theDoc)
{
    theDoc->Hide();
}