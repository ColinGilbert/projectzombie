#include "gui/GuiController.h"
#include "gui/RenderInterfaceOgre3D.h"
#include "gui/SystemInterfaceOgre3D.h"
#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <Rocket/Debugger.h>
#include <Rocket/Core/RenderInterface.h>

using namespace ZGame::Gui;

GuiController::GuiController() : _data_path(""), FONT_PATH("")
{
    _fontStrVec.push_back("Delicious-Roman.otf");
    _fontStrVec.push_back("Delicious-Bold.otf");
    _fontStrVec.push_back("Delicious-Italic.otf");
    _fontStrVec.push_back("Delicious-BoldItalic.otf");
}

GuiController::~GuiController()
{
}

bool
    GuiController::onInit(ZGame::ZInitPacket initPacket)
{
    using Ogre::ResourceGroupManager;
    Ogre::Log::Stream debug = Ogre::LogManager::getSingleton().getLog("App.log")->stream(Ogre::LML_TRIVIAL);
    Ogre::Log::Stream log = Ogre::LogManager::getSingleton().getLog("App.log")->stream();

    debug << "in GuiController onInit\n";

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

        for(std::vector<Rocket::Core::String>::const_iterator iter = _fontStrVec.cbegin(); iter != _fontStrVec.end(); ++iter)
        {
            Rocket::Core::FontDatabase::LoadFontFace(_data_path + FONT_PATH + (*iter));
        }

        context = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(_W_WIDTH, _W_HEIGHT));
        Rocket::Debugger::Initialise(context);

        //Load docs.
        Rocket::Core::ElementDocument* cursor = context->LoadMouseCursor(_data_path + "cursor.rml");
        if(cursor)
            cursor->RemoveReference();

        Rocket::Core::ElementDocument* document = context->LoadDocument(_data_path + "demo.rml");

        if(document)
        {
            document->Show();
            document->RemoveReference();
        }
        else
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Document home.rml does not exist!",
            "GuiController::onInit");


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

    context->RemoveReference();
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
        context->Update();
        ConfigureRenderSystem();
        context->Render();
    }
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