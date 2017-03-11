#ifndef __UTIL__H__
#define __UTIL__H__
#include<vector>
#include<string>

class Util
{
	public:
		static bool SplitFields(std::string CL_InStr,
				std::vector<std::string>& CL_Fields, 
				const std::string& CL_Tockenizer,
				bool bL_IsTrim= false);
		static std::string trim(const std::string CL_StrToTrim);
};

#endif
