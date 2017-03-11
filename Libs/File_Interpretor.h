////////////////////////////////////////////////////////////////////////////////////////////
// All Rights Reserved Six DEE Telecom Solutions, Bangalore.
// Class  : CFileInterpretor
// Purpose: Main functionality of this class is to reads a file to get the data separated 
//		    by some token. In addition it also provide methods(member and static) to get 
// 			various statistics about file e.g. Number of lines ...
// Project Manager: Amiya Ranjan Thakur
// Author : Sunil Khatri
// Date   : 22-12-2007
// Version Details:(Please mention Author and Date while documenting about Changes )
//      +-----------+----------------------------------------------------------------+
//      | Version   |                          Comments                              |
//      +-----------+----------------------------------------------------------------+
//      | V1.0.0    |   Initial version. (Sunil/22-12-2007) 						 |
//      +-----------+----------------------------------------------------------------+
//      |           |                                                                |
//      +-----------+----------------------------------------------------------------+
//TODO: Use inheritance to divide the class in general db configuration and db operations
// 	 	so that same base class can be used with other modules also e.g. Memory class
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __FILE_INTERPRETOR_H
#define __FILE_INTERPRETOR_H

#include<iostream>
#include<fstream> 
#include<vector>
#include<string>
#include"STSException.h"

class CFileInterpretor
{
private:
	/// Input file stream object
	std::ifstream meC_InputFile;
	
	/// File name 
	std::string   meC_FileName;
	
	/// Current line number index: Starting from 0
	long 		  mesl_CurrentLineNumber;

	/// Common initialization function for all constructors	
	void mefn_Initialize();
	
public:
	/// Defaule constructor
	CFileInterpretor();
	/// Constructor with file name to open
	CFileInterpretor(const std::string& CL_FileName);
	/// Destructor
	~CFileInterpretor();
	
	/// Open the file
	bool 			mcfn_Open(std::string CL_FileName= "");
	
	/// Handles all closing related operations
	void 			mcfn_Close();
	
	/// Rewind the file to the begining.
	void 			mcfn_Rewind();
	
	/// Returns the total number of lines(all/except null lines)
	long  			mcfn_TotalNumber_Of_Lines(bool bL_CareNull_Lines = false);
	
	/// Returns next line from the read pointer of the file.
	bool 			mcfn_GetNextLine(std::string& CL_OutputLine);
		
	/// increment and returns next line number.
	long   	 		mcfn_NextLineNumber();

	/// Returns the fields of current line of file.
	bool 			mcfn_GetFields(std::vector<std::string>& CL_Fields, 
								   std::string CL_Tockenizer,
								   bool bL_IsTrim= false);

	/// Returns the fields of supplied line tokenized with supplied tokenizer
	static bool mcfn_GetFields(std::string CL_InStr,
							   std::vector<std::string>& CL_Fields, 
							   const std::string& CL_Tockenizer,
							   bool bL_IsTrim= false);
	
	/// Trim the supplied string: Removes left and right spaces.
	static std::string mcfn_Trim(const std::string &CL_StrToTrim); 	
	
	/// Trim the supplied const char* string: Removes left and rightt spaces.
	static char* mcfn_Trim(const char* pscL_StrToTrim);
};
#endif //__FILE_INTERPRETOR_H
