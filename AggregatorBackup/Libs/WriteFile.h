/*
 * WriteFile.h
 *
 *  Created on: July 12, 2012
 *      Author: surya
 */

#ifndef CWRITEFILE_H_
#define CWRITEFILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include<list>
#include"AutoLocker.h"
#include<STSOSUtil.h>
class CFileDetails;
typedef std::list <CFileDetails *> FILE_LIST;

class CWriteFile {
	private:
		long mesl_MaxNumOfRecordsInFile;
		long mesl_FileChangeTime;
		int mesi_FileMaxSize;
		int mesi_FileCounter;
		THREAD_HANDLE meC_ThreadHandle;
		FILE_LIST meS_ListOfFile;
		CCriticalSection meC_WriteLock;
		std::string  meC_FileGenarationPath;
		std::string meC_FilePostFix;
		std::string meC_FilePreFix;
		std::string meC_FormatString;
		int mesi_timeZoneInSec;
		int mesi_numOfPartsInDay;		
		int mesi_numOfSecPerPart;
	private:	
		void mefn_addToList(CFileDetails *pSL_Param);
		CFileDetails* mefn_getFileDetails(long lL_time);
		int mefn_getSystemTimeZoneInSec();
	public:
		CWriteFile();
		~CWriteFile();

		void mcfn_SetFileGenarationPath(std::string CL_SourceFilePath);
		void mcfn_SetFilePostFix(std::string CL_PostFix);
		void mcfn_SetFilePreFix(std::string CL_PreFix);
		void mcfn_SetMaxNumOfRecordsInFile(long lL_MaxNumOfRecordsInFile);
		void mcfn_SetFileChangeTime(long lL_FileChangeTime);
		void mcfn_SetFileMaxSize(int iL_FileMaxSizeinMB);
		void mcfn_initialise(std::string CL_SourceFilePath,std::string CL_PreFix,std::string CL_PostFix,long lL_MaxNumOfRecordsInFile,long lL_FileChangeTime,int iL_FileMaxSizeInMB,int iL_numOfPartsInDay=1,std::string CL_FormatString="",bool bL_isTimerThread=true);

		bool mcfn_AddStringToFile(std::string CL_StringToInsertIntoFile,long lL_time=time(NULL));
		void* mcfn_ThCheckFileDetails();
		bool mcfn_CloseFile();
		std::string mcfn_getFormatString() const ;	
		std::string mcfn_getFileGenPath() const;
		std::string mcfn_getFilePostfix() const;
		std::string mcfn_getFilePrefix() const;
		int mcfn_getFileCounter() ;
		std::string mcfn_getPresentFile(long lL_time=time(NULL),bool is_withPath=false);
		void mcfn_checkCloseFilesByTimer();
		int mcfn_getTimeZoneInSec() const;
		int mcfn_getNumberOfSecPerPart() const;
};

class  CFileDetails{
	private:
		
		long mesl_FileCreatedTime;
		long mesl_CurrentRecordCount;
		int mesi_FileCurrentSize;
		int mesi_CurrentDayOfTheYear;
		std::ofstream meC_FileOutputStream;
		std::string meC_PresentFileName;
		std::string meC_PresentFileNameWithPostFix;
		CWriteFile *pmeC_Writer;
		
		void mefn_getFormatDate(long siL_Value,char* pscL_Format, char* pscL_Date, int siL_size);
		bool mefn_IsFileSizeExited(int iL_FileMaxSize);
		bool mefn_fileExists(const char *fileName);
	public:	
		CFileDetails(CWriteFile *pCL_writer){
			mesl_FileCreatedTime=0;
			mesl_CurrentRecordCount=0;
			mesi_FileCurrentSize=0;
			mesi_CurrentDayOfTheYear=0;
			meC_PresentFileName.clear();
			pmeC_Writer=pCL_writer;
			meC_FileOutputStream.exceptions (std::ofstream::failbit | std::ofstream::badbit);
		}
		~CFileDetails(){
			if(meC_PresentFileName.empty()==false)
			 rename(meC_PresentFileName.c_str(),meC_PresentFileNameWithPostFix.c_str());	
		}
		std::string mcfn_GetFileName(long lL_time=0);
		bool mcfn_OpenFile(long lL_time=0);
		bool mcfn_ChangeFile(long lL_time=0);
		bool mcfn_isFileNeedToChange(long lL_FileChangeTime,long lL_MaxNumOfRecodes,int lL_MaxSize);
		bool mcfn_CloseFile();
		bool mcfn_AddStringToFile(std::string CL_StringToInsertIntoFile);
		int mcfn_getCurrDayOfYear() const;
		std::string mcfn_getPresentFileName() const;
		bool mcfn_createDirectory(std::string );
};
inline int CWriteFile::mcfn_getTimeZoneInSec() const{
	return mesi_timeZoneInSec;
}
inline int CWriteFile::mcfn_getNumberOfSecPerPart() const{
	return mesi_numOfSecPerPart;
}
inline std::string CFileDetails::mcfn_getPresentFileName() const{
	return meC_PresentFileNameWithPostFix;
}

inline int CFileDetails::mcfn_getCurrDayOfYear() const{
	return mesi_CurrentDayOfTheYear;
}

inline std::string CWriteFile::mcfn_getFormatString() const{
	return meC_FormatString;
}

inline std::string CWriteFile::mcfn_getFileGenPath() const{
	return meC_FileGenarationPath;
}

inline std::string CWriteFile::mcfn_getFilePostfix() const{
	return meC_FilePostFix;
}

inline std::string CWriteFile::mcfn_getFilePrefix() const{
	return meC_FilePreFix;
}

inline int CWriteFile::mcfn_getFileCounter() {
	if(mesi_FileCounter>=9999){
		mesi_FileCounter=0;
	}
	++mesi_FileCounter;
	return mesi_FileCounter;
}

#endif /* CWRITEFILE_H_ */
