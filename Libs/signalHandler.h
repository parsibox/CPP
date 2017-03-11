/*
 * signalHandler.h
 *
 *  Created on: Feb 5, 2013
 *      Author: surya
 */
#ifndef __SIGNALHANDLER_H__
#define __SIGNALHANDLER_H__
#include <stdexcept>
#include <iostream>
using std::runtime_error;

class CSignalException : public runtime_error
{
public:
   CSignalException(const std::string& _message)
      : std::runtime_error(_message)
   {}
};

class CSignalHandler
{
protected:
public:
    CSignalHandler();
    ~CSignalHandler();

    void        mcfn_setupSignalHandlers();
    static void mcfnS_exitSignalHandler(int _ignored);

};
#endif
          
