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
        Exception(std::string str) :_msg(str)
        {
            
        }
        Exception(std::exception except) : std::exception(except)
        {
        }
        virtual ~Exception() throw() {} 
        const char* what() const throw() {return _msg.c_str();} 
    protected:
    private:
        std::string _msg;
    };
}