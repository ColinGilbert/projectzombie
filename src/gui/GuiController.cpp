#include "gui/GuiController.h"
#include "gui/RenderInterfaceOgre3D.h"
#include "gui/SystemInterfaceOgre3D.h"
#include <iostream>

using std::cout; 
using std::endl;

using namespace ZGame::Gui;

GuiController::GuiController() : _data_path(""), FONT_PATH(""), _vp(0), _gui2d(0)
{
    _fontStrVec.push_back("font/Delicious-Roman.otf");
    _fontStrVec.push_back("font/Delicious-Bold.otf");
    _fontStrVec.push_back("font/Delicious-Italic.otf");
    _fontStrVec.push_back("font/Delicious-BoldItalic.otf");
}

GuiController::~GuiController()
{
}

void
    GuiController::_createGui2d()
{
    _gui2d = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(_W_WIDTH, _W_HEIGHT));
    Rocket::Debugger::Initialise(_gui2d);
    Rocket::Core::ElementDocument* cursor = _gui2d->LoadMouseCursor(_data_path + "common/cursor.rml");
    if(cursor)
        cursor->RemoveReference();

}

/**
*This method will load documents in a string to document map. Here the assumption is that the map 
already contains the string keys, and such keys are valid resource locators to the documents.
**/
void
    GuiController::loadDocumentsWithContext(Rocket::Core::Context* context,
    StrToDocumentMap &docMap)
{
    StrToDocumentMap::iterator iter = docMap.begin();
    for(; iter != docMap.end(); iter++)
    {
        Rocket::Core::ElementDocument* doc = context->LoadDocument(_data_path + iter->first.c_str());
        if(doc)
        {
            doc->RemoveReference();
        }
        iter->second = doc;
    }
}

bool
    GuiController::onInit(ZGame::ZInitPacket initPacket)
{
    using Ogre::ResourceGroupManager;
    Ogre::Log::Stream debug = Ogre::LogManager::getSingleton().getLog("App.log")->stream(Ogre::LML_TRIVIAL);
    Ogre::Log::Stream log = Ogre::LogManager::getSingleton().getLog("App.log")->stream();

    debug << "in GuiController onInit\n";

    mKeyboard = initPacket.keyboard;

    try
    {
        log << "List of Rocket resource locations: \n";
        const ResourceGroupManager::LocationList locList = ResourceGroupManager::getSingleton().getResourceLocationList("Rocket");
        //WARNING: We are assuming here the first line is the "root" directory to GUI data. This is for now, we need
        //to validate or come up with better system (such as fully utilizing Ogre ResourceGroupManager for our resource
        //loading needs.
        ResourceGroupManager::LocationList::const_iterator iter = locList.cbegin();
        if(iter != locList.end()) //there is at least one line. We read the first line.
        {
            log << (*iter)->archive->getName() << "\n";
            _data_path = (*iter)->archive->getName().c_str();
        }
        else
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "Rocket resource location for data root was not found!",
            "GuiController::onInit");
        //rocket initialization
        _W_WIDTH = initPacket.renderWindow->getWidth();
        _W_HEIGHT = initPacket.renderWindow->getHeight();
        _vp = initPacket.initialCamera->getViewport();


        ogre_renderer = new RenderInterfaceOgre3D(_W_WIDTH, _W_HEIGHT);
        Rocket::Core::SetRenderInterface(ogre_renderer);

        ogre_system = new SystemInterfaceOgre3D();
        Rocket::Core::SetSystemInterface(ogre_system);


        Rocket::Core::Initialise();
        Rocket::Controls::Initialise();

        _createGui2d();

        for(std::vector<Rocket::Core::String>::const_iterator iter = _fontStrVec.cbegin(); iter != _fontStrVec.end(); ++iter)
        {
            Rocket::Core::FontDatabase::LoadFontFace(_data_path + FONT_PATH + (*iter));
        }
    }catch(Ogre::Exception e)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, Ogre::String("Failed to initialize Gui system. ").append(e.getDescription()),
            "GuiController::onInit");
    }



    return true;
}

bool
    GuiController::onUpdate(const Ogre::FrameEvent &evt)
{
    return true;
}

bool
    GuiController::onDestroy()
{
    Ogre::Log::Stream debug = Ogre::LogManager::getSingleton().getLog("App.log")->stream(Ogre::LML_TRIVIAL);
    Ogre::Log::Stream log = Ogre::LogManager::getSingleton().getLog("App.log")->stream();

    debug << "GuiController::onDestroy()\n";

    _gui2d->RemoveReference();
    Rocket::Core::Shutdown();
    log << "Rocket Core shutdown\n";
    delete ogre_system;
    ogre_system = 0;

    delete ogre_renderer;
    ogre_renderer = 0;

    return true;
}

bool
    GuiController::onRenderQueueStart(Ogre::uint8 queueGroupId,
    const Ogre::String& invocation, bool& skipThisInvocation)
{
    if(queueGroupId == Ogre::RENDER_QUEUE_OVERLAY && _vp->getOverlaysEnabled())
    {
        _gui2d->Update();
        ConfigureRenderSystem();
        _gui2d->Render();
    }
    return true;
}

bool
    GuiController::onRenderQueueEnd(Ogre::uint8 queueGroupId,
    const Ogre::String& invocation, bool& skipThisInvocation)
{
    return true;
}

void
    GuiController::ConfigureRenderSystem()
{
    Ogre::RenderSystem* render_system = Ogre::Root::getSingleton().getRenderSystem();

    // Set up the projection and view matrices.
    Ogre::Matrix4 projection_matrix;
    BuildProjectionMatrix(projection_matrix);
    render_system->_setProjectionMatrix(projection_matrix);
    render_system->_setViewMatrix(Ogre::Matrix4::IDENTITY);

    // Disable lighting, as all of Rocket's geometry is unlit.
    render_system->setLightingEnabled(false);
    // Disable depth-buffering; all of the geometry is already depth-sorted.
    render_system->_setDepthBufferParams(false, false);
    // Rocket generates anti-clockwise geometry, so enable clockwise-culling.
    render_system->_setCullingMode(Ogre::CULL_CLOCKWISE);
    // Disable fogging.
    render_system->_setFog(Ogre::FOG_NONE);
    // Enable writing to all four channels.
    render_system->_setColourBufferWriteEnabled(true, true, true, true);
    // Unbind any vertex or fragment programs bound previously by the application.
    render_system->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);
    render_system->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);

    // Set texture settings to clamp along both axes.
    Ogre::TextureUnitState::UVWAddressingMode addressing_mode;
    addressing_mode.u = Ogre::TextureUnitState::TAM_CLAMP;
    addressing_mode.v = Ogre::TextureUnitState::TAM_CLAMP;
    addressing_mode.w = Ogre::TextureUnitState::TAM_CLAMP;
    render_system->_setTextureAddressingMode(0, addressing_mode);

    // Set the texture coordinates for unit 0 to be read from unit 0.
    render_system->_setTextureCoordSet(0, 0);
    // Disable texture coordinate calculation.
    render_system->_setTextureCoordCalculation(0, Ogre::TEXCALC_NONE);
    // Enable linear filtering; images should be rendering 1 texel == 1 pixel, so point filtering could be used
    // except in the case of scaling tiled decorators.
    render_system->_setTextureUnitFiltering(0, Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_POINT);
    // Disable texture coordinate transforms.
    render_system->_setTextureMatrix(0, Ogre::Matrix4::IDENTITY);
    // Reject pixels with an alpha of 0.
    render_system->_setAlphaRejectSettings(Ogre::CMPF_GREATER, 0, false);
    // Disable all texture units but the first.
    render_system->_disableTextureUnitsFrom(1);

    // Enable simple alpha blending.
    render_system->_setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    // Disable depth bias.
    render_system->_setDepthBias(0, 0);
}

void
    GuiController::BuildProjectionMatrix(Ogre::Matrix4& projection_matrix)
{
    float z_near = -1;
    float z_far = 1;

    projection_matrix = Ogre::Matrix4::ZERO;

    // Set up matrices.
    projection_matrix[0][0] = 2.0f / _W_WIDTH;
    projection_matrix[0][3]= -1.0000000f;
    projection_matrix[1][1]= -2.0f / _W_HEIGHT;
    projection_matrix[1][3]= 1.0000000f;
    projection_matrix[2][2]= -2.0f / (z_far - z_near);
    projection_matrix[3][3]= 1.0000000f;
}

bool
    GuiController::onMouseMove(const OIS::MouseEvent& e)
{
    int key_modifier_state = GetKeyModifierState();

    _gui2d->ProcessMouseMove(e.state.X.abs, e.state.Y.abs, key_modifier_state);
    if (e.state.Z.rel != 0)
        _gui2d->ProcessMouseWheel(e.state.Z.rel / -120, key_modifier_state);

    return true;
}

bool
    GuiController::onMouseDown(const OIS::MouseEvent& ROCKET_UNUSED(e), OIS::MouseButtonID id)
{
    _gui2d->ProcessMouseButtonDown((int) id, GetKeyModifierState());
    return true;
}

bool
    GuiController::onMouseUp(const OIS::MouseEvent& ROCKET_UNUSED(e), OIS::MouseButtonID id)
{
    _gui2d->ProcessMouseButtonUp((int) id, GetKeyModifierState());
    return true;
}

bool
    GuiController::onKeyDown(const OIS::KeyEvent& e)
{
    Rocket::Core::Input::KeyIdentifier key_identifier = key_identifiers[e.key];

    // Toggle the debugger on a shift-~ press.
    if (key_identifier == Rocket::Core::Input::KI_OEM_3 &&
        (GetKeyModifierState() & Rocket::Core::Input::KM_SHIFT))
    {
        Rocket::Debugger::SetVisible(!Rocket::Debugger::IsVisible());
        return true;
    }

    if (key_identifier != Rocket::Core::Input::KI_UNKNOWN)
        _gui2d->ProcessKeyDown(key_identifier, GetKeyModifierState());

    // Send through the ASCII value as text input if it is printable.
    if (e.text >= 32)
        _gui2d->ProcessTextInput((Rocket::Core::word) e.text);
    else if (key_identifier == Rocket::Core::Input::KI_RETURN)
        _gui2d->ProcessTextInput((Rocket::Core::word) '\n');

    return true;
}

bool
    GuiController::onKeyUp(const OIS::KeyEvent& e)
{
    Rocket::Core::Input::KeyIdentifier key_identifier = key_identifiers[e.key];

    if (key_identifier != Rocket::Core::Input::KI_UNKNOWN)
        _gui2d->ProcessKeyUp(key_identifier, GetKeyModifierState());

    return true;
}

void
    GuiController::BuildKeyMaps()
{
    key_identifiers[OIS::KC_UNASSIGNED] = Rocket::Core::Input::KI_UNKNOWN;
    key_identifiers[OIS::KC_ESCAPE] = Rocket::Core::Input::KI_ESCAPE;
    key_identifiers[OIS::KC_1] = Rocket::Core::Input::KI_1;
    key_identifiers[OIS::KC_2] = Rocket::Core::Input::KI_2;
    key_identifiers[OIS::KC_3] = Rocket::Core::Input::KI_3;
    key_identifiers[OIS::KC_4] = Rocket::Core::Input::KI_4;
    key_identifiers[OIS::KC_5] = Rocket::Core::Input::KI_5;
    key_identifiers[OIS::KC_6] = Rocket::Core::Input::KI_6;
    key_identifiers[OIS::KC_7] = Rocket::Core::Input::KI_7;
    key_identifiers[OIS::KC_8] = Rocket::Core::Input::KI_8;
    key_identifiers[OIS::KC_9] = Rocket::Core::Input::KI_9;
    key_identifiers[OIS::KC_0] = Rocket::Core::Input::KI_0;
    key_identifiers[OIS::KC_MINUS] = Rocket::Core::Input::KI_OEM_MINUS;
    key_identifiers[OIS::KC_EQUALS] = Rocket::Core::Input::KI_OEM_PLUS;
    key_identifiers[OIS::KC_BACK] = Rocket::Core::Input::KI_BACK;
    key_identifiers[OIS::KC_TAB] = Rocket::Core::Input::KI_TAB;
    key_identifiers[OIS::KC_Q] = Rocket::Core::Input::KI_Q;
    key_identifiers[OIS::KC_W] = Rocket::Core::Input::KI_W;
    key_identifiers[OIS::KC_E] = Rocket::Core::Input::KI_E;
    key_identifiers[OIS::KC_R] = Rocket::Core::Input::KI_R;
    key_identifiers[OIS::KC_T] = Rocket::Core::Input::KI_T;
    key_identifiers[OIS::KC_Y] = Rocket::Core::Input::KI_Y;
    key_identifiers[OIS::KC_U] = Rocket::Core::Input::KI_U;
    key_identifiers[OIS::KC_I] = Rocket::Core::Input::KI_I;
    key_identifiers[OIS::KC_O] = Rocket::Core::Input::KI_O;
    key_identifiers[OIS::KC_P] = Rocket::Core::Input::KI_P;
    key_identifiers[OIS::KC_LBRACKET] = Rocket::Core::Input::KI_OEM_4;
    key_identifiers[OIS::KC_RBRACKET] = Rocket::Core::Input::KI_OEM_6;
    key_identifiers[OIS::KC_RETURN] = Rocket::Core::Input::KI_RETURN;
    key_identifiers[OIS::KC_LCONTROL] = Rocket::Core::Input::KI_LCONTROL;
    key_identifiers[OIS::KC_A] = Rocket::Core::Input::KI_A;
    key_identifiers[OIS::KC_S] = Rocket::Core::Input::KI_S;
    key_identifiers[OIS::KC_D] = Rocket::Core::Input::KI_D;
    key_identifiers[OIS::KC_F] = Rocket::Core::Input::KI_F;
    key_identifiers[OIS::KC_G] = Rocket::Core::Input::KI_G;
    key_identifiers[OIS::KC_H] = Rocket::Core::Input::KI_H;
    key_identifiers[OIS::KC_J] = Rocket::Core::Input::KI_J;
    key_identifiers[OIS::KC_K] = Rocket::Core::Input::KI_K;
    key_identifiers[OIS::KC_L] = Rocket::Core::Input::KI_L;
    key_identifiers[OIS::KC_SEMICOLON] = Rocket::Core::Input::KI_OEM_1;
    key_identifiers[OIS::KC_APOSTROPHE] = Rocket::Core::Input::KI_OEM_7;
    key_identifiers[OIS::KC_GRAVE] = Rocket::Core::Input::KI_OEM_3;
    key_identifiers[OIS::KC_LSHIFT] = Rocket::Core::Input::KI_LSHIFT;
    key_identifiers[OIS::KC_BACKSLASH] = Rocket::Core::Input::KI_OEM_5;
    key_identifiers[OIS::KC_Z] = Rocket::Core::Input::KI_Z;
    key_identifiers[OIS::KC_X] = Rocket::Core::Input::KI_X;
    key_identifiers[OIS::KC_C] = Rocket::Core::Input::KI_C;
    key_identifiers[OIS::KC_V] = Rocket::Core::Input::KI_V;
    key_identifiers[OIS::KC_B] = Rocket::Core::Input::KI_B;
    key_identifiers[OIS::KC_N] = Rocket::Core::Input::KI_N;
    key_identifiers[OIS::KC_M] = Rocket::Core::Input::KI_M;
    key_identifiers[OIS::KC_COMMA] = Rocket::Core::Input::KI_OEM_COMMA;
    key_identifiers[OIS::KC_PERIOD] = Rocket::Core::Input::KI_OEM_PERIOD;
    key_identifiers[OIS::KC_SLASH] = Rocket::Core::Input::KI_OEM_2;
    key_identifiers[OIS::KC_RSHIFT] = Rocket::Core::Input::KI_RSHIFT;
    key_identifiers[OIS::KC_MULTIPLY] = Rocket::Core::Input::KI_MULTIPLY;
    key_identifiers[OIS::KC_LMENU] = Rocket::Core::Input::KI_LMENU;
    key_identifiers[OIS::KC_SPACE] = Rocket::Core::Input::KI_SPACE;
    key_identifiers[OIS::KC_CAPITAL] = Rocket::Core::Input::KI_CAPITAL;
    key_identifiers[OIS::KC_F1] = Rocket::Core::Input::KI_F1;
    key_identifiers[OIS::KC_F2] = Rocket::Core::Input::KI_F2;
    key_identifiers[OIS::KC_F3] = Rocket::Core::Input::KI_F3;
    key_identifiers[OIS::KC_F4] = Rocket::Core::Input::KI_F4;
    key_identifiers[OIS::KC_F5] = Rocket::Core::Input::KI_F5;
    key_identifiers[OIS::KC_F6] = Rocket::Core::Input::KI_F6;
    key_identifiers[OIS::KC_F7] = Rocket::Core::Input::KI_F7;
    key_identifiers[OIS::KC_F8] = Rocket::Core::Input::KI_F8;
    key_identifiers[OIS::KC_F9] = Rocket::Core::Input::KI_F9;
    key_identifiers[OIS::KC_F10] = Rocket::Core::Input::KI_F10;
    key_identifiers[OIS::KC_NUMLOCK] = Rocket::Core::Input::KI_NUMLOCK;
    key_identifiers[OIS::KC_SCROLL] = Rocket::Core::Input::KI_SCROLL;
    key_identifiers[OIS::KC_NUMPAD7] = Rocket::Core::Input::KI_7;
    key_identifiers[OIS::KC_NUMPAD8] = Rocket::Core::Input::KI_8;
    key_identifiers[OIS::KC_NUMPAD9] = Rocket::Core::Input::KI_9;
    key_identifiers[OIS::KC_SUBTRACT] = Rocket::Core::Input::KI_SUBTRACT;
    key_identifiers[OIS::KC_NUMPAD4] = Rocket::Core::Input::KI_4;
    key_identifiers[OIS::KC_NUMPAD5] = Rocket::Core::Input::KI_5;
    key_identifiers[OIS::KC_NUMPAD6] = Rocket::Core::Input::KI_6;
    key_identifiers[OIS::KC_ADD] = Rocket::Core::Input::KI_ADD;
    key_identifiers[OIS::KC_NUMPAD1] = Rocket::Core::Input::KI_1;
    key_identifiers[OIS::KC_NUMPAD2] = Rocket::Core::Input::KI_2;
    key_identifiers[OIS::KC_NUMPAD3] = Rocket::Core::Input::KI_3;
    key_identifiers[OIS::KC_NUMPAD0] = Rocket::Core::Input::KI_0;
    key_identifiers[OIS::KC_DECIMAL] = Rocket::Core::Input::KI_DECIMAL;
    key_identifiers[OIS::KC_OEM_102] = Rocket::Core::Input::KI_OEM_102;
    key_identifiers[OIS::KC_F11] = Rocket::Core::Input::KI_F11;
    key_identifiers[OIS::KC_F12] = Rocket::Core::Input::KI_F12;
    key_identifiers[OIS::KC_F13] = Rocket::Core::Input::KI_F13;
    key_identifiers[OIS::KC_F14] = Rocket::Core::Input::KI_F14;
    key_identifiers[OIS::KC_F15] = Rocket::Core::Input::KI_F15;
    key_identifiers[OIS::KC_KANA] = Rocket::Core::Input::KI_KANA;
    key_identifiers[OIS::KC_ABNT_C1] = Rocket::Core::Input::KI_UNKNOWN;
    key_identifiers[OIS::KC_CONVERT] = Rocket::Core::Input::KI_CONVERT;
    key_identifiers[OIS::KC_NOCONVERT] = Rocket::Core::Input::KI_NONCONVERT;
    key_identifiers[OIS::KC_YEN] = Rocket::Core::Input::KI_UNKNOWN;
    key_identifiers[OIS::KC_ABNT_C2] = Rocket::Core::Input::KI_UNKNOWN;
    key_identifiers[OIS::KC_NUMPADEQUALS] = Rocket::Core::Input::KI_OEM_NEC_EQUAL;
    key_identifiers[OIS::KC_PREVTRACK] = Rocket::Core::Input::KI_MEDIA_PREV_TRACK;
    key_identifiers[OIS::KC_AT] = Rocket::Core::Input::KI_UNKNOWN;
    key_identifiers[OIS::KC_COLON] = Rocket::Core::Input::KI_OEM_1;
    key_identifiers[OIS::KC_UNDERLINE] = Rocket::Core::Input::KI_OEM_MINUS;
    key_identifiers[OIS::KC_KANJI] = Rocket::Core::Input::KI_KANJI;
    key_identifiers[OIS::KC_STOP] = Rocket::Core::Input::KI_UNKNOWN;
    key_identifiers[OIS::KC_AX] = Rocket::Core::Input::KI_OEM_AX;
    key_identifiers[OIS::KC_UNLABELED] = Rocket::Core::Input::KI_UNKNOWN;
    key_identifiers[OIS::KC_NEXTTRACK] = Rocket::Core::Input::KI_MEDIA_NEXT_TRACK;
    key_identifiers[OIS::KC_NUMPADENTER] = Rocket::Core::Input::KI_NUMPADENTER;
    key_identifiers[OIS::KC_RCONTROL] = Rocket::Core::Input::KI_RCONTROL;
    key_identifiers[OIS::KC_MUTE] = Rocket::Core::Input::KI_VOLUME_MUTE;
    key_identifiers[OIS::KC_CALCULATOR] = Rocket::Core::Input::KI_UNKNOWN;
    key_identifiers[OIS::KC_PLAYPAUSE] = Rocket::Core::Input::KI_MEDIA_PLAY_PAUSE;
    key_identifiers[OIS::KC_MEDIASTOP] = Rocket::Core::Input::KI_MEDIA_STOP;
    key_identifiers[OIS::KC_VOLUMEDOWN] = Rocket::Core::Input::KI_VOLUME_DOWN;
    key_identifiers[OIS::KC_VOLUMEUP] = Rocket::Core::Input::KI_VOLUME_UP;
    key_identifiers[OIS::KC_WEBHOME] = Rocket::Core::Input::KI_BROWSER_HOME;
    key_identifiers[OIS::KC_NUMPADCOMMA] = Rocket::Core::Input::KI_SEPARATOR;
    key_identifiers[OIS::KC_DIVIDE] = Rocket::Core::Input::KI_DIVIDE;
    key_identifiers[OIS::KC_SYSRQ] = Rocket::Core::Input::KI_SNAPSHOT;
    key_identifiers[OIS::KC_RMENU] = Rocket::Core::Input::KI_RMENU;
    key_identifiers[OIS::KC_PAUSE] = Rocket::Core::Input::KI_PAUSE;
    key_identifiers[OIS::KC_HOME] = Rocket::Core::Input::KI_HOME;
    key_identifiers[OIS::KC_UP] = Rocket::Core::Input::KI_UP;
    key_identifiers[OIS::KC_PGUP] = Rocket::Core::Input::KI_PRIOR;
    key_identifiers[OIS::KC_LEFT] = Rocket::Core::Input::KI_LEFT;
    key_identifiers[OIS::KC_RIGHT] = Rocket::Core::Input::KI_RIGHT;
    key_identifiers[OIS::KC_END] = Rocket::Core::Input::KI_END;
    key_identifiers[OIS::KC_DOWN] = Rocket::Core::Input::KI_DOWN;
    key_identifiers[OIS::KC_PGDOWN] = Rocket::Core::Input::KI_NEXT;
    key_identifiers[OIS::KC_INSERT] = Rocket::Core::Input::KI_INSERT;
    key_identifiers[OIS::KC_DELETE] = Rocket::Core::Input::KI_DELETE;
    key_identifiers[OIS::KC_LWIN] = Rocket::Core::Input::KI_LWIN;
    key_identifiers[OIS::KC_RWIN] = Rocket::Core::Input::KI_RWIN;
    key_identifiers[OIS::KC_APPS] = Rocket::Core::Input::KI_APPS;
    key_identifiers[OIS::KC_POWER] = Rocket::Core::Input::KI_POWER;
    key_identifiers[OIS::KC_SLEEP] = Rocket::Core::Input::KI_SLEEP;
    key_identifiers[OIS::KC_WAKE] = Rocket::Core::Input::KI_WAKE;
    key_identifiers[OIS::KC_WEBSEARCH] = Rocket::Core::Input::KI_BROWSER_SEARCH;
    key_identifiers[OIS::KC_WEBFAVORITES] = Rocket::Core::Input::KI_BROWSER_FAVORITES;
    key_identifiers[OIS::KC_WEBREFRESH] = Rocket::Core::Input::KI_BROWSER_REFRESH;
    key_identifiers[OIS::KC_WEBSTOP] = Rocket::Core::Input::KI_BROWSER_STOP;
    key_identifiers[OIS::KC_WEBFORWARD] = Rocket::Core::Input::KI_BROWSER_FORWARD;
    key_identifiers[OIS::KC_WEBBACK] = Rocket::Core::Input::KI_BROWSER_BACK;
    key_identifiers[OIS::KC_MYCOMPUTER] = Rocket::Core::Input::KI_UNKNOWN;
    key_identifiers[OIS::KC_MAIL] = Rocket::Core::Input::KI_LAUNCH_MAIL;
    key_identifiers[OIS::KC_MEDIASELECT] = Rocket::Core::Input::KI_LAUNCH_MEDIA_SELECT;
}

int
    GuiController::GetKeyModifierState()
{
    int modifier_state = 0;

    if (mKeyboard->isModifierDown(OIS::Keyboard::Ctrl))
        modifier_state |= Rocket::Core::Input::KM_CTRL;
    if (mKeyboard->isModifierDown(OIS::Keyboard::Shift))
        modifier_state |= Rocket::Core::Input::KM_SHIFT;
    if (mKeyboard->isModifierDown(OIS::Keyboard::Alt))
        modifier_state |= Rocket::Core::Input::KM_ALT;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32

    if (GetKeyState(VK_CAPITAL) > 0)
        modifier_state |= Rocket::Core::Input::KM_CAPSLOCK;
    if (GetKeyState(VK_NUMLOCK) > 0)
        modifier_state |= Rocket::Core::Input::KM_NUMLOCK;
    if (GetKeyState(VK_SCROLL) > 0)
        modifier_state |= Rocket::Core::Input::KM_SCROLLLOCK;

#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE

    UInt32 key_modifiers = GetCurrentEventKeyModifiers();
    if (key_modifiers & (1 << alphaLockBit))
        modifier_state |= Rocket::Core::Input::KM_CAPSLOCK;

#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX

    XKeyboardState keyboard_state;
    XGetKeyboardControl(DISPLAY!, &keyboard_state);

    if (keyboard_state.led_mask & (1 << 0))
        modifier_state |= Rocket::Core::Input::KM_CAPSLOCK;
    if (keyboard_state.led_mask & (1 << 1))
        modifier_state |= Rocket::Core::Input::KM_NUMLOCK;
    if (keyboard_state.led_mask & (1 << 2))
        modifier_state |= Rocket::Core::Input::KM_SCROLLLOCK;

#endif

    return modifier_state;
}