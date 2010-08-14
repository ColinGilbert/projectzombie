#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <sstream>

using std::stringstream;
using std::string;
using std::cout;
using std::endl;
using std::cin;

//#include "boost/program_options.hpp"
//namespace po = boost::program_options;

#include <CommandController.h> //This is minimal bootstrap systems required. Note: We need to implement a class which contains all these minimal required systems.
#include <entities/EntitiesManager.h>
#include <entities/ZEntityBuilder.h>
#include <ZCL/ZCLController.h>
#include <world/WorldMap.h>
#include <command/CmdException.h>
#include "utilities/cl.hpp"
#include "world/WorldMap.h"


int main(int argc, char** argv)
{
    using ZGame::CommandController;
    using ZGame::Entities::EntitiesManager;
    using ZGame::Entities::EntitiesBuilder;
    using ZGame::COMMAND::CmdException;
    using ZGame::ZCL::ZCLController;
    using ZGame::World::WorldMap;
    CommandController* cmdCtrl = new CommandController();
    EntitiesManager* entMgr = new EntitiesManager();
    WorldMap* worldMap = new WorldMap();
    try
    {
        /*      
        po::options_description desc("Allowd options");
        desc.add_options()
        ("help", "produce help message")
        ("count", po::value<int>(), "set compression level");
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if(vm.count("help"))
        {
        cout << desc << "\n" << endl;
        return 1;
        }
        size_t numOfEnts = 1000;
        if(vm.count("count"))
        {
        numOfEnts = vm["count"].as<int>();
        }
        */
        
        size_t numOfEnts = 1000;
        
        if(argc == 2)
        {
            cout << "argv[0]" << argv[0] << "argv[1]" << argv[1] << endl;

            std::istringstream buffer(argv[1]);

            buffer >> numOfEnts;
        }
        //Start the needed service of CommandController
        cout << "Command Controller started." << endl;
        try
        {
            EntitiesBuilder::build(entMgr, numOfEnts);
        }catch(CmdException e)
        {
            cout << "Command Exception caught. Ignoring it: " << e.what() << endl; //we can ignore CmdExceptions. Since not all commands are implemented in this tester.
        }
        //convert zentities into buffer format.
        entMgr->zEntitiesToBuffer();
        cout << "ZEntities buffers created!" << endl;

        worldMap->load();
        cout << "World map loaded!" << endl;

        ZCLController ctrl;
        ctrl.init("../scripts/testkernels.cl", entMgr->getEntBuffers(), worldMap);
        cout << "Running kernel" << endl;
        ctrl.onUpdate();
        //cout << "Print time: " << endl;
        ctrl.printKernelTime();
        //cout << "Finished running kernel" << endl;

        //Test freeing memeory.


    }catch(std::exception &e)
    {
        cout << "exception caught: " << e.what() << endl;

    }
    delete worldMap;
    delete entMgr;
    delete cmdCtrl;
    return 0;
}