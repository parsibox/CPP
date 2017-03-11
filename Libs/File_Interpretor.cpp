#include "File_Interpretor.h"


CFileInterpretor::CFileInterpretor()
{
	meC_FileName.clear();
	mefn_Initialize();
}
CFileInterpretor::CFileInterpretor(const std::string& CL_FileName)
				  :meC_FileName(CL_FileName)
{
	mefn_Initialize();
}

CFileInterpretor::~CFileInterpretor()
{
	mcfn_Close();
}

void CFileInterpretor::mefn_Initialize()
{
	mesl_CurrentLineNumber = -1;
}

/**
 * If file name is supplied as a parameter then updates the objects file name 
 * with supplied filename and opens it else directly opens the file 'objects filename'.
 * @param File name : std::string
 * @return true on success, false on failure.
 */
bool CFileInterpretor::mcfn_Open(std::string CL_FileName)
{
	CL_FileName!=""?meC_FileName=CL_FileName:meC_FileName ;
	meC_InputFile.open( meC_FileName.c_str(), std::ifstream::in);
	
	if(!meC_InputFile.good())
	{
		return false;	
	}
	return true;
}

/**
 * Performs the neccessary closing operations.
 * @param void
 * @retur void
 */
void CFileInterpretor::mcfn_Close()
{
	meC_InputFile.close();
	mefn_Initialize();
}

/**
 * Set the file read pointer to the begining of the file and re-initialize the file
 * file name.
 * @param void
 * @return void
 */
void CFileInterpretor::mcfn_Rewind()
{
	try{
		meC_InputFile.seekg(0, std::ios::beg);
		mesl_CurrentLineNumber=-1;
	}catch(std::ios_base::failure)
	{
		throw STSGenException(100, "Rewind failed : ios_base exception\n" );
	}
}

/**
 * Returns the total number of all lines OR number of non-null lines in the file according 
 * to passed bool flag.
 * @param bL_CareNull_Lines: bool, if True: Returns number of non-null 
 *		  lines else number of all lines.
 * @return Nubmer of lines: long.
 */
long CFileInterpretor::mcfn_TotalNumber_Of_Lines(bool bL_CareNull_Lines)
{
	std::string CL_Str;
	long slL_Count=0;
	//Opens a new instance of file to avoid O(N) time complexity in 
	//Rewinding the file and setting back the position of pointer
	//to older position.
	std::ifstream CL_Temp(meC_FileName.c_str(), std::ifstream::in);
	if(CL_Temp.good())
	{
		while(getline(CL_Temp, CL_Str, '\n'))
		{
			if(!bL_CareNull_Lines) slL_Count++;
			else if(bL_CareNull_Lines && CL_Str.empty()) slL_Count++;
		}
	}
	CL_Temp.close();
	return slL_Count;
}

/**
 * Copies next line from file to the passed std::string variable 
 * and increments the current line number.
 * @param std::string, Output string parameter.
 * @return bool : true on success, false on failure.
 */
bool CFileInterpretor::mcfn_GetNextLine(std::string& CL_OutputStr)
{
	if(getline(meC_InputFile, CL_OutputStr, '\n'))
	{
		mesl_CurrentLineNumber++;
		return true;
	}
	return false;
}

/**
 * Increment and return the next line number.
 * @param void
 * @return long: Next line number.
 */
long CFileInterpretor::mcfn_NextLineNumber()
{
	return ++mesl_CurrentLineNumber;
}


/** 
 * Increments the current line to next line and fills the passed 
 * vector objects with tokenized fields of current line.
 */
bool CFileInterpretor::mcfn_GetFields(std::vector<std::string>& CL_Fields,
		std::string CL_Tockenizer,
		bool bL_IsTrim)
{
	std::string CL_Str;
	if(getline(meC_InputFile, CL_Str, '\n'))
	{
		return mcfn_GetFields(CL_Str, CL_Fields, CL_Tockenizer, bL_IsTrim);
	}
	return false;
}

/**
 * Static member function to get the fields of passed line tokenized 
 * and trimed(or not trimed) according to passed parameters.
 */
bool CFileInterpretor::mcfn_GetFields(std::string CL_InStr,
						std::vector<std::string>& CL_Fields,
						const std::string& CL_Tockenizer,
						bool bL_IsTrim)
{
/*	if(CL_InStr =="") return false;
	std::string temp=CL_InStr;
	int Last_Index=0;
	int index=0;
	while((index=temp.find_first_of(CL_Tockenizer,Last_Index))!=std::string::npos )
	{
		CL_Fields.push_back(temp.substr(Last_Index, index-Last_Index));
		Last_Index = index+ CL_Tockenizer.length();
		printf("\n INDEX ::%d ,last index:%d\n",index,Last_Index);
	}	
	if(index!=Last_Index)
		CL_Fields.push_back(temp.substr(Last_Index, index-Last_Index));	
		printf("\n INDEX finel ::%d,last index:%d \n",index,Last_Index);
	return true;*/

	if(CL_InStr =="") return false;
	//This is added because if you are passing a string means it will get changed its original value so we are coping this to local variable.
	char *pscL_InputStr=new char[CL_InStr.length()+1];
	strcpy(pscL_InputStr,CL_InStr.c_str());
	char *pscL_Token =NULL;
   	if((pscL_Token=strtok(pscL_InputStr, CL_Tockenizer.c_str()))==NULL)
		return false;
	
	if(bL_IsTrim) mcfn_Trim(pscL_Token);
	CL_Fields.push_back(pscL_Token);
	
	while(pscL_Token = strtok(NULL, CL_Tockenizer.c_str()))
	{
		if(bL_IsTrim)	mcfn_Trim(pscL_Token);
		CL_Fields.push_back(pscL_Token);
	}	
	delete pscL_InputStr;
	return true;

}

/**
 * Static function that returns trimed copy of input string.
 * @param const std::string& , String whose trimed copy will be returned.
 * @return std::string, Returns trimed copy of input string.
 */
std::string CFileInterpretor::mcfn_Trim(const std::string& CL_StrToTrim)
{
	std::string::size_type start;
	std::string::size_type end;
	if((start = CL_StrToTrim.find_first_not_of(' '))== std::string::npos)
		start = 0;
	if((end = CL_StrToTrim.find_last_not_of(' '))== std::string::npos)
		end = 0;	
	if(start==0 and end ==0 and CL_StrToTrim[0]==' ') return "";
	return CL_StrToTrim.substr(start, end-start+1);
}

/**
 * Static overloaded function for triming.
 * @param const char* pscL_StrToTrim input string.
 * @return char*, Trimed string.
 */
char* CFileInterpretor::mcfn_Trim(const char* pscL_StrToTrim) 
{
	std::string CL_Str(pscL_StrToTrim);
	return (char*) mcfn_Trim(CL_Str).c_str();
}
