#ifndef __ICONFIGURATIONS__
#define __ICONFIGURATIONS__
#include"Get_pri_profile.h"
#include<string>
#define LABLE_NAME (char*) mdC_moduleTag.c_str()
namespace FrameWork
{
class IConfiguration{
        protected:
                std::string mdC_moduleTag;
	private:
		int mesi_moduleId;
		int mesi_logLevel;
		int mesi_logFileChangeInterval;
		int mesi_numOfWorkerThreads;
		char pmesc_logPath[1024];
		
	public:
		IConfiguration(std::string CL_moduleTag);
		bool mcfn_loadConfigurations();
		int mcfn_getModuleId();
		int mcfn_getLogLevel();
		int mcfn_getLogChangeInterval();
		int mcfn_getNumOfWorkerThreads();
		const char* mcfn_getLogPath();
	
		
};
inline int IConfiguration::mcfn_getModuleId(){
	return mesi_moduleId;
}
inline int IConfiguration::mcfn_getLogLevel(){
	return mesi_logLevel;
}
inline int IConfiguration::mcfn_getLogChangeInterval(){
	return mesi_logFileChangeInterval;
}
inline int IConfiguration::mcfn_getNumOfWorkerThreads(){
	return mesi_numOfWorkerThreads;
}
inline const char* IConfiguration::mcfn_getLogPath(){
	return pmesc_logPath;
}
}
#endif
