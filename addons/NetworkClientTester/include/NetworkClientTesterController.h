#ifndef _ZGAME_NETWORK_CLIENT_TEST_CTRL_H
#define _ZGAME_NETWORK_CLIENT_TEST_CTRL_H

#include <memory>
#include <vector>
#include <Ogre.h>

#include <MainController.h>


namespace ZGame
{
    class NetworkClientTesterController : public MainController
    {
    public:
        NetworkClientTesterController();
        virtual
            ~NetworkClientTesterController();
        //implements virtual method for MainController
        virtual bool
            onInit();
        virtual void
            run();
        virtual void
            onDestroy();
    protected:
    private:
    };
}

#endif