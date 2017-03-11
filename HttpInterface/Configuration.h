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

		int mesi_httpPort;
                int mesi_esmePort;
                int mesi_esmeSecPort;

		char pmesc_logPath[1024];
		char pmesc_dbIp[40];
		char pmesc_dbUserName[40];
		char pmesc_dbPassword[40];
		char pmesc_dbName[40];

                char pmesc_httpSecurFile[64];
                char pmesc_esmeIp[40];
                char pmesc_esmeSecIp[40];
             

	public:
		CConfiguration();
		bool mcfn_loadConfigurations();
		int mcfn_getModuleId();
		int mcfn_getLogLevel();
		int mcfn_getLogChangeInterval();
		int mcfn_getNumOfWorkerThreads();
		int mcfn_getUssdTimeout();

		int mcfn_getHttpPort();
                int mcfn_getEsmePort();
                int mcfn_getEsmeSecPort();

                const char* mcfn_getHttpSecureFile();
                const char* mcfn_getEsmeIp();
                const char* mcfn_getEsmeSecIp();

		const char* mcfn_getLogPath();
		const char* mcfn_getDbIp();
		const char* mcfn_getDbUserName();
		const char* mcfn_getDbPassword();
		const char* mcfn_getDbName();

};

inline int CConfiguration::mcfn_getHttpPort()
{
     return mesi_httpPort;
}

inline int CConfiguration::mcfn_getEsmePort()
{
    return mesi_esmePort;
}

inline int CConfiguration::mcfn_getEsmeSecPort()
{
    return mesi_esmeSecPort;
}

inline const char* CConfiguration::mcfn_getHttpSecureFile()
{
    return pmesc_httpSecurFile;
}
                
inline const char* CConfiguration::mcfn_getEsmeIp()
{
    return pmesc_esmeIp;
}
inline const char* CConfiguration::mcfn_getEsmeSecIp()
{
      return pmesc_esmeSecIp;
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
#endif
