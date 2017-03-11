/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  File name					:: CriticalSection.cpp
//  Purpose				      	:: Implementation of CriticalSection class.
//
// Classes included				:: 
//		 Name	                        Purpose
//		-------                         -----------
//		CriticalSection				Locking/Unlocking Critical Sections
//
//   Development History		::
// 
//   Date					Author                      Description
//   -----------			-----------------------		--------------------------------------------------------------     
//	08/08/2003 12:00:00		Sahoo TM			Created.													
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CriticalSection.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Name 			:: CCriticalSection::CCriticalSection
//
//  Purpose      	:: Constructor, Initializes the CriticalSection
//
//  Input          	:: NONE
//
//  Output			:: Critical Section Entered
//
//  Return Value	:: NONE
//
//	Notes			::
//
//	Modifications	::	Date					Author				Description
//						------					-------------		----------------
//						08/08/2003 12:00:00		Sahoo TM			Created.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCriticalSection::CCriticalSection()
{
#ifdef	__LINUX__
	pthread_mutex_init(&m_oCriticalRegion, NULL);
        pthread_cond_init(&m_oConditionalVariable, NULL);
        pthread_mutex_init(&m_oEvent, NULL);
#else
	pme_Handle = CreateEvent(NULL,FALSE,FALSE,NULL);
	InitializeCriticalSection(&m_oCriticalRegion);
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Name 			:: CCriticalSection::~CCriticalSection
//
//  Purpose      	:: Desstructor, Deletes the Critical Section
//
//  Input          	:: NONE
//
//  Output			:: Critical section left
//
//  Return Value	:: NONE
//
//	Notes			::
//
//	Modifications	::	Date					Author				Description
//						------					-------------		----------------
//						08/08/2003 12:00:00		Sahoo TM			Created.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCriticalSection::~CCriticalSection()
{
#ifdef	__LINUX__
	pthread_mutex_destroy(&m_oCriticalRegion);
	pthread_mutex_destroy(&m_oEvent);
	pthread_cond_destroy(&m_oConditionalVariable);

#else
	CloseHandle(pme_Handle);
	DeleteCriticalSection(&m_oCriticalRegion);
#endif
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// noninline Member Function
///	Lock function allows an object to Enter a Critical Section
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCriticalSection::Lock()
{
#ifdef	__LINUX__
	pthread_mutex_lock(&m_oCriticalRegion);


#else
	EnterCriticalSection(&m_oCriticalRegion);

#endif

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// noninline Member Function
///	UnLock function allows an object to Leave a Critical Section
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCriticalSection::Unlock()
{
#ifdef	__LINUX__
	pthread_mutex_unlock(&m_oCriticalRegion);

#else
	LeaveCriticalSection(&m_oCriticalRegion);
#endif
}

int CCriticalSection::mcfn_WaitForSingleObject(signed int sL_Milliseconds)
{
	#ifdef __LINUX__
	
		if(sL_Milliseconds  == UK_UNLIMITED)
		{
			pthread_mutex_lock(&m_oEvent);
			pthread_cond_wait(&m_oConditionalVariable,&m_oEvent);
			pthread_mutex_unlock(&m_oEvent);
			return UK_RET_GOTOBJECT;
		}
		else
		{
			  struct timeval time_now;
			  struct timespec timeout;
			  int l_retcode;

			  pthread_mutex_lock(&m_oEvent);
			  gettimeofday(&time_now,NULL);
			  timeout.tv_sec = time_now.tv_sec + (sL_Milliseconds/1000);
			  long int _ln = ((time_now.tv_usec) + ((sL_Milliseconds%1000)*1000)) *1000;
			  if(_ln >= 1000000000)
			  {
				  timeout.tv_nsec = _ln-1000000000;
				  timeout.tv_sec++;
			  }
			  else
				  timeout.tv_nsec = _ln;

			  l_retcode = 0;
			  l_retcode = pthread_cond_timedwait(&m_oConditionalVariable,&m_oEvent, &timeout);
  			  pthread_mutex_unlock(&m_oEvent);	          
			  if (l_retcode == ETIMEDOUT) 
			  {
				  /* timeout occurred */
				return UK_RET_TIMEOUT;
			  }
			  else if(l_retcode == 0)
			  {
				 return UK_RET_GOTOBJECT;
			  }
			  else
				 return UK_FAILURE;

		}
	#else
		long returnVal;
		returnVal = WaitForSingleObject(pme_Handle,sL_Milliseconds);
		switch(returnVal)
		{
		case WAIT_TIMEOUT:
			return UK_RET_TIMEOUT;
		case WAIT_ABANDONED:
			return UK_FAILURE;
		case WAIT_OBJECT_0:
			return UK_RET_GOTOBJECT;
		}
	#endif
		return UK_FAILURE;
}

int CCriticalSection::mcfn_SetEvent()
{
	#ifdef __LINUX__
		int l_retval;
		pthread_mutex_lock(&m_oEvent);
        	l_retval = pthread_cond_signal(&m_oConditionalVariable);
		pthread_mutex_unlock(&m_oEvent);
		if(l_retval == 0)
			return UK_SUCCESS;
		else
			return UK_FAILURE;
	#else
		return SetEvent(pme_Handle);
	#endif
}
