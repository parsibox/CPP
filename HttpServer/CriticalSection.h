#ifndef _CRITICALSECTION_H_
#define _CRITICALSECTION_H_

#ifdef	__LINUX__
	#include <pthread.h>
	#include <sys/time.h>
#else
	#include "Windows.h"
#endif

class  CCriticalSection  
{
private:

#ifdef	__LINUX__
	pthread_mutex_t	m_oCriticalRegion;
	pthread_cond_t m_oConditionalVariable;
	pthread_mutex_t m_oEvent;
#else
	void *pme_Handle;
	CRITICAL_SECTION m_oCriticalRegion;		
#endif

public:
	CCriticalSection();
	virtual ~CCriticalSection();
	void Unlock();
	void Lock();
};
#endif
