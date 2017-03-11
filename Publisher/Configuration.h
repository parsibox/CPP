#ifndef __CONFIGURATIONS__
#define __CONFIGURATIONS__
#include"Get_pri_profile.h"
class CConfiguration{
	private:
		int mesi_moduleId;
		int mesi_logLevel;
		int mesi_logFileChangeInterval;
		int mesi_numOfWorkerThreads;
		int mesi_channelId;
		int mesi_taskPoolInterval;
		int mesi_taskAssignmentInterval;
		int mesi_bunchSize;
		char pmesc_logPath[1024];
		char pmesc_dbIp[40];
		char pmesc_dbUserName[40];
		char pmesc_dbPassword[40];
		char pmesc_dbName[40];
		
	public:
		CConfiguration();
		bool mcfn_loadConfigurations();
		int mcfn_getModuleId();
		int mcfn_getLogLevel();
		int mcfn_getLogChangeInterval();
		int mcfn_getNumOfWorkerThreads();
		int mcfn_getChannelId();
		int mcfn_getTaskPoolInterval();
		int mcfn_getAssignmentInterval();
		int mcfn_BunchSize();
		const char* mcfn_getLogPath();
		const char* mcfn_getDbIp();
		const char* mcfn_getDbUserName();
		const char* mcfn_getDbPassword();
		const char* mcfn_getDbName();
	
		
};
inline int CConfiguration::mcfn_BunchSize(){
	return mesi_bunchSize;
}
inline int CConfiguration::mcfn_getAssignmentInterval(){
	return mesi_taskAssignmentInterval;
}
inline int CConfiguration::mcfn_getTaskPoolInterval(){
	return mesi_taskPoolInterval;
}
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
inline int CConfiguration::mcfn_getChannelId(){
	return mesi_channelId;
}
inline const char* CConfiguration::mcfn_getLogPath(){
	return pmesc_logPath;
}
inline const char* CConfiguration::mcfn_getDbIp(){
	return pmesc_dbIp;
}
inline const char* CConfiguration::mcfn_getDbUserName(){
	return pmesc_dbUserName;
}
inline const char* CConfiguration::mcfn_getDbPassword(){
	return pmesc_dbPassword;
}
inline const char* CConfiguration::mcfn_getDbName(){
	return pmesc_dbName;
}
#endif
