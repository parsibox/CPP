#include "CriticalSection.h"

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



void CCriticalSection::Lock()
{
#ifdef	__LINUX__
	pthread_mutex_lock(&m_oCriticalRegion);


#else
	EnterCriticalSection(&m_oCriticalRegion);

#endif

}

void CCriticalSection::Unlock()
{
#ifdef	__LINUX__
	pthread_mutex_unlock(&m_oCriticalRegion);

#else
	LeaveCriticalSection(&m_oCriticalRegion);
#endif
}
