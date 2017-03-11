/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  File name									:: MsgQue.h
//  Purpose				      					:: Ineterface for CMsgQue class.
//
//  Classes included							:: 
//           Name									 Purpose
//	        -------									-----------
//			CMsgQue								To store the Messages
//
//  Development History						     ::
// 
//   Date          Author								Description
//   -----------   -----------------------			--------------------------------------------------------------     
//   08/08/2003        Sahoo TM						Created.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_MSGLIST_H__966EAF0A_4F62_4EEE_B30E_6D6348638594__INCLUDED_)
#define AFX_MSGLIST_H__966EAF0A_4F62_4EEE_B30E_6D6348638594__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CriticalSection.h"
#include <list>

#ifdef __LINUX__
#include <pthread.h>
#include <error.h>
#include <sys/time.h>
#endif

template <class T>
class CMsgQue  
{
public:
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// A Constructor 
///	For initializing the message list
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CMsgQue();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// A Destructor for message list
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual ~CMsgQue();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Push messages into the Que
///	@param  Reference to Message of type T
/// @return bool, the result (success/Failure)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool mcfb_insertIntoQue (T &);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Pop the next message from Queue 
/// It is both Blocking or non Blocking based on parameter
/// Blocking will wait until it has an element , Non Blocking will return even if Q is empty
///	@param  element Reference to Message of type T
/// @return bool, the result (success/Failure)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool mcfb_getNextMessage(T &element,bool bL_Blocking = false);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Get the next message from Queue
///	@param  element Reference to Message of type T
/// @return bool, the result (success/Failure)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool mcfb_readTopMessage(T &element);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Get the Number of messages in the Queue
/// @return int, the result (success/Failure)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int mcfb_getCount();
//////////////////////////////////////////////////////////////
//clear the Que
///////////////////////////////////////////////
	void mcf_clearQue()
	{
		meC_List.clear();
		mesi_CurrCnt = 0;
	}

///////////////////////////////////////
//Is elment exists
///////////////////////////////////////
	bool mcfn_IsAlreadyExists(T &);
	
private:
	CCriticalSection meC_LockObj;	/// Variable for Critical Section
	std::list<T> meC_List;			/// Que for storing SS7 messages 
	int mesi_CurrCnt;
#ifdef __LINUX__
	pthread_cond_t	mei_Handle;
	pthread_mutex_t meS_Mutex;
#else

	HANDLE mei_Handle;
#endif
};

template <class T>
bool CMsgQue<T>::mcfn_IsAlreadyExists(T& CL_Obj){
	bool bL_Return = false;
#ifdef __LINUX__

	pthread_mutex_lock(&meS_Mutex);
#else	
	meC_LockObj.Lock();
#endif
	typename std::list<T>::iterator pCL_Itr;
	for(pCL_Itr=meC_List.begin();pCL_Itr!=meC_List.end();++pCL_Itr){
		if((*pCL_Itr)==CL_Obj){
			bL_Return = true;
			break;
		}
	}

#ifdef __LINUX__

        if( pthread_cond_signal(&mei_Handle) != 0 )
        {
                perror("Inserting failed..\n");
                pthread_exit(NULL);
        }
        pthread_mutex_unlock(&meS_Mutex);

#else
        SetEvent(mei_Handle);
        meC_LockObj.Unlock();

#endif
	return bL_Return;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Name 			:: CMsgQue:CMsgQue()
//
//  Purpose      	:: Constructor
//
//  Input          	:: NONE
//
//  Output			:: NONE
//
//  Return Value	:: NONE
//
//	Notes			::
//
//	Modifications	::	Date					Author				Description
//						------					-------------		----------------
//						08/08/2003 12:00:00		Sahoo TM			Created.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
CMsgQue<T>::CMsgQue()
{
	mesi_CurrCnt = 0;
#ifdef __LINUX__
	//pthread_condattr_t attr = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
	pthread_cond_init(&mei_Handle, NULL);
	//mei_Handle =  PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
	pthread_mutex_init(&meS_Mutex, NULL);

#else
	mei_Handle = CreateEvent(NULL,FALSE,FALSE,NULL);

#endif
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Name 		:: CMsgQue::~CMsgQue()
//
//  Purpose      	:: Destructor. 
//
//  Input          	:: NONE
//
//  Output		:: NONE
//
//  Return Value	:: NONE
//
//	Notes		::
//
//	Modifications	::Date				Author			Description
//			------				-------------		----------------
//			08/08/2003 12:00:00		Sahoo TM		Created.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
CMsgQue<T>::~CMsgQue()
{
#ifdef __LINUX__
	pthread_cond_destroy(&mei_Handle);
	pthread_mutex_destroy(&meS_Mutex);
#endif	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Name 		:: CMsgQue:: mcfb_insertIntoQue
//
//  Purpose      	:: Addds an element (T) to the list
//
//  Input          	:: 1. T object Reference
//
//  Output		:: NONE
//
//  Return Value	:: Bool (success/failure) of the Operation
//
//	Notes		::
//	Modifications	::Date			Author				Description
//			------			-------------		----------------
//			08/08/2003 12:00:00		Sahoo TM			Created.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
bool CMsgQue<T>::mcfb_insertIntoQue (T &element)
{
#ifdef __LINUX__

	pthread_mutex_lock(&meS_Mutex);

	meC_List.push_back(element);
	mesi_CurrCnt++;
   if(pthread_cond_broadcast(&mei_Handle) != 0 )//Added by surya
//	if( pthread_cond_signal(&mei_Handle) != 0 )
	{
		perror("Inserting failed..\n");
		pthread_exit(NULL);
	}
	pthread_mutex_unlock(&meS_Mutex);

#else
	meC_LockObj.Lock();
	meC_List.push_back(element);
	SetEvent(mei_Handle);
	mesi_CurrCnt++;
	meC_LockObj.Unlock();

#endif
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Name 			:: CMsgQue:: mcfb_getNextMessage
//
//  Purpose      	:: Pops an element (T) if any from the list
//
//  Input          	:: 1. T object Reference
//
//  Output			:: NONE
//
//  Return Value	:: Bool (success/failure) of the Operation
//
//	Notes			::
//	Modifications	::	Date					Author				Description
//						------					-------------		----------------
//						08/08/2003 12:00:00		Sahoo TM			Created.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
bool CMsgQue<T>::mcfb_getNextMessage(T &element,bool bL_Blocking)
{
#ifdef __LINUX__

	pthread_mutex_lock(&meS_Mutex);
	bool lb_Flag = false;
	int  count = 0;	

	if(bL_Blocking)
	{
		//count = meC_List.size();
		if (meC_List.empty())
		{
			pthread_cond_wait(&mei_Handle,&meS_Mutex);
			//count = meC_List.size();
			if(meC_List.empty())
			{
				mesi_CurrCnt=0;
				pthread_mutex_unlock(&meS_Mutex);
				return false;		
			}
			element = meC_List.front();
			meC_List.erase(meC_List.begin());
			mesi_CurrCnt--;
			pthread_mutex_unlock(&meS_Mutex);
			return true;		
		}
			element = meC_List.front();
			meC_List.erase(meC_List.begin());
			mesi_CurrCnt--;
			pthread_mutex_unlock(&meS_Mutex);
			return true;
	}
	else
	{
		if(!meC_List.empty())
		{
			element = meC_List.front();
			meC_List.erase(meC_List.begin());
			mesi_CurrCnt--;
			lb_Flag = true;
		}
		pthread_mutex_unlock(&meS_Mutex);
		return lb_Flag;
	
	}

#else
	bool lb_Flag = false;
	meC_LockObj.Lock();

	if (meC_List.size() == 0)
	{
		meC_LockObj.Unlock();
		if(bL_Blocking)
		{
			WaitForSingleObject(mei_Handle,INFINITE);
			lb_Flag = true;
			meC_LockObj.Lock();
		}
		else
			return false;
	}
	element = meC_List.front();
	meC_List.erase(meC_List.begin());
	mesi_CurrCnt--;
	if (meC_List.size() == 0)
	{
		WaitForSingleObject(mei_Handle,0);
	}

	else
	{
		if(lb_Flag)
		SetEvent(mei_Handle);
	}
	meC_LockObj.Unlock();
	return true;
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Name 			:: CMsgQue:: mcfb_readTopMessage
//
//  Purpose      	:: Gets an element (T) if any from the list
//
//  Input          	:: 1. T object Reference
//
//  Output			:: NONE
//
//  Return Value	:: Bool (success/failure) of the Operation
//
//	Notes			::
//	Modifications	::	Date					Author				Description
//						------					-------------		----------------
//						08/08/2003 12:00:00		Sahoo TM			Created.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
bool CMsgQue<T>::mcfb_readTopMessage(T &element)
{
	meC_LockObj.Lock();

	if (meC_List.empty())
	{

		meC_LockObj.Unlock();
		return false;
	}
	else
	{
		element = meC_List.front();
		
	}

	meC_LockObj.Unlock();
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Name 			:: CMsgQue:: mcfb_getCount
//
//  Purpose      	:: Counts the number of messages in the list
//
//  Input          	:: None
//
//  Output			:: Size of the message list
//
//  Return Value	:: Integer
//
//	Notes			::
//	Modifications	::	Date					Author				Description
//						------					-------------		----------------
//						08/08/2003 12:00:00		Sahoo TM			Created.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
int CMsgQue<T>::mcfb_getCount()
{
	/*pthread_mutex_lock(&meS_Mutex);
	int li_size = mesi_CurrCnt; //meC_List.size();
	pthread_mutex_unlock(&meS_Mutex);
	return li_size;*/
	return mesi_CurrCnt;
}



#endif // !defined(AFX_MSGLIST_H__966EAF0A_4F62_4EEE_B30E_6D6348638594__INCLUDED_)
