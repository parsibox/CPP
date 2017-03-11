
/**
* SixDEE Telecom Solutions Pvt. Ltd.
* Copyright 2006
* All Rights Reserved.
*/

#include <stdarg.h>
#include "EventLog.h"
#include "STSException.h"
#include <time.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/timeb.h>
#include <stdlib.h>
#define OPENFAIL	1003
#define MEMFAIL		1004
#define MAX_PARAM_LEN           320

CEventLog::CEventLog()
{
	meb_Is2Log = true;
	pmeS_File = NULL;
	strcpy(pmesc_AppTag,"_");
	strcpy(pmesc_FilePath,"./");
	memset(pmesc_FileName,0x00,sizeof(pmesc_FileName));
	mesl_Duration = 86400;
	mcu8_DBGLevel = 0x00;
	mesi_DayofMonth = 0;
	mesi_FileCount = 0;
	mesi_FileSize = 0;
	//max size of 100MB allowed now
	mesi_FileMaxSize =100;
	mesi_maxNumberOfFiles=0;
}

CEventLog::~CEventLog()
{
	if(pmeS_File != NULL)
		fclose(pmeS_File);
	pmeS_File = NULL;
}

void CEventLog::mcfn_close()
{
	meC_CriticalSection.Lock();
	if(pmeS_File != NULL)
	{
		fclose(pmeS_File);
		pmeS_File = NULL;
	}
	meC_CriticalSection.Unlock();
}

void CEventLog::mcfn_openNewFile()
{
	if((mcu8_DBGLevel & 0x1f ) == 0x00)
	{
		if((mcu8_DBGLevel & 0x40 ) == 0x00)
		{
			meb_Is2Log = false;
			return;
		}
	}
	struct tm SL_TM;

	mesl_StartTime = time(NULL);
	localtime_r(&mesl_StartTime,&SL_TM);
	mesi_DayofMonth = SL_TM.tm_mday;
	mesi_FileSize = 0;


	if(pmeS_File != NULL)
	{
		fclose(pmeS_File);
		pmeS_File = NULL;
	}

	memset(pmesc_FileName,0x00,sizeof(pmesc_FileName));
	if(mesi_maxNumberOfFiles==0){	
		sprintf(pmesc_FileName,"%s/%s_%02d_%02d_%02d_%02d_%02d_%02d_%02d.log",pmesc_FilePath,\
			pmesc_AppTag,SL_TM.tm_mday,SL_TM.tm_mon + 1,SL_TM.tm_year + 1900,\
			SL_TM.tm_hour,SL_TM.tm_min,SL_TM.tm_sec,mesi_FileCount);
	}
	else{
		if(mesi_FileCount>=mesi_maxNumberOfFiles)
		mesi_FileCount=0;
		sprintf(pmesc_FileName,"%s/%s.log.%d",pmesc_FilePath,pmesc_AppTag,mesi_FileCount);
	}
	++mesi_FileCount;
	
	pmeS_File = fopen(pmesc_FileName,"w+");
	if(pmeS_File == NULL)
	{
		char pscL_ErrStrg[300];
		memset(pscL_ErrStrg,0x00,sizeof(pscL_ErrStrg));
		sprintf(pscL_ErrStrg,"Error Opening File --> %s-%s\n",pmesc_FileName,strerror(errno));
		meb_Is2Log = false;
		pmeS_File = NULL;
		throw STSGenException(OPENFAIL,pscL_ErrStrg);
	}
}
int CEventLog::mcfn_getFileSize()                                                                         
{
	if(pmeS_File == NULL)
		return 0;                                                                                                  
#ifdef __LINUX__                                                                                           
	struct stat buf;                                                                                   
	int ret = stat(pmesc_FileName,&buf);                                                               
#else                                                                                                      
	struct _stat buf;                                                                                  
	int ret = _stat(pmesc_FileName,&buf);                                                              
#endif                                                                                                     
	if((mesi_FileSize >0) && (ret == -1))
        {
       		mesi_FileSize = 101;
		return 0; 
        }
	if(ret != -1)                                                                                      
		mesi_FileSize  = (buf.st_size)/(1024*1024);                                                 
	return buf.st_nlink;
}    
bool CEventLog::mefn_changeFileIfReq(struct tm &SL_TM,unsigned short &siL_milliSec){
	struct timeval  SL_timeVal;
	long lL_currTime=0;
	gettimeofday(&SL_timeVal,NULL);
	lL_currTime=SL_timeVal.tv_sec;
	localtime_r(&lL_currTime,&SL_TM);
	siL_milliSec=SL_timeVal.tv_usec/1000;
	try{
		if(mcfn_getFileSize()<=0){//if file is deleted
			//mesi_FileCount++;
			mcfn_openNewFile();
		}	
		/**
		 * if file duration is set(non-zero) file will be created based on duration and size
		 * else file will  be created based on change of day and size
		 **/
		if(mesl_Duration!=0){
			if((lL_currTime - mesl_StartTime) > mesl_Duration)
				mcfn_openNewFile();
			else if(mesi_FileSize >= mesi_FileMaxSize)
			{
				//mesi_FileCount++;
				mcfn_openNewFile();
			}
		}
		else if(mesi_DayofMonth != SL_TM.tm_mday)
			mcfn_openNewFile();
		else if(mesi_FileSize >= mesi_FileMaxSize)
		{
			//mesi_FileCount++;
			mcfn_openNewFile();
		}
	}
	catch(STSGenException e){
		perror(e.reasonString());
		if(e.reasonCode()==OPENFAIL){
			char pscL_createDir[2024]={0x00};
			sprintf(pscL_createDir,"mkdir -p %s",pmesc_FilePath);
			system(pscL_createDir);
		}
		return false;
	}
	return true;
}

void CEventLog::mcfn_logCallerInfo(int LogLevel,char *pscL_DBGLevel,char *pscL_FilePath,int siL_LineNo,void * pvL_ptr)
{
	try
	{
		struct tm SL_TM;
		unsigned short uiL_milliSec=0;
		if(mefn_changeFileIfReq(SL_TM,uiL_milliSec)==false)
			return;	
		
		char *pscL_FileName = strrchr(pscL_FilePath,'\\');
		if(pscL_FileName == NULL)
			pscL_FileName = pscL_FilePath;
		else
			pscL_FileName++;


		if(pvL_ptr == NULL)
		{
			fprintf(pmeS_File,"[%s|%s|%05d|%02d-%02d-%02d %02d:%02d:%02d:%03d|%s]",pmesc_AppTag,
					pscL_FileName,siL_LineNo,SL_TM.tm_mday,SL_TM.tm_mon + 1,SL_TM.tm_year + 1900,
					SL_TM.tm_hour,SL_TM.tm_min,SL_TM.tm_sec,uiL_milliSec,pscL_DBGLevel);
		}
		else
		{
			fprintf(pmeS_File,"[%s|%s|%05d|%02d-%02d-%02d %02d:%02d:%02d:%03d|%s][%04x]",pmesc_AppTag,
					pscL_FileName,siL_LineNo,SL_TM.tm_mday,SL_TM.tm_mon + 1,SL_TM.tm_year + 1900,
					SL_TM.tm_hour,SL_TM.tm_min,SL_TM.tm_sec,uiL_milliSec,pscL_DBGLevel,pvL_ptr);
		}

		meb_Is2Log = true;
	}
	catch(...)
	{
		meb_Is2Log = false;
	}
}

void CEventLog::mcfn_logMessage(const char *pscL_Format,...)
{
	try
	{
		if(!meb_Is2Log)
			return;

		va_list	   args;
		va_start(args, pscL_Format);

		if(vsprintf(pmesc_TempBuffer, pscL_Format, args ) < 0x00)
			return;

		va_end(args);
		if(pmeS_File != NULL)
		{
			fprintf(pmeS_File,"%s\n",pmesc_TempBuffer);
			fflush(pmeS_File);
		}
		if((mcu8_DBGLevel & CONSOLE) == 0)
			return;
		fprintf(stdout,"[%s]%s\n",pmesc_AppTag,pmesc_TempBuffer);
		fflush(stdout);
	}
	catch(...)
	{
	}
}

void CEventLog::mcfn_logSS7Message(STSMSG_SS7 *pSL_Msg)
{
	if((mcu8_DBGLevel & LOGMSG) == 0)
	{
		return;
	}	
	if(pmeS_File == NULL)
		return;
	unsigned long ulL_InstanceId = 0x00;

	meC_CriticalSection.Lock();
	u8 *pu8L_Pptr = stsget_param(pSL_Msg);
	struct tm SL_TM;
	unsigned short uiL_milliSec=0;
	if(mefn_changeFileIfReq(SL_TM,uiL_milliSec)==false){
		meC_CriticalSection.Unlock();		
		return;
	}
	ulL_InstanceId = (unsigned long)pSL_Msg->hdr.next;

	fprintf(pmeS_File,"[~SS7~%02d-%02d-%02d %02d:%02d:%02d:%03d]I%02x-M-t%04x-i%04x-f%02x-d%02x",\
			SL_TM.tm_mday,SL_TM.tm_mon + 1,SL_TM.tm_year + 1900, SL_TM.tm_hour,\
			SL_TM.tm_min,SL_TM.tm_sec,uiL_milliSec,\
			ulL_InstanceId,pSL_Msg->hdr.type,pSL_Msg->hdr.id,pSL_Msg->hdr.src,pSL_Msg->hdr.dst);

	if(pSL_Msg->hdr.rsp_req != 0)
		fprintf(pmeS_File,"-r%04x",pSL_Msg->hdr.rsp_req);
	if(pSL_Msg->hdr.err_info != 0)
		fprintf(pmeS_File,"-e%08x",pSL_Msg->hdr.err_info);
	if(pSL_Msg->hdr.hclass != 0)
		fprintf(pmeS_File,"-h%02x",pSL_Msg->hdr.hclass);

	fprintf(pmeS_File,"-s%02x-L%02x-p",pSL_Msg->hdr.status,pSL_Msg->len);

	if(pSL_Msg->len > MAX_PARAM_LEN)
		pSL_Msg->len=MAX_PARAM_LEN;
	for(int i = 0; i < pSL_Msg->len; i++)
		fprintf(pmeS_File,"%02x",*pu8L_Pptr++);

	fprintf(pmeS_File,"\n");
	fflush(pmeS_File);
	meC_CriticalSection.Unlock();		
}

void CEventLog::mcfn_logU8Message(u8 *p,int len)
{
	if((mcu8_DBGLevel & LOGMSG) == 0)
	{
		return;
	}

	if(pmeS_File == NULL)                                                                              
		return;                                                                                    

	mcfn_Lock();

	int i = 0x00;
	while(i < len)
	{
		fprintf(pmeS_File,"%02x",*p++);
		i++;
	}

	fprintf(pmeS_File,"\n");
	fflush(pmeS_File);
	mcfn_Unlock();

}

void CEventLog::mcfn_logByteStream(char *pscL_MsgTag,unsigned char *pu8L_Pptr,int siL_Len)                 
{                                                                                                          
	if((mcu8_DBGLevel & LOGMSG) == 0)
	{
		return;
	}

	if(pmeS_File == NULL)                                                                              
		return;                                                                                    

	meC_CriticalSection.Lock();
	struct tm SL_TM;
	unsigned short uiL_milliSec=0;
	if(mefn_changeFileIfReq(SL_TM,uiL_milliSec)==false){
		meC_CriticalSection.Unlock();		
		return;
	}

	fprintf(pmeS_File,"[%s|%02d-%02d-%02d %02d:%02d:%02d:%03d|%s-Len:%ld]-",pmesc_AppTag,\
			SL_TM.tm_mday,SL_TM.tm_mon + 1,SL_TM.tm_year + 1900,SL_TM.tm_hour,\
			SL_TM.tm_min,SL_TM.tm_sec,uiL_milliSec,pscL_MsgTag,siL_Len);                        

	for(int i=0;i< siL_Len;i++)                                                                        
		fprintf(pmeS_File,"%02x",*pu8L_Pptr++);                                                    

	fprintf(pmeS_File,"\n");                                                                           
	fflush(pmeS_File);
	meC_CriticalSection.Unlock();                                                                      
}
