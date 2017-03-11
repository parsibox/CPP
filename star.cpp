#include<string>
#include<vector>
#include <string.h>
#include <iostream>

bool mcfn_GetFields(std::string CL_InStr,
		std::vector<std::string>& CL_Fields,
		const std::string& CL_Tockenizer,
		bool bL_IsTrim)
{
	char *pscL_InputStr=new char[CL_InStr.length()+1];
	strcpy(pscL_InputStr,CL_InStr.c_str());
	char *pscL_Token =NULL;
	if((pscL_Token=strtok(pscL_InputStr, CL_Tockenizer.c_str()))==NULL)
		return false;

	CL_Fields.push_back(pscL_Token);

	while(pscL_Token = strtok(NULL, CL_Tockenizer.c_str()))
	{
		CL_Fields.push_back(pscL_Token);
	}	
	delete pscL_InputStr;
	return true;
}

int main()
{
	std::string input ="1234";
	std::vector<std::string> tokens;
	mcfn_GetFields(input,tokens,"*",true);
        for(int i=0;i<tokens.size();i++)
        std::cout<<tokens[i]<<"--> "<<i<<std::endl;
	return 0;
}
