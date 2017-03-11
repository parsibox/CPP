#include"Util.h"

bool Util::SplitFields(std::string CL_InStr,
		std::vector<std::string>& CL_Fields,
		const std::string& CL_Tockenizer,
		bool bL_IsTrim)
{
	if(CL_InStr =="") return false;
	char *pscL_InputStr=new char[CL_InStr.length()+1];
	strcpy(pscL_InputStr,CL_InStr.c_str());
	char *pscL_Token =NULL;
	if((pscL_Token=strtok(pscL_InputStr, CL_Tockenizer.c_str()))==NULL)
		return false;

	if(bL_IsTrim) trim(pscL_Token);
	CL_Fields.push_back(pscL_Token);

	while(pscL_Token = strtok(NULL, CL_Tockenizer.c_str()))
	{
		if(bL_IsTrim)	trim(pscL_Token);
		CL_Fields.push_back(pscL_Token);
	}	
	delete pscL_InputStr;
	return true;
}

std::string Util::trim(const std::string CL_StrToTrim)
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


