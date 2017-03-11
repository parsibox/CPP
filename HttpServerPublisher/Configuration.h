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
		char pmesc_logPath[1024];
		char pmesc_dbIp[40];
		char pmesc_dbUserName[40];
		char pmesc_dbPassword[40];
		char pmesc_dbName[40];
		int mesi_httpPort;
                char pmesc_sslFileName[120];
		
	public:
		CConfiguration();
		bool mcfn_loadConfigurations();
		int mcfn_getModuleId();
		int mcfn_getLogLevel();
		int mcfn_getLogChangeInterval();
		int mcfn_getNumOfWorkerThreads();
		const char* mcfn_getLogPath();
		const char* mcfn_getDbIp();
		const char* mcfn_getDbUserName();
		const char* mcfn_getDbPassword();
		const char* mcfn_getDbName();
                int mcfn_getHttpPort();
                const char* mcfn_getSslFileName();
	
		
};
inline int CConfiguration::mcfn_getHttpPort()
{
  return mesi_httpPort;
}
inline const char* CConfiguration::mcfn_getSslFileName()
{
  return pmesc_sslFileName;
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
