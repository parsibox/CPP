#include "STSOSUtil.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <sys/stat.h>
#include <sys/timeb.h>
#include <signal.h>
#include "File_Interpretor.h"
#include <errno.h>
#include <sched.h>

#ifdef __LINUX__

bool mcfn_SetCoresToCurrThread(const std::string& L_CsvOfCpuIdx, std::string &L_Err){
	int siL_nCpu = sysconf(_SC_NPROCESSORS_ONLN);
	if(siL_nCpu<0){
		L_Err.assign("Error: No CPU is online");
		return false;
	}
	char pscL_Str[100]={0x00};
	std::vector<std::string> L_Fields;
	CFileInterpretor::mcfn_GetFields(L_CsvOfCpuIdx, L_Fields, ",", true);
	cpu_set_t L_Cpu;
	CPU_ZERO(&L_Cpu);
	if(L_Fields.size()<1){
		L_Err.assign("Invalid list of cpu core index");
		return false;
	}
	for(int i=0;i<L_Fields.size();++i){
		int siL_CoreIdx = atoi(L_Fields[i].c_str());
		if(siL_CoreIdx<0 || siL_CoreIdx>=siL_nCpu){
			sprintf(pscL_Str, 
			"Provided cpu core index[%d] is not available: Total "
			"available cores are:%d, Returning\n", siL_CoreIdx, siL_nCpu);
			L_Err.assign(pscL_Str);
			return false;
		}
		CPU_SET(siL_CoreIdx, &L_Cpu);
	}
/*	if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &L_Cpu) <0) {
		sprintf(pscL_Str, "Error in setting cores: %d-%s", errno, strerror(errno));
		L_Err.assign(pscL_Str);
		return false;
	}*/
	sprintf(pscL_Str,"Successfully assigned the cores: %s", L_CsvOfCpuIdx.c_str());
	L_Err.assign(pscL_Str);
	return true;

}
time_t CSTSOSUtil::mcfn_timeTolong ( const struct tm *psL_ltm, float fL_utcdiff ) {
	const int piL_MonthDays [] =
	{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	long lL_TotalYears, lL_TotalDays, lL_leaps;
	float  fL_UTCHrs;
	int iL_i=0;
	lL_TotalYears = psL_ltm->tm_year - 70 ; // tm->tm_year is from 1900.
	lL_leaps = ((lL_TotalYears + 2) / 4)-(( psL_ltm->tm_mon<2&&(1900+psL_ltm->tm_year)%4==0 )?1:0);//Finding number of leap days upto given date
	iL_i = (psL_ltm->tm_year -100) / 100;//This is for getting the leap years after 2100 
	lL_leaps -= ( (iL_i/4)*3 + iL_i%4 );
	lL_TotalDays = 0;
	for (iL_i=0; iL_i < psL_ltm->tm_mon; iL_i++) lL_TotalDays += piL_MonthDays[iL_i];//Adding the days upto given month - 1

	lL_TotalDays += psL_ltm->tm_mday-1; // days of month passed.
	lL_TotalDays = lL_TotalDays + (lL_TotalYears * 365) + lL_leaps;//Finding total number of days

	fL_UTCHrs = psL_ltm->tm_hour + fL_utcdiff; // for your time zone.India time zone is:-5.5
	return (lL_TotalDays * 86400) + (int)(fL_UTCHrs * 3600) + (psL_ltm->tm_min * 60) + psL_ltm->tm_sec;
}



bool CSTSOSUtil::mcfn_SetCoresToUse(const std::string &L_CsvOfCpuIdx, std::string &L_Err){
    int siL_nCpu = sysconf(_SC_NPROCESSORS_ONLN);
    if(siL_nCpu<0){
        L_Err.assign("Error: No CPU is online");
        return false;
    }
    char pscL_Str[100]={0x00};
    std::vector<std::string> L_Fields;
    CFileInterpretor::mcfn_GetFields(L_CsvOfCpuIdx, L_Fields, ",", true);
    cpu_set_t L_Cpu;
    CPU_ZERO(&L_Cpu);
    if(L_Fields.size()<1){
        L_Err.assign("Invalid list of cpu core index");
        return false;
    }
    for(int i=0;i<L_Fields.size();++i){
        int siL_CoreIdx = atoi(L_Fields[i].c_str());
        if(siL_CoreIdx<0 || siL_CoreIdx>=siL_nCpu){
            sprintf(pscL_Str, "Provided cpu core index[%d] is not available: Total available cores are:%d, Returning\n", siL_CoreIdx, siL_nCpu);
            L_Err.assign(pscL_Str);
            return false;
        }
        CPU_SET(siL_CoreIdx, &L_Cpu);
    }
    int siL_Ret= sched_setaffinity(getpid(), sizeof(cpu_set_t), &L_Cpu);
    if(siL_Ret==-1){
        sprintf(pscL_Str, "Error in setting cores: %d-%s", errno, strerror(errno));
        L_Err.assign(pscL_Str);
        return false;
    }
    sprintf(pscL_Str,"Successfully assigned the cores: %s", L_CsvOfCpuIdx.c_str());
    L_Err.assign(pscL_Str);
    return true;

}


void CSTSOSUtil::mcfn_clear()
{
	system("clear");
}

void CSTSOSUtil::mcfn_sleep(int siL_Duration)
{
	sleep(siL_Duration);
}

void CSTSOSUtil::mcfn_usleep(int microseconds)
{	
	usleep(microseconds);
}
void CSTSOSUtil::mcfn_msleep(int milliseconds) //in milliseconds
{
	usleep(milliseconds*1000);
}

int CSTSOSUtil::mcfn_initAttr(pthread_attr_t &attr)
{
	int siL_Ret = 0;
	if((siL_Ret=pthread_attr_init(&attr)) !=0)
                return siL_Ret;
        if((siL_Ret=pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED)) !=0)
                return siL_Ret;
        if((siL_Ret=pthread_attr_setinheritsched(&attr,PTHREAD_INHERIT_SCHED)) !=0)
                return siL_Ret;
	return 0;
}
int CSTSOSUtil::mcfn_destroyAttr(pthread_attr_t &attr)
{
	return (pthread_attr_destroy(&attr));
}
long CSTSOSUtil::mcfn_beginThread(void *(*func_ptr)(void *),void *pArgs)
{
	THREAD_HANDLE hThread;
	int siL_retValue = -1;
	siL_retValue = pthread_create((THREAD_HANDLE *)&hThread,NULL,func_ptr,pArgs);
	return siL_retValue;
}
long CSTSOSUtil::mcfn_beginThread(pthread_attr_t *attr,void *(*func_ptr)(void *),void *pArgs)
{
	THREAD_HANDLE hThread;
	int siL_retValue = -1;
	siL_retValue = pthread_create((THREAD_HANDLE *)&hThread,attr,func_ptr,pArgs);
	return siL_retValue;
}
long CSTSOSUtil::mcfn_beginThread(THREAD_HANDLE &hThread,void *(*func_ptr)(void *),void *pArgs)
{
	return pthread_create((THREAD_HANDLE *)&hThread,NULL,func_ptr,pArgs);
}
long CSTSOSUtil::mcfn_beginThread(THREAD_HANDLE &hThread,pthread_attr_t *attr,void *(*func_ptr)(void *),void *pArgs)
{
	return pthread_create((THREAD_HANDLE *)&hThread,attr,func_ptr,pArgs);
}

long CSTSOSUtil::mcfn_beginAndDetachThread(void *(*func_ptr)(void *),void *pArgs)
{
	THREAD_HANDLE hThread;
	long siL_retValue = -1;
	siL_retValue = pthread_create((THREAD_HANDLE *)&hThread,NULL,func_ptr,pArgs);
	pthread_detach(hThread);
	return siL_retValue;
}
long CSTSOSUtil::mcfn_beginAndDetachThread(pthread_attr_t *attr,void *(*func_ptr)(void *),void *pArgs)
{
	THREAD_HANDLE hThread;
	long siL_retValue = -1;
	siL_retValue = pthread_create((THREAD_HANDLE *)&hThread,attr,func_ptr,pArgs);
	pthread_detach(hThread);
	return siL_retValue;
}

long CSTSOSUtil::mcfn_beginAndDetachThread(THREAD_HANDLE &hThread,void *(*func_ptr)(void *),void *pArgs)
{
	long siL_retValue = -1;
	siL_retValue = pthread_create((THREAD_HANDLE *)&hThread,NULL,func_ptr,pArgs);
	pthread_detach(hThread);
	return siL_retValue;
}
long CSTSOSUtil::mcfn_beginAndDetachThread(THREAD_HANDLE &hThread,pthread_attr_t *attr,void *(*func_ptr)(void *),void *pArgs)
{
	long siL_retValue = -1;
	siL_retValue = pthread_create((THREAD_HANDLE *)&hThread,attr,func_ptr,pArgs);
	pthread_detach(hThread);
	return siL_retValue;
}

void CSTSOSUtil::mcfn_endThread(void *retval)
{
	pthread_exit(retval);
}

int CSTSOSUtil::mcfn_killThread(THREAD_HANDLE &hProcess,THREAD_EXITCODE exitcode)
{
	return (pthread_kill(hProcess,exitcode));
}

int CSTSOSUtil::mcfn_joinThread(THREAD_HANDLE hProcess,void ** vL_Val)
{
	return pthread_join(hProcess,vL_Val);
}

long long CSTSOSUtil::mcfn_atoll(const char *nptr)
{
	return atoll(nptr);
}

int CSTSOSUtil::mcfn_strcasecmp(const char *s1, const char *s2)
{
	return strcasecmp(s1,s2);
}

int CSTSOSUtil::mcfn_strncasecmp(const char *s1, const char *s2,int len)
{
	return strncasecmp(s1,s2,len);
}

void CSTSOSUtil::mcfn_textColorEffect(int slL_Attr, int slL_ForeGrd, int slL_BkGrd)
{
	char	pscL_Cmd[30];
	sprintf(pscL_Cmd, "%c[%d;%d;%dm", 0x1B, slL_Attr, slL_ForeGrd + 30, slL_BkGrd + 40);
	fprintf(stdout, "%s", pscL_Cmd);
}

int CSTSOSUtil::mcfn_createProcess(char *pscL_ProcessString,char *pscL_ExePath,bool bL_Foreground)
{
	char pscL_SystemCmd[1000];
	if(bL_Foreground)
	{
		sprintf(pscL_SystemCmd,"%s",pscL_ProcessString);
	}
	else
	{
		sprintf(pscL_SystemCmd,"%s &",pscL_ProcessString);                                         
	}                                                                                                  
	system(pscL_SystemCmd);                                                                            
}                                                                                                          

bool CSTSOSUtil::mcfn_terminateProcess(int siL_ProcessId)                                                  
{                                                                                                          
	char pscL_SystemCmd[1000];                                                                         
	
	sprintf(pscL_SystemCmd,"kill -9 %d",siL_ProcessId);                                                
	
	system(pscL_SystemCmd);                                                                            
	
	return true;                                                                                       
}    
long CSTSOSUtil::mcfn_getpid()
{
	return(getpid());
}
int CSTSOSUtil::mcfn_getFileSizeGB(const char * pscL_File)
{
	if(pscL_File == NULL)
                return -1;
	struct stat buf;
	int siL_FileSize = 0x00;
        int ret = stat(pscL_File,&buf);
	if(ret != -1)
        {
	        siL_FileSize  = (buf.st_size)/(1024*1024*1024);
		return siL_FileSize;
	}
	else
		return -1;
}
int CSTSOSUtil::mcfn_getFileSizeMB(const char * pscL_File)
{
	if(pscL_File == NULL)
                return -1;
	struct stat buf;
	int siL_FileSize = 0x00;
        int ret = stat(pscL_File,&buf);
	if(ret != -1)
        {
	        siL_FileSize  = (buf.st_size)/(1024*1024);
		return siL_FileSize;
	}
	else
		return -1;
}
int CSTSOSUtil::mcfn_getFileSizeKB(const char * pscL_File)
{
	if(pscL_File == NULL)
                return -1;
	struct stat buf;
	int siL_FileSize = 0x00;
        int ret = stat(pscL_File,&buf);
	if(ret != -1)
        {
	        siL_FileSize  = (buf.st_size)/(1024);
		return siL_FileSize;
	}
	else
		return -1;
}
int CSTSOSUtil::mcfn_getFileSizeBY(const char * pscL_File)
{
	if(pscL_File == NULL)
                return -1;
	struct stat buf;
	int siL_FileSize = 0x00;
        int ret = stat(pscL_File,&buf);
	if(ret != -1)
        {
	        siL_FileSize  = (buf.st_size);
		return siL_FileSize;
	}
	else
		return -1;
}


int CSTSOSUtil::mcfn_getMilliCount()
{
  // Something like GetTickCount but portable
  // It rolls over every ~ 12.1 days (0x100000/24/60/60)
  // Use GetMilliSpan to correct for rollover
  timeb tb;
  ftime( &tb );
  int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
  return nCount;
}

int CSTSOSUtil::mcfn_GetMilliSpan( int nTimeStart )
{
  int nSpan = mcfn_getMilliCount() - nTimeStart;
  if ( nSpan < 0 )
    nSpan += 0x100000 * 1000;
  return nSpan;
}


#else

void CSTSOSUtil::mcfn_clear()
{
	system("cls");
}

void CSTSOSUtil::mcfn_sleep(int siL_Duration)
{
	Sleep(siL_Duration * 1000);
}

void CSTSOSUtil::mcfn_usleep(int microseconds)
{	
	if(microseconds < 1000)
		Sleep(1);
	else
		Sleep(microseconds/1000);
}
void CSTSOSUtil::mcfn_msleep(int milliseconds) //in milliseconds
{
	Sleep(milliseconds);
}

long CSTSOSUtil::mcfn_beginThread(void (*func_ptr)(void *),void *pArgs)
{
	long siL_retValue = 0;
	siL_retValue = _beginthread(func_ptr,0,pArgs);
	return siL_retValue;
}

long CSTSOSUtil::mcfn_beginThread(THREAD_HANDLE &hThread,void (*func_ptr)(void *),void *pArgs)
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
long CSTSOSUtil::mcfn_beginAndDetachThread(void (*func_ptr)(void *),void *pArgs)
{
	
	long siL_retValue = 0;
	siL_retValue = _beginthread(func_ptr,0,pArgs);
	return siL_retValue;
}

long CSTSOSUtil::mcfn_beginAndDetachThread(THREAD_HANDLE &hThread,void (*func_ptr)(void *),void *pArgs)
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

void CSTSOSUtil::mcfn_endThread(void *retval)
{
	_endthread();
}

int CSTSOSUtil::mcfn_killThread(THREAD_HANDLE &hProcess,THREAD_EXITCODE exitcode)
{
	int retval = 0;
	retval = TerminateThread(hProcess,exitcode);
	if(retval == 0)
		return GetLastError();
	else
		return 0;
}

int CSTSOSUtil::mcfn_joinThread(THREAD_HANDLE hProcess,void ** vL_Val)
{
	return 1;
}

__int64 CSTSOSUtil::mcfn_atoll(const char *nptr)
{
	return _atoi64(nptr);
}

int CSTSOSUtil::mcfn_strcasecmp(const char *s1, const char *s2)
{
	return stricmp(s1,s2);
}

int CSTSOSUtil::mcfn_strncasecmp(const char *s1, const char *s2,int len)
{
	return strnicmp(s1,s2,len);
}

void CSTSOSUtil::mcfn_textColorEffect(int slL_Attr, int slL_ForeGrd, int slL_BkGrd)
{
	HANDLE	hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, slL_Attr|slL_ForeGrd|slL_BkGrd);
}

int CSTSOSUtil::mcfn_createProcess(char *pscL_ProcessString,char *pscL_ExePath,bool bL_IsForeground)       
{                                                                                                          
	STARTUPINFO SL_StartInfo;                                                                          
	ZeroMemory(&SL_StartInfo,sizeof(SL_StartInfo));                                                    
	PROCESS_INFORMATION SL_ProcessInfo;                                                                
	
	if(bL_IsForeground)                                                                                
	{                                                                                                  
		if(!CreateProcess(NULL,pscL_ProcessString,NULL,NULL,NULL,CREATE_NEW_CONSOLE,NULL,pscL_ExePath,&SL_StartInfo,&
			SL_ProcessInfo))
			return -1;                                                                         
	}                                                                                                  
	else                                                                                               
	{                                                                                                  
		if(!CreateProcess(NULL,pscL_ProcessString,NULL,NULL,NULL,CREATE_NO_WINDOW,NULL,pscL_ExePath,&SL_StartInfo,&SL_ProcessInfo))
			return -1;                                                                         
	}                                                                                                  
	return SL_ProcessInfo.dwProcessId;                                                                 
}                                                                                                          


bool CSTSOSUtil::mcfn_terminateProcess(int siL_ProcessId)                                                  
{                                                                                                          
	char pscL_SystemCmd[1000];                                                                         
	
	sprintf(pscL_SystemCmd,"kill.exe -f %d",siL_ProcessId);                                            
	
	system(pscL_SystemCmd);                                                                            
	
	return true;                                                                                       
}
long CSTSOSUtil::mcfn_getpid()
{
	return(GetCurrentProcessId());
}
static int mcfn_getFileSizeGB(const char * pscL_File)
{
	if(pscL_File == NULL)
                return -1;
	struct _stat buf;
	int siL_FileSize = 0x00;
        int ret = _stat(pscL_File,&buf);
	if(ret != -1)
        {
	        siL_FileSize  = (buf.st_size)/(1024*1024*1024);
		return siL_FileSize;
	}
	else
		return -1;
}
static int mcfn_getFileSizeMB(const char * pscL_File)
{
	if(pscL_File == NULL)
                return -1;
	struct _stat buf;
	int siL_FileSize = 0x00;
        int ret = _stat(pscL_File,&buf);
	if(ret != -1)
        {
	        siL_FileSize  = (buf.st_size)/(1024*1024);
		return siL_FileSize;
	}
	else
		return -1;
}
static int mcfn_getFileSizeKB(const char * pscL_File)
{
	if(pscL_File == NULL)
                return -1;
	struct _stat buf;
	int siL_FileSize = 0x00;
        int ret = _stat(pscL_File,&buf);
	if(ret != -1)
        {
	        siL_FileSize  = (buf.st_size)/(1024);
		return siL_FileSize;
	}
	else
		return -1;
}
static int mcfn_getFileSizeBY(const char * pscL_File)
{
	if(pscL_File == NULL)
                return -1;
	struct _stat buf;
	int siL_FileSize = 0x00;
        int ret = _stat(pscL_File,&buf);
	if(ret != -1)
        {
	        siL_FileSize  = (buf.st_size);
		return siL_FileSize;
	}
	else
		return -1;
}

#endif
