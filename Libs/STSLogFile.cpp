
#include "STSLogFile.h"
#include <time.h>

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Description : Constructor
// Input  : filename without the timestamp
// Output : Creates the file with the timestamp attached to passed
//			filename		
// Return : None
///////////////////////////////////////////////////////////////////////////////


CSTSLogFile::CSTSLogFile(char *pscL_Filename)
{
	char pscL_LogFilename[255];
	time_t LS_Time;
	struct tm *pLS_Time;

	//getting the time
	time(&LS_Time); 
	//converting the time into tm structure
	pLS_Time = localtime(&LS_Time);
	
	//framing the filename using the timestamp and filename passed
	sprintf(pscL_LogFilename,"%d%d%d%s.log",pLS_Time->tm_year,pLS_Time->tm_mon,
		pLS_Time->tm_mday,pscL_Filename);

	pmvS_File = NULL;	//initializing for safety
	//opening thr file in read write mode
	pmvS_File = fopen(pscL_LogFilename,"w+");
	if(pmvS_File == NULL)
	{
		//throwing an error in case of failure
		throw "Error opening file";
	}
}


///////////////////////////////////////////////////////////////////////////////
// Description : Constructor
// Input  : None
// Output : Closes the opened file if exists
// Return : None
///////////////////////////////////////////////////////////////////////////////

CSTSLogFile::~CSTSLogFile()
{
	if(pmvS_File != NULL)
		fclose(pmvS_File);	//closing the open file handle
}


///////////////////////////////////////////////////////////////////////////////
// Description : function used to print the timestamp
// Input  : None
// Output : writes the timestamp
// Return : None
///////////////////////////////////////////////////////////////////////////////

void CSTSLogFile::mbfn_PrintTimeStamp()
{
	try
	{
		time_t LS_Time;
		struct tm *pLS_Time;

		//getting the time
		time(&LS_Time); 
		//converting the time into tm structure
		pLS_Time = localtime(&LS_Time);

		//writing the time stamp to the file
		fprintf(pmvS_File,"[%02d-%02d-%4d][%02d:%02d:%02d] ",pLS_Time->tm_mday,
			pLS_Time->tm_mon,pLS_Time->tm_year + 1900,pLS_Time->tm_hour,
			pLS_Time->tm_min,pLS_Time->tm_sec);  
	}
	catch(...)
	{
	}

}

///////////////////////////////////////////////////////////////////////////////
// Description : overloaded function output operator for char *
// Input  : a char string
// Output : writes the string into the file 
// Return : returns a reference of the Logfile
///////////////////////////////////////////////////////////////////////////////

CSTSLogFile & operator << (CSTSLogFile& pmvS_File,const char * pscL_String)
{
	//writing into file the passed string
	pmvS_File.mbfn_PrintTimeStamp();
	fprintf(pmvS_File.pmvS_File,"%s\n",pscL_String);
	fflush(pmvS_File.pmvS_File);
	return (pmvS_File);
}
