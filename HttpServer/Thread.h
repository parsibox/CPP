#ifndef __THREAD__
#define __THREAD__
#ifdef __LINUX__

#include "unistd.h"
#include "pthread.h"

#define RETURN	return NULL;
#define FNVOID	void *
#define THREAD_HANDLE	pthread_t
#define THREAD_EXITCODE	int


#else

#include "Windows.h"
#include "process.h"

#define RETURN	return;
#define FNVOID	void
#define THREAD_HANDLE	HANDLE
#define THREAD_EXITCODE	DWORD

#endif


class Thread
{
 public:
	static long beginAndDetachThread(void *(*func_ptr)(void *),void *);
	static long beginAndDetachThread(THREAD_HANDLE &,void *(*func_ptr)(void *),void *);

};
#endif
