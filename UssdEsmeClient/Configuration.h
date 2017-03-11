#ifndef __CONFIGURATIONS__
#define __CONFIGURATIONS__
#include"Get_pri_profile.h"
class CConfiguration{
	private:
		int mesi_moduleId;
		int mesi_logLevel;
		int mesi_logFileChangeInterval;
		int mesi_numOfWorkerThreads;
		int mesi_ussdTimeout;
		int mesi_cdrPort;
		int mesi_cdrSecPort;
		int mesi_reloadPort;
		char pmesc_logPath[1024];
		char pmesc_dbIp[40];
		char pmesc_dbUserName[40];
		char pmesc_dbPassword[40];
		char pmesc_dbName[40];
		char pmesc_CdrIp[40];
		char pmesc_CdrSecIp[40];
		char pmesc_operatorServiceCode[40];
		char pmesc_endMessage[200];

	public:
		CConfiguration();
		bool mcfn_loadConfigurations();
		int mcfn_getModuleId();
		int mcfn_getLogLevel();
		int mcfn_getLogChangeInterval();
		int mcfn_getNumOfWorkerThreads();
		int mcfn_getUssdTimeout();
		const char* mcfn_getLogPath();
		const char* mcfn_getDbIp();
		const char* mcfn_getDbUserName();
		const char* mcfn_getDbPassword();
		const char* mcfn_getDbName();
		const char* mcfn_getCdrIp(){return pmesc_CdrIp;}
		const char* mcfn_getCdrSecIp(){return pmesc_CdrSecIp;}
		int mcfn_getCdrPort(){return mesi_cdrPort;}
		int mcfn_getCdrSecPort(){return mesi_cdrSecPort;}
		const char* mcfn_getOpeartorServiceCode();
		const char* mcfn_getEndMessage();
		int mcfn_getReloadPort();

};

inline int CConfiguration::mcfn_getReloadPort()
{
	return mesi_reloadPort;
}
inline const char* CConfiguration::mcfn_getEndMessage()
{
	return pmesc_endMessage;
}
inline int CConfiguration::mcfn_getUssdTimeout(){
	return mesi_ussdTimeout;
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
inline const char* CConfiguration::mcfn_getOpeartorServiceCode(){
	return pmesc_operatorServiceCode;
}


#endif
