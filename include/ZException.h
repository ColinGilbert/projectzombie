#pragma once
/**
* \file THis file defines a super class for ZGame exceptions.
*
*/
#include <exception>
#include <string>
namespace ZGame
{
    class Exception : public std::exception
    {
    public:
        Exception(std::string str) : std::exception(str.c_str())
        {
        }
        Exception(std::exception except) : std::exception(except)
        {
        }
        virtual ~Exception(){}
    protected:
    private:
    };
}