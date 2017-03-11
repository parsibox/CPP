
#ifndef _STS_LOGFILE_H
#define _STS_LOGFILE_H

///////////////////////////////////////////////////////////////////////////////
// This class is used for creating Logfiles
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

class CSTSLogFile
{
public:
	CSTSLogFile(char *);					//Constructor with filename as argument
	virtual ~CSTSLogFile();				//Destructor

friend	CSTSLogFile & operator<< (CSTSLogFile&,const char *);	//Operator overloaded for cosnt char *
	void mbfn_PrintTimeStamp();			//writes the timestamp
protected:
private:
	FILE *pmvS_File;					//Maintains the file handle

};



CSTSLogFile & operator << (CSTSLogFile& pmvS_File,const char * pscL_String);

#endif

