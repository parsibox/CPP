#ifndef __STSOSUTIL_H
#define __STSOSUTIL_H

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

/**
 * Color Effects.
 */
#ifdef __LINUX__
#define STS_RESET                                                           0
#define STS_BRIGHT                                                          1
#define STS_DIM                                                             2
#define STS_UNDERLINE                                                       3
#define STS_BLINK                                                           4
#define STS_REVERSE                                                         7
#define STS_HIDDEN                                                          8

#define STS_BLACK                                                           0
#define STS_RED                                                             1
#define STS_GREEN                                                           2
#define STS_YELLOW                                                          3
#define STS_BLUE                                                            4
#define STS_MAGENTA                                                         5
#define STS_CYAN                                                            6
#define STS_WHITE                                                           7

#else
#define STS_BRIGHT                                                          FOREGROUND_INTENSITY
#define STS_BLACK                                                           0
#define STS_RED                                                             FOREGROUND_RED
#define STS_GREEN                                                           FOREGROUND_GREEN
#define STS_YELLOW                                                          FOREGROUND_YELLOW
#define STS_BLUE                                                            FOREGROUND_BLUE
#define STS_WHITE                                                           15

#endif
#include <string>

class CSTSOSUtil
{
public:
	static void mcfn_sleep(int seconds); //in seconds
	static void mcfn_usleep(int microseconds); //in microseconds
	static void mcfn_msleep(int milliseconds); //in milliseconds
	static int mcfn_strcasecmp(const char *s1, const char *s2);
	static int mcfn_strncasecmp(const char *s1, const char *s2,int len);
	static void mcfn_textColorEffect(int slL_Attr, int slL_ForeGrd, int slL_BkGrd);
	static int  mcfn_createProcess(char *,char *,bool = false);
	static bool mcfn_terminateProcess(int);
	static void mcfn_clear();
	static long mcfn_getpid();
	static void mcfn_endThread(void *retval = 0);
	static int mcfn_killThread(THREAD_HANDLE &hProcess,THREAD_EXITCODE exitcode=0);
	static int mcfn_joinThread(THREAD_HANDLE hProcess,void ** vL_Val);
	static int mcfn_getFileSizeGB(const char*);
	static int mcfn_getFileSizeMB(const char*);
	static int mcfn_getFileSizeKB(const char*);
	static int mcfn_getFileSizeBY(const char*);
#ifdef __LINUX__
	static int mcfn_initAttr(pthread_attr_t &attr);
	static int mcfn_destroyAttr(pthread_attr_t &attr);
	static long mcfn_beginThread(pthread_attr_t *attr,void *(*func_ptr)(void *),void *);
	static long mcfn_beginThread(void *(*func_ptr)(void *),void *);
	static long mcfn_beginThread(THREAD_HANDLE &,void *(*func_ptr)(void *),void *);
	static long mcfn_beginThread(THREAD_HANDLE &,pthread_attr_t *attr,void *(*func_ptr)(void *),void *);
	static long mcfn_beginAndDetachThread(void *(*func_ptr)(void *),void *);
	static long mcfn_beginAndDetachThread(pthread_attr_t *attr,void *(*func_ptr)(void *),void *);
	static long mcfn_beginAndDetachThread(THREAD_HANDLE &,void *(*func_ptr)(void *),void *);
	static long mcfn_beginAndDetachThread(THREAD_HANDLE &,pthread_attr_t *attr,void *(*func_ptr)(void *),void *);
	static long long mcfn_atoll(const char *nptr);
	static int mcfn_getMilliCount();
	static int mcfn_GetMilliSpan( int nTimeStart );	
	//Added by surya ,This alternative for mktime 
	static time_t mcfn_timeTolong ( const struct tm *psL_ltm, float fL_utcdiff=-5.5 );
	//Added by sunil
	//@param: L_CsvOfCpuIdx Comma separated list of cpu index as mentioned in output of top command.
	//@return: TRUE if sucess other wise false and L_Err is set
	static bool mcfn_SetCoresToUse(const std::string& L_CsvOfCpuIdx, std::string &L_Err);
	static bool mcfn_SetCoresToCurrThread(const std::string& L_CsvOfCpuIdx, std::string &L_Err);
#else
	static long mcfn_beginThread(void (*func_ptr)(void *),void *);
	static long mcfn_beginThread(THREAD_HANDLE &,void (*func_ptr)(void *),void *);
	static long mcfn_beginAndDetachThread(void (*func_ptr)(void *),void *);
	static long mcfn_beginAndDetachThread(THREAD_HANDLE &,void (*func_ptr)(void *),void *);
	static __int64 mcfn_atoll(const char *nptr);
#endif

};

#endif

