#ifndef __CONFIGURATIONS__
#define __CONFIGURATIONS__
#include"Get_pri_profile.h"
class CConfiguration{
	private:
		int mesi_moduleId;
		int mesi_logLevel;
		int mesi_logFileChangeInterval;
		int mesi_numOfWorkerThreads;
		char pmesc_logPath[1024];
		
	public:
		CConfiguration();
		bool mcfn_loadConfigurations();
		int mcfn_getModuleId();
		int mcfn_getLogLevel();
		int mcfn_getLogChangeInterval();
		int mcfn_getNumOfWorkerThreads();
		const char* mcfn_getLogPath();
	
		
};
inline int CConfiguration::mcfn_getModuleId(){
	return mesi_moduleId;
}
inline int CConfiguration::mcfn_getLogLevel(){
	return mesi_logLevel;
}
inline int CConfiguration::mcfn_getLogChangeInterval(){
	return mesi_logFileChangeInterval;
}
inline int CConfiguration::mcfn_getNumOfWorkerThreads(){
	return mesi_numOfWorkerThreads;
}
inline const char* CConfiguration::mcfn_getLogPath(){
	return pmesc_logPath;
}
#endif
