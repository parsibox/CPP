/*
 * signalHandler.cpp
 *
 *  Created on: Feb 5, 2013
 *      Author: surya
 */
#include <signal.h>
#include <errno.h>
#include <sstream>
#include "signalHandler.h"

/**
* Default Contructor.
*/
CSignalHandler::CSignalHandler()
{
	mcfn_setupSignalHandlers();
}

/**
* Destructor.
*/
CSignalHandler::~CSignalHandler()
{
}

void CSignalHandler::mcfnS_exitSignalHandler(int _ignored)
{
	sigset_t signal_set;
	sigaddset(&signal_set,_ignored);
	pthread_sigmask(SIG_BLOCK, &signal_set, NULL);
	std::stringstream CL_stream;
	CL_stream.str("");
	CL_stream<<"Signal "<<strsignal(_ignored)<<"["<<_ignored<<"] came";
	pthread_sigmask(SIG_UNBLOCK, &signal_set, NULL);
	throw CSignalException(CL_stream.str());
}

void CSignalHandler::mcfn_setupSignalHandlers()
{
	std::stringstream CL_stream;
	for(int i=1;i<=31;i++){
		if (signal(i, CSignalHandler::mcfnS_exitSignalHandler) == SIG_ERR)
		{
        		CL_stream.str("");
			CL_stream<<"Signal "<<strsignal(i)<<"["<<i<<"],can not be handled";
			std::cerr << CL_stream.str()<<std::endl;
			//throw CSignalException(CL_stream.str());
		}
	}
}
          
