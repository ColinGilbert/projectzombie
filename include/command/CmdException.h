#pragma once
/**
* \file THis file defines a Command Exception class.
*
**/
#include "ZException.h"

namespace ZGame
{
    namespace COMMAND
    {
        class CmdException : public ZGame::Exception
        {
        public:
            CmdException(std::string str) : ZGame::Exception(str)
            {
            }

	  virtual ~CmdException() throw(){}
        };
    }
}
