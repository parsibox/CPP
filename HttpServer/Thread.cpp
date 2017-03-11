#include "Thread.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <sys/stat.h>
#include <sys/timeb.h>
#include <signal.h>
#include <errno.h>
#include <sched.h>

#ifdef __LINUX__

long Thread::beginAndDetachThread(void *(*func_ptr)(void *),void *pArgs)
{
	THREAD_HANDLE hThread;
	long siL_retValue = -1;
	siL_retValue = pthread_create((THREAD_HANDLE *)&hThread,NULL,func_ptr,pArgs);
	pthread_detach(hThread);
	return siL_retValue;
}

long Thread::beginAndDetachThread(THREAD_HANDLE &hThread,void *(*func_ptr)(void *),void *pArgs)
{
	long siL_retValue = -1;
	siL_retValue = pthread_create((THREAD_HANDLE *)&hThread,NULL,func_ptr,pArgs);
	pthread_detach(hThread);
	return siL_retValue;
}
#else
long Thread::beginAndDetachThread(void (*func_ptr)(void *),void *pArgs)
{
	
	long siL_retValue = 0;
	siL_retValue = _beginthread(func_ptr,0,pArgs);
	return siL_retValue;
}

long Thread::beginAndDetachThread(THREAD_HANDLE &hThread,void (*func_ptr)(void *),void *pArgs)
{
	long siL_retValue = 0;
	if((int)(hThread = (THREAD_HANDLE) _beginthread(func_ptr,0,pArgs)) == -1)
	{
		siL_retValue = GetLastError();
		return siL_retValue;
	}
	else
		return 0;
}
#endif


