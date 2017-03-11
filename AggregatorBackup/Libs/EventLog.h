
/**
* SixDEE Telecom Solutions Pvt. Ltd.
* Copyright 2006
* All Rights Reserved.
*/

#ifndef __EVENTLOG_H
#define __EVENTLOG_H

#define CRITICAL	1 	//0x00000001 
#define LOGERROR	2 	//0x00000010 
#define INFO		4 	//0x00000100
#define VERBOSE		8 	//0x00001000 
#define LOGMSG		16	//0x00010000
#define CONSOLE		32  	//0x00100000
#define DEBUG		64  	//0x01000000


#include "stdio.h"
#include "string.h"
#include "CriticalSection.h"

#include "STSMsg.h"
#include "STSSystem.h"


#define DBG_CPDEBUG(OBJECT,PRINTDATA)  {if(OBJECT.mcu8_DBGLevel & DEBUG) {OBJECT.mcfn_Lock();OBJECT.mcfn_logCallerInfo(DEBUG,"~DG~",__FILE__,__LINE__,this); OBJECT.mcfn_logMessage PRINTDATA;OBJECT.mcfn_Unlock();}}	
#define DBG_CPVERBOSE(OBJECT,PRINTDATA)  {if(OBJECT.mcu8_DBGLevel & VERBOSE) {OBJECT.mcfn_Lock();OBJECT.mcfn_logCallerInfo(VERBOSE,"~VB~",__FILE__,__LINE__,this); OBJECT.mcfn_logMessage PRINTDATA;OBJECT.mcfn_Unlock();}}	
#define DBG_CPINFO(OBJECT,PRINTDATA)  {if(OBJECT.mcu8_DBGLevel & INFO) { OBJECT.mcfn_Lock();OBJECT.mcfn_logCallerInfo(INFO,"~IN~",__FILE__,__LINE__,this); OBJECT.mcfn_logMessage PRINTDATA;OBJECT.mcfn_Unlock();}}
#define DBG_CPERROR(OBJECT,PRINTDATA)  {if(OBJECT.mcu8_DBGLevel & LOGERROR) {OBJECT.mcfn_Lock();OBJECT.mcfn_logCallerInfo(LOGERROR,"~ER~",__FILE__,__LINE__,this); OBJECT.mcfn_logMessage PRINTDATA;OBJECT.mcfn_Unlock();}}
#define DBG_CPCRITICAL(OBJECT,PRINTDATA)  {if(OBJECT.mcu8_DBGLevel & CRITICAL) { OBJECT.mcfn_Lock();OBJECT.mcfn_logCallerInfo(CRITICAL,"~CR~",__FILE__,__LINE__,this); OBJECT.mcfn_logMessage PRINTDATA;OBJECT.mcfn_Unlock();}}

#define DBG_DEBUG(OBJECT,PRINTDATA)  {if(OBJECT.mcu8_DBGLevel & DEBUG) {OBJECT.mcfn_Lock();OBJECT.mcfn_logCallerInfo(DEBUG,"~DG~",__FILE__,__LINE__); OBJECT.mcfn_logMessage PRINTDATA;OBJECT.mcfn_Unlock();}}	
#define DBG_VERBOSE(OBJECT,PRINTDATA)  {if(OBJECT.mcu8_DBGLevel & VERBOSE) {OBJECT.mcfn_Lock();OBJECT.mcfn_logCallerInfo(VERBOSE,"~VB~",__FILE__,__LINE__); OBJECT.mcfn_logMessage PRINTDATA;OBJECT.mcfn_Unlock();}}	
#define DBG_INFO(OBJECT,PRINTDATA)  {if(OBJECT.mcu8_DBGLevel & INFO) { OBJECT.mcfn_Lock();OBJECT.mcfn_logCallerInfo(INFO,"~IN~",__FILE__,__LINE__); OBJECT.mcfn_logMessage PRINTDATA;OBJECT.mcfn_Unlock();}}
#define DBG_ERROR(OBJECT,PRINTDATA)  {if(OBJECT.mcu8_DBGLevel & LOGERROR) {OBJECT.mcfn_Lock();OBJECT.mcfn_logCallerInfo(LOGERROR,"~ER~",__FILE__,__LINE__); OBJECT.mcfn_logMessage PRINTDATA;OBJECT.mcfn_Unlock();}}
#define DBG_CRITICAL(OBJECT,PRINTDATA)  {if(OBJECT.mcu8_DBGLevel & CRITICAL) { OBJECT.mcfn_Lock();OBJECT.mcfn_logCallerInfo(CRITICAL,"~CR~",__FILE__,__LINE__); OBJECT.mcfn_logMessage PRINTDATA;OBJECT.mcfn_Unlock();}}
#define DBG_FVERBOSE(OBJECT,PRINTDATA,FILENAME,LINENO)  {if(OBJECT.mcu8_DBGLevel & VERBOSE) {OBJECT.mcfn_Lock();OBJECT.mcfn_logCallerInfo(VERBOSE,"~VB~",FILENAME,LINENO); OBJECT.mcfn_logMessage PRINTDATA;OBJECT.mcfn_Unlock();}}     
#define DBG_FDEBUG(OBJECT,PRINTDATA,FILENAME,LINENO)  {if(OBJECT.mcu8_DBGLevel & DEBUG) {OBJECT.mcfn_Lock();OBJECT.mcfn_logCallerInfo(VERBOSE,"~DG~",FILENAME,LINENO); OBJECT.mcfn_logMessage PRINTDATA;OBJECT.mcfn_Unlock();}}
#define __STS_MAX_STR_SIZE 5120
class CEventLog
{
private:
	
	CCriticalSection meC_CriticalSection;
	FILE *pmeS_File;
	char pmesc_AppTag[20];
	char pmesc_FilePath[2000];
	char pmesc_FileName[3000];
	long mesl_StartTime;
	long mesl_Duration;
	bool meb_Is2Log;
	int  mesi_DayofMonth;
	u8   meu8_SS7LOG;
	int mesi_FileCount;
	int mesi_FileSize;
	int mesi_FileMaxSize;
	char pmesc_TempBuffer[__STS_MAX_STR_SIZE];
	bool mefn_changeFileIfReq(struct tm &SL_TM,unsigned short &siL_milliSec);
	int mesi_maxNumberOfFiles;	
public:
	CEventLog();
	~CEventLog();
	u8 mcu8_DBGLevel;
	
public:
	void mcfn_setNewFileGenerationPeriod(long );
	void mcfn_setAPPTag(char *);
	void mcfn_setLogLevel(int );
	void mcfn_Lock();
	void mcfn_Unlock();
	void mcfn_openNewFile();
	void mcfn_logCallerInfo(int,char *,char *,int,void * =NULL);
	void mcfn_logMessage(const char *,...);
	void mcfn_close();
	void mcfn_setFilePath(char *);
	int  mcfn_getDBGLevel() const;
	void mcfn_logSS7Message(STSMSG_SS7 *);
	void mcfn_logByteStream(char *,unsigned char *,int);
	void mcfn_logU8Message(u8 *,int len);
	
	u8      mcfn_getSS7LoggingReq();
    void    mcfn_setSS7LoggingReq(u8);
	
	void mcfn_setMaxFileSize(int);
	int mcfn_getFileSize();
	void mcfn_setMaxNumberOfFiles(int);
};
inline void CEventLog::mcfn_setMaxNumberOfFiles (int iL_numOfFiles){
	mesi_maxNumberOfFiles=iL_numOfFiles;
}
inline
void CEventLog::mcfn_setSS7LoggingReq(u8 u8L_LogLevel)                                                     
{                                                                                                          
	meu8_SS7LOG = u8L_LogLevel;                                                                        
}                                                                                                          
inline                                                                                                     
u8 CEventLog::mcfn_getSS7LoggingReq()                                                                      
{                                                                                                          
	return meu8_SS7LOG;                                                                                
}  
inline                                                                                                     
void CEventLog::mcfn_setMaxFileSize(int siL_Val)                                                           
{                                                                                                          
	mesi_FileMaxSize = siL_Val;                                                                        
}


inline
void CEventLog::mcfn_setNewFileGenerationPeriod(long slL_Duration)
{
	mesl_Duration = slL_Duration;
}

inline
void CEventLog::mcfn_setFilePath(char *pscL_FilePath)
{
	strcpy(pmesc_FilePath,pscL_FilePath);
}

inline
int CEventLog::mcfn_getDBGLevel() const
{
	return mcu8_DBGLevel;
}

inline
void CEventLog::mcfn_setLogLevel(int siL_LogLevel)
{
	mcu8_DBGLevel = (u8) siL_LogLevel;
}

inline
void CEventLog::mcfn_setAPPTag(char *pscL_AppTag)
{
	strcpy(pmesc_AppTag,pscL_AppTag);
}

inline
void CEventLog::mcfn_Lock()

{
	meC_CriticalSection.Lock();
}

inline
void CEventLog::mcfn_Unlock()
{
	meC_CriticalSection.Unlock();
}


#endif

