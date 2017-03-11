/*
 * CWriteFile.cpp
 *
 *  Created on: July 12, 2012
 *      Author: surya
 */

#include "WriteFile.h"
#include "EventLog.h"
#include"signalHandler.h"
#include <cerrno>
#include <sstream>
extern CEventLog CG_EventLog;
static FNVOID fnG_Wrapper_CheckFileDetails(void *pvL_This) {
        try {
                (static_cast<CWriteFile*> (pvL_This))->mcfn_ThCheckFileDetails();
		DBG_CRITICAL((CG_EventLog),("Thread Execution stoped which is checking all file Descriptors"));
        } catch (std::bad_cast x) {
                //DBG_ERROR((CG_EventLog), ("Unable to start TaskPollThread"));
        }
        RETURN;
}


CWriteFile::CWriteFile() {
	// TODO Auto-generated constructor stub
	meC_FileGenarationPath.clear();
	meC_FilePostFix.clear();
	meC_FilePreFix.clear();
	mesl_MaxNumOfRecordsInFile=0;
	mesl_FileChangeTime=0;
	mesi_FileCounter=0;
	mesi_FileMaxSize=0;
	meC_ThreadHandle=0;
	//mesi_timeZoneInSec=-1*(int)(mefn_getSystemTimeZone()*60*60);
	
}
CWriteFile::~CWriteFile() {
	// TODO Auto-generated destructor stub
	mcfn_CloseFile();
	//DBG_CRITICAL((CG_EventLog),("object destroied"));
}
int CWriteFile::mefn_getSystemTimeZoneInSec()
{
	struct timezone SL_timeZone;
	struct timeval  SL_timeVal;
	gettimeofday(&SL_timeVal,&SL_timeZone);
	return (int)(SL_timeZone.tz_minuteswest*60);
}
void CWriteFile::mcfn_SetFileGenarationPath(std::string CL_SourceFilePath){
	meC_FileGenarationPath.assign(CL_SourceFilePath);
}

void CWriteFile::mcfn_SetFilePostFix(std::string CL_PostFix){
	meC_FilePostFix.assign(CL_PostFix);
}

void CWriteFile::mcfn_SetFilePreFix(std::string CL_PreFix){
	meC_FilePreFix.assign(CL_PreFix);
}

void CWriteFile::mcfn_SetMaxNumOfRecordsInFile(long lL_MaxNumOfRecordsInFile){
	mesl_MaxNumOfRecordsInFile=lL_MaxNumOfRecordsInFile;
}

void CWriteFile::mcfn_SetFileChangeTime(long lL_FileChangeTime){
	mesl_FileChangeTime=lL_FileChangeTime;
}
void CWriteFile::mcfn_SetFileMaxSize(int iL_FileMaxSizeinMB){
	mesi_FileMaxSize=iL_FileMaxSizeinMB;
}

//Initialising the params
void CWriteFile::mcfn_initialise(std::string CL_SourceFilePath,std::string CL_PreFix,std::string CL_PostFix,long lL_MaxNumOfRecordsInFile,long lL_FileChangeTime,int iL_FileMaxSizeInMB,int iL_numOfPartsInDay,std::string CL_FormatString,bool bL_isTimerThread){
	char pscL_RenameCommand[1024];
	memset(pscL_RenameCommand,0x00,1024);
	
	meC_FileGenarationPath.assign(CL_SourceFilePath);
	meC_FilePostFix.assign(CL_PostFix);
	meC_FilePreFix.assign(CL_PreFix);
	mesl_MaxNumOfRecordsInFile=lL_MaxNumOfRecordsInFile;
	mesl_FileChangeTime=lL_FileChangeTime;
	mesi_FileMaxSize=iL_FileMaxSizeInMB;
	meS_ListOfFile.clear();	
	meC_FormatString.assign(CL_FormatString);
	if(meC_FormatString.empty()){
		meC_FormatString.assign("_%Y_%m_%d_%H_%M_%S_");
	}
	mesi_numOfPartsInDay=iL_numOfPartsInDay;
	mesi_numOfSecPerPart=86400;
	if(mesi_numOfPartsInDay>0){
		mesi_numOfSecPerPart=mesi_numOfSecPerPart/mesi_numOfPartsInDay;
	}
//	sprintf(pscL_RenameCommand,"rename .TEMP %s %s/*", meC_FilePostFix.c_str(),meC_FileGenarationPath.c_str());
//	system(pscL_RenameCommand);
	if(bL_isTimerThread)
	CSTSOSUtil::mcfn_beginAndDetachThread(meC_ThreadHandle,fnG_Wrapper_CheckFileDetails, this);	
}

bool CFileDetails::mefn_fileExists(const char *fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
	/*struct stat buf;
	  if (stat(fileName, &buf) != -1)
	  {
	  printf("IN:%s\n",fileName);
	  return true;
	  }
	  return false;*/
}
//Getting the file name with .TEMP as post fix of file name in <filepath>/<prifix>_YYYY_MM_D_HH_mm_ss_Counter.TEMP
std::string CFileDetails::mcfn_GetFileName(long lL_time){
	char pscl_fileName[1024];
	char pscl_TimeStamp[50];
	memset(pscl_fileName,0x00,sizeof(pscl_fileName));
	memset(pscl_TimeStamp,0x00,sizeof(pscl_TimeStamp));
	mesl_FileCreatedTime=time(NULL);
	mefn_getFormatDate(lL_time<=0?mesl_FileCreatedTime:lL_time,(char*)pmeC_Writer->mcfn_getFormatString().c_str(),pscl_TimeStamp,sizeof(pscl_TimeStamp));	
	mesi_CurrentDayOfTheYear=((lL_time<=0?mesl_FileCreatedTime:lL_time)+pmeC_Writer->mcfn_getTimeZoneInSec())/pmeC_Writer->mcfn_getNumberOfSecPerPart();
	int iL_fileCount=0;	
	while(1){
		iL_fileCount=pmeC_Writer->mcfn_getFileCounter();	
		sprintf(pscl_fileName,"%s/%s%s%d%s",pmeC_Writer->mcfn_getFileGenPath().c_str(),pmeC_Writer->mcfn_getFilePrefix().c_str(),pscl_TimeStamp,iL_fileCount,pmeC_Writer->mcfn_getFilePostfix().c_str());
		meC_PresentFileNameWithPostFix.assign(pscl_fileName);
		sprintf(pscl_fileName,"%s/%s%s%d.TEMP",pmeC_Writer->mcfn_getFileGenPath().c_str(),pmeC_Writer->mcfn_getFilePrefix().c_str(),pscl_TimeStamp,iL_fileCount);
		meC_PresentFileName.assign(pscl_fileName);
		if(!(mefn_fileExists(meC_PresentFileNameWithPostFix.c_str()) || mefn_fileExists(meC_PresentFileName.c_str()))){
			break;
		}
	}
	return meC_PresentFileName;
}
bool CFileDetails::mcfn_createDirectory(std::string CL_dirName){
	std::stringstream CL_dirPathStream;
	CL_dirPathStream.str("");
	CL_dirPathStream<<"mkdir -p "<<CL_dirName;
	if(system(CL_dirPathStream.str().c_str())!=0){
		CL_dirPathStream.str("");
		CL_dirPathStream<<"sh ./FolderNotPresent.sh "<<CL_dirName<<" &";
		system(CL_dirPathStream.str().c_str());
	}
}
//opening a new file .
bool CFileDetails::mcfn_OpenFile(long lL_time){
	mcfn_CloseFile();
	mesl_CurrentRecordCount=0;
	mcfn_GetFileName(lL_time<=0?time(NULL):lL_time);
	try{
		meC_FileOutputStream.open(meC_PresentFileName.c_str(),std::ios::out);
		if(meC_FileOutputStream.is_open()){
			return true;
		}
		else{
			return false;
		}
	}
	catch(std::ofstream::failure &e){
		DBG_ERROR((CG_EventLog),("Exception in opening a file :%s,%s",e.what(),strerror(errno)));
		mcfn_CloseFile();
		mcfn_createDirectory(pmeC_Writer->mcfn_getFileGenPath().c_str());
		return false;
	}
}
bool CFileDetails::mcfn_CloseFile(){
	   
	if(meC_FileOutputStream.is_open()){
                meC_FileOutputStream.flush();
                meC_FileOutputStream.close();
                rename(meC_PresentFileName.c_str(),meC_PresentFileNameWithPostFix.c_str());
        	DBG_INFO((CG_EventLog),("File closed and renamed as %s",meC_PresentFileNameWithPostFix.c_str()));
	}
	return true;
}

//changing the file by closing the existing file and calling open file method
bool CFileDetails::mcfn_ChangeFile(long lL_time){

	mcfn_CloseFile();
	return mcfn_OpenFile(lL_time<=0?time(NULL):lL_time);
}

//this is for getting the data in given format
void CFileDetails::mefn_getFormatDate(long siL_Value,char* pscL_Format, char* pscL_Date, int siL_size)
{
	struct tm tmp;
	localtime_r(&siL_Value,&tmp);
	if (strftime(pscL_Date, siL_size, pscL_Format, &tmp) == 0)
	{
		//DBG_ERROR((CG_EventLog), ("Unable to get the time string"));
	}
	else
	{ 
		//DBG_VERBOSE((CG_EventLog), ("Timestamp String: %s", pscL_Date));
	}
}

//This is to get the file size which is presently using .
bool CFileDetails::mefn_IsFileSizeExited(int iL_FileMaxSize)
{
	if(meC_PresentFileName.empty())
		return false;
	struct stat buf;
	int ret = stat(meC_PresentFileName.c_str(),&buf);
	if((mesi_FileCurrentSize >0) && (ret == -1))
	{
		return true;
	}
	if(ret != -1)
		mesi_FileCurrentSize = (buf.st_size)/(1024*1024);
	if(mesi_FileCurrentSize>=iL_FileMaxSize){
		return true;
	}
	else{
		return false;
	}
}
//Here we are checking file size, file duriation ,and number of records .if any one is crossed means we will change the file.
bool CFileDetails::mcfn_isFileNeedToChange(long lL_FileChangeTime,long lL_MaxNumOfRecodes,int lL_MaxSize)
{
	if((mesl_FileCreatedTime+lL_FileChangeTime<=time(NULL))|| mesl_CurrentRecordCount>=lL_MaxNumOfRecodes || mefn_IsFileSizeExited(lL_MaxSize)){
		return true;
	}
	return false;
}
//This is to add the string to file
bool CFileDetails::mcfn_AddStringToFile(std::string CL_StringToInsertIntoFile){
	if(mefn_fileExists(meC_PresentFileName.c_str()) && meC_FileOutputStream.is_open()){
		meC_FileOutputStream<<CL_StringToInsertIntoFile;			
		++mesl_CurrentRecordCount;
		//if(mesl_CurrentRecordCount%100==0)
		meC_FileOutputStream.flush();	
		return true;
	}
	else{
		return false;
	}
}

CFileDetails* CWriteFile::mefn_getFileDetails(long lL_time){
	CFileDetails *pCL_FileDetailsTemp=NULL;
	if(lL_time<=0){
		lL_time=time(NULL);
	}
	int iL_recordDayOftheYear=(int)(lL_time+(-1*mefn_getSystemTimeZoneInSec()))/mesi_numOfSecPerPart;
	for(FILE_LIST::iterator pCL_Itr=meS_ListOfFile.begin();pCL_Itr!=meS_ListOfFile.end();pCL_Itr++){
		if((*pCL_Itr)->mcfn_getCurrDayOfYear()==iL_recordDayOftheYear){
			pCL_FileDetailsTemp=(*pCL_Itr);
			if(pCL_FileDetailsTemp==NULL){
				pCL_Itr=meS_ListOfFile.erase(pCL_Itr);
			}
			break;
		}
	}	
	if(pCL_FileDetailsTemp==NULL){
		pCL_FileDetailsTemp=new CFileDetails(this);
		/*
		while(!pCL_FileDetailsTemp->mcfn_OpenFile(lL_time)){
			sleep(2);
		}*/
		pCL_FileDetailsTemp->mcfn_OpenFile(lL_time);
		mefn_addToList(pCL_FileDetailsTemp);	
	}
	return pCL_FileDetailsTemp;
}

void CWriteFile::mefn_addToList(CFileDetails *pSL_Param){
	meS_ListOfFile.push_back(pSL_Param);
}

std::string CWriteFile::mcfn_getPresentFile(long lL_time,bool is_withPath){
	CFileDetails *pCL_FileDetails=NULL;
	CAutoLocker CL_Lock(meC_WriteLock);
	pCL_FileDetails=mefn_getFileDetails(lL_time);
	if(pCL_FileDetails!=NULL){
		std::string CL_FileName="";
		CL_FileName=pCL_FileDetails->mcfn_getPresentFileName();
		if(!is_withPath)
			return CL_FileName.substr(CL_FileName.find_last_of('/')+1,CL_FileName.length());
		else
			return CL_FileName;
	}
	else
		return "";
}

bool CWriteFile::mcfn_AddStringToFile(std::string CL_StringToInsertIntoFile,long lL_time){
	bool bL_return=false;
	CFileDetails *pCL_FileDetails=NULL;
	try{
		CAutoLocker CL_Lock(meC_WriteLock);
		pCL_FileDetails=mefn_getFileDetails(lL_time);
		if(pCL_FileDetails->mcfn_isFileNeedToChange(mesl_FileChangeTime,mesl_MaxNumOfRecordsInFile,mesi_FileMaxSize)){
			if(!pCL_FileDetails->mcfn_OpenFile(lL_time)){
				DBG_ERROR((CG_EventLog),("Not able to open the file:%ld",this));
				return bL_return;
			}
		}
		while(!pCL_FileDetails->mcfn_AddStringToFile(CL_StringToInsertIntoFile)){
			if(!pCL_FileDetails->mcfn_OpenFile(lL_time)){
				DBG_ERROR((CG_EventLog),("Not able to open the file:%ld",this));
				return bL_return;
			}
		}
		bL_return=true;
	}
	catch(std::ofstream::failure &e){
		DBG_ERROR((CG_EventLog),("Exception in opening a file :%s[%s]",e.what(),strerror(errno)));
		delete pCL_FileDetails;
		pCL_FileDetails=NULL;
		bL_return=false;
	}
	return bL_return;
}
bool CWriteFile::mcfn_CloseFile(){
	CAutoLocker CL_Lock(meC_WriteLock);
	for(FILE_LIST::iterator pCL_Itr=meS_ListOfFile.begin();pCL_Itr!=meS_ListOfFile.end();pCL_Itr++){
		(*pCL_Itr)->mcfn_CloseFile();
		delete (*pCL_Itr);
		pCL_Itr=meS_ListOfFile.erase(pCL_Itr);
	}
	//DBG_CRITICAL((CG_EventLog),("Closing all Files opend by this object"));
	meS_ListOfFile.clear();
	//THREAD_EXITCODE CL_ExitCode;
	//CSTSOSUtil::mcfn_killThread(meC_ThreadHandle, CL_ExitCode);
	if(meC_ThreadHandle)
	pthread_cancel(meC_ThreadHandle);
}
void* CWriteFile::mcfn_ThCheckFileDetails(){
	DBG_INFO((CG_EventLog),("mcfn_ThCheckFileDetails Stated"));
	while(1){
		try{
		CSTSOSUtil::mcfn_sleep(mesl_FileChangeTime);
		mcfn_checkCloseFilesByTimer();
		}
		 catch(CSignalException e){
                        DBG_ERROR((CG_EventLog),("Exception:%s",e.what()));
                }
                catch(...){
                        DBG_ERROR((CG_EventLog),("Some unknown Exception came in mcfn_ThCheckFileDetails"));
                }
	
	}
	 DBG_INFO((CG_EventLog),("mcfn_ThCheckFileDetails Stoped"));
}

void CWriteFile::mcfn_checkCloseFilesByTimer(){
	CAutoLocker CL_Lock(meC_WriteLock);
	DBG_INFO((CG_EventLog),("Checking all files which are crossed the time to close"));
	for(FILE_LIST::iterator pCL_Itr=meS_ListOfFile.begin();pCL_Itr!=meS_ListOfFile.end();pCL_Itr++){
		if((*pCL_Itr)->mcfn_isFileNeedToChange(mesl_FileChangeTime,mesl_MaxNumOfRecordsInFile,mesi_FileMaxSize)){
			(*pCL_Itr)->mcfn_CloseFile();
			delete (*pCL_Itr);
			pCL_Itr=meS_ListOfFile.erase(pCL_Itr);
		}
	}
}
