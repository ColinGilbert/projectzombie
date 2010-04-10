#include <iostream>

using namespace std;
#include "NetworkClientTesterController.h"

using namespace ZGame;

NetworkClientTesterController::NetworkClientTesterController()
{
}

NetworkClientTesterController::~NetworkClientTesterController()
{

}

bool 
NetworkClientTesterController::onInit()
{
    cout << "In Net client tester onInit." << endl;
    return true;
}

void
NetworkClientTesterController::run()
{
    cout << "In Net client tester run." << endl;
}

void
NetworkClientTesterController::onDestroy()
{
    cout << "In net client test onDestroy." << endl;
}