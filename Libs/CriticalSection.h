/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  File name					:: CriticalSection.h
//  Purpose				      	:: Interface for CCriticalSection class.
//
//  Classes included				:: 
//		 Name	                        Purpose
//		-------                         -----------
//		CCriticalSection				Locking/Unlocking Critical Sections
//
//  Development History		::
// 
//  Date					Author                      Description
//  -----------			-----------------------		--------------------------------------------------------------     
//	08/08/2003:11:00:00		Sahoo TM					Created													
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CRITICALSECTION_H_
#define _CRITICALSECTION_H_

#ifdef	__LINUX__
	#include <pthread.h>
	#include <sys/time.h>
	#ifndef ETIMEDOUT 
		#define ETIMEDOUT       70               // Operation timed out
	#endif
#else
	#include "Windows.h"
#endif



#define UK_UNLIMITED -1
#define UK_FAILURE 0
#define UK_SUCCESS 1
#define UK_RET_TIMEOUT  2
#define UK_RET_GOTOBJECT 3
//#define ETIMEDOUT */


class  CCriticalSection  
{
private:

#ifdef	__LINUX__
	pthread_mutex_t	m_oCriticalRegion;
	pthread_cond_t m_oConditionalVariable;
	pthread_mutex_t m_oEvent;
#else
	void *pme_Handle;
	CRITICAL_SECTION m_oCriticalRegion;		///Member variable for Critical Section
#endif

public:
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// A Constructor 
///	For Entring a Critical Section
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CCriticalSection();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// A Destructor 
///	For Terminating a Critical Section
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual ~CCriticalSection();
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Unlocks the critical Section
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Unlock();///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Locks the critical Section
	void Lock();
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//      Wait for single object
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int mcfn_WaitForSingleObject(int sL_MilliSeconds);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Setting the event;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int mcfn_SetEvent();


};



#endif
